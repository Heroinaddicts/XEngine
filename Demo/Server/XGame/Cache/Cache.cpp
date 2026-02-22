#include "Cache.h"

Api::iEngine* g_Engine = nullptr;
ISQLCache* g_SQLCache = nullptr;
IDb* g_Db = nullptr;
IDbGroup* g_DbGroup = nullptr;
static Cache* s_Self = nullptr;

FrameworkSession* g_CacheClientSession = nullptr;

enum eCacheTimeID {
    ReleaseGateServerSession = 0,
    PrintCacheCount = 1
};

static const int s_ReconnectTimerDelay = 100;

static int s_CacheServerPort = 0;
static UInt64 GetDBOptMask() {
    static UInt64 s_Mask = 0;
    return s_Mask++;
}

static std::unordered_map<UInt64, std::map<eTable, ISQLCacheTable*>> s_AccountSQLCache;

bool Cache::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    IFrameworkSession::SetEngine(g_Engine);
    s_Self = this;

    FrameworkSession::RegistServerProtobuf(eFrameworkID::CacheRequest, Cache::OnCacheRequest);
    return true;
}

bool Cache::Launch(Api::iEngine* const engine) {
    UInt16 portForGame = engine->GetLaunchParameterUInt16("PortForGame");
    engine->GetNetApi()->LaunchTcpServer(this, "0.0.0.0", portForGame, 20 * SafeSystem::Network::MB);

    std::string mysqlhost = g_Engine->GetLaunchParameter("MysqlHost");
    const int mysqlport = g_Engine->GetLaunchParameterInt32("MysqlPort");
    const std::string mysqldb = g_Engine->GetLaunchParameter("MysqlDb");
    const std::string mysqlusername = g_Engine->GetLaunchParameter("MysqlUsername");
    const std::string mysqlpassword = g_Engine->GetLaunchParameter("MysqlPassword");
    const std::string redishost = g_Engine->GetLaunchParameter("RedisHost");
    const int redisport = g_Engine->GetLaunchParameterInt32("RedisPort");
    const std::string redispassword = g_Engine->GetLaunchParameter("RedisPassword") ? g_Engine->GetLaunchParameter("RedisPassword") : "";
    const int dbGroupCount = g_Engine->GetLaunchParameterInt32("DbGroupCount");

    GetModule(g_Engine, g_SQLCache, SQLCache);
    GetModule(g_Engine, g_Db, Db);
    g_DbGroup = g_Db->CreateGroup(dbGroupCount, mysqlhost, mysqlport, mysqldb, mysqlusername, mysqlpassword, redishost, redisport, redispassword);

    START_TIMER(g_Engine, this, eCacheTimeID::PrintCacheCount, 1000, Api::Unlimited, 1000, 0);
    return true;
}

bool Cache::Destroy(Api::iEngine* const engine) {
    return true;
}

FrameworkSession* Cache::CreateGameServerSession() {
    FrameworkSession* session = FrameworkSession::Create(eFrameworkSessionType::Game);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Connected, Cache::OnCacheClientSessionConnected);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Disconnected, Cache::OnCacheClientSessionDisconnected);
    return session;
}

void Cache::ReleaseGameServerSession(FrameworkSession* session) {
    START_TIMER(g_Engine, s_Self, eCacheTimeID::ReleaseGateServerSession, 10, 1, 10, session);
}

void Cache::OnCacheClientSessionConnected(FrameworkSession* const session) {
    TraceLog(g_Engine, "OnCacheClientSessionConnected");
    if (nullptr != g_CacheClientSession) {
        Cache::ReleaseGameServerSession(g_CacheClientSession);
        g_CacheClientSession->Close();
    }
    g_CacheClientSession = session;
}

void Cache::OnCacheClientSessionDisconnected(FrameworkSession* const session) {
    TraceLog(g_Engine, "OnCacheClientSessionDisconnected");
    Cache::ReleaseGameServerSession(session);
    if (session == g_CacheClientSession) {
        g_CacheClientSession = nullptr;

        for (auto itor = s_AccountSQLCache.begin(); itor != s_AccountSQLCache.end(); itor++) {
            for (auto i = itor->second.begin(); i != itor->second.end(); i++) {
                g_SQLCache->ReleaseTable(i->second);
            }
        }

        s_AccountSQLCache.clear();
    }
}

Api::iTcpSession* Cache::OnMallocTcpSession(const std::string& remoteIp, const UInt16 remotePort) {
    return Cache::CreateGameServerSession();
}

void Cache::OnError(Api::iTcpSession* session) {

}

void Cache::OnRelease() {

}

void Cache::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eCacheTimeID::ReleaseGateServerSession: {
        FrameworkSession::Release((FrameworkSession*)context);
        break;
    }
    case eCacheTimeID::PrintCacheCount: {
        TraceLog(g_Engine, "Cache Count %d", s_AccountSQLCache.size());
        break;
    }
    }
}

__forceinline void OnCacheRequestCreate(IFrameworkSession* const session, const UInt64 sessionId, const UInt64 account, const eTable tab, const oCacheRequest& req) {
    auto itor = s_AccountSQLCache.find(account);
    if (s_AccountSQLCache.end() == itor) {
        itor = s_AccountSQLCache.insert({ account, std::map<eTable, ISQLCacheTable*>() }).first;
    }

    auto tabItor = itor->second.find(tab);
    if (itor->second.end() == tabItor) {
        tabItor = itor->second.insert({ tab, g_SQLCache->CreateTable(tab) }).first;
    }

    bool has = false;
    tabItor->second->Query(req.conditioncolumn(), req.conditionvalue(),
        [&](ISQLCacheRow* const row) {
            if (row) {
                has = true;
            }
        }
    );

    if (has) {
        ErrorLog(g_Engine, "OnCacheRequestCreate cache already exists, account %llu, table %s", account, GetTableTemplate(tab)->_Name.c_str());
        return;
    }

    ISQLCacheRow* row = tabItor->second->Insert(
        [&](ISQLCacheRow* const row) {
            for (int i = 0; i < req.values().size(); i++) {
                row->GetColumn(req.values(i).index())->SetValueString(req.values(i).value());
            }
        }
    );

    if (nullptr == row) {
        oCacheAnswer asw;
        asw.set_code(eFrameworkError::SQLCacheError);
        asw.set_account(account);
        asw.set_opt(req.opt());
        asw.set_tableindex(req.tableindex());
        asw.set_context(req.context());
        session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
        return;
    }

    std::string sql = "insert into `{TableName}`({Columns}) value({Values});";
    {
        sql = SafeString::Replace(sql, "{TableName}", GetTableTemplate(tab)->_Name);
        {
            std::string columnsString;
            for (int i = 0; i < req.values().size(); i++) {
                columnsString += "`" + GetTableTemplate(tab)->_ColumnTempates[req.values(i).index()]._Name + "`" + (i < req.values().size() - 1 ? ", " : "");
            }
            sql = SafeString::Replace(sql, "{Columns}", columnsString);
        }
        {
            std::string valuesString;
            for (int i = 0; i < req.values().size(); i++) {
                valuesString += "'" + req.values(i).value() + "'" + (i < req.values().size() - 1 ? ", " : "");
            }
            sql = SafeString::Replace(sql, "{Values}", valuesString);
        }
        sql = SafeString::Replace(sql, "{TableName}", GetTableTemplate(tab)->_Name);
    }

    TraceLog(g_Engine, "OnCacheRequestCreate Cache Succeed : %s", sql.c_str());
    g_DbGroup->MysqlQuery(account, sql,
        [=](const int code, const std::vector<std::vector<std::string>>* const result) {
            if (code != MYSQL_CODE_SUCCESS) {
                ErrorLog(g_Engine, "OnCacheRequestCreate mysql query error, sql %s", sql.c_str());
                if (FrameworkSession::Query(sessionId)) {
                    oCacheAnswer asw;
                    asw.set_code(eFrameworkError::MysqlError);
                    asw.set_account(account);
                    asw.set_opt(req.opt());
                    asw.set_tableindex(req.tableindex());
                    asw.set_context(req.context());
                    session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
                }
                return;
            }

            TraceLog(g_Engine, "OnCacheRequestCreate mysql query succeed, sql %s", sql.c_str());
            if (FrameworkSession::Query(sessionId)) {
                oCacheAnswer asw;
                asw.set_code(eFrameworkError::OK);
                asw.set_account(account);
                asw.set_opt(req.opt());
                asw.set_tableindex(req.tableindex());
                asw.set_context(req.context());
                session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
            }
        }
    );
}

__forceinline void OnCacheRequestUpdate(IFrameworkSession* const session, const UInt64 sessionId, const UInt64 account, const eTable tab, const oCacheRequest& req) {
    auto itor = s_AccountSQLCache.find(account);
    if (s_AccountSQLCache.end() == itor) {
        ErrorLog(g_Engine, "OnCacheRequestUpdate cache not exists, account %llu", account);
        return;
    }

    auto tableItor = itor->second.find(tab);
    if (itor->second.end() == tableItor) {
        ErrorLog(g_Engine, "OnCacheRequestUpdate cache not exists, account %llu, table %s", account, GetTableTemplate(tab)->_Name.c_str());
        return;
    }

    bool ret = true;
    tableItor->second->Query(req.conditioncolumn(), req.conditionvalue(),
        [&](ISQLCacheRow* const row) {
            for (int i = 0; i < req.values().size(); i++) {
                if (!row->GetColumn(req.values(i).index())->SetValueString(req.values(i).value())) {
                    ErrorLog(g_Engine, "OnCacheRequestUpdate Error, table %s column %s value %s",
                        GetTableTemplate(tab)->_Name.c_str(),
                        GetTableTemplate(tab)->_ColumnTempates[req.values(i).index()]._Name.c_str(),
                        req.values(i).value().c_str()
                    );
                    ret = false;
                }
            }
        }
    );

    if (false == ret) {
        if (FrameworkSession::Query(sessionId)) {
            oCacheAnswer asw;
            asw.set_code(eFrameworkError::SQLCacheError);
            asw.set_account(account);
            asw.set_opt(req.opt());
            asw.set_tableindex(req.tableindex());
            asw.set_context(req.context());
            session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
        }
        return;
    }

    std::string sql = "update `{TableName}` set {Values} where {Condition}";
    {
        sql = SafeString::Replace(sql, "{TableName}", GetTableTemplate(tab)->_Name);
        {
            std::string valuesString;
            for (int i = 0; i < req.values().size(); i++) {
                valuesString += "`" + GetTableTemplate(tab)->_ColumnTempates[req.values(i).index()]._Name + "` = '" + req.values(i).value() + (i < req.values().size() - 1 ? "', " : "'");
            }
            sql = SafeString::Replace(sql, "{Values}", valuesString);
        }
        {
            std::string conditionString = "`" + GetTableTemplate(tab)->_ColumnTempates[req.conditioncolumn()]._Name + "` = '" + req.conditionvalue() + "'";
            sql = SafeString::Replace(sql, "{Condition}", conditionString);
        }
    }
    TraceLog(g_Engine, "OnCacheRequestUpdate Cache succeed, sql %s", sql.c_str());
    g_DbGroup->MysqlQuery(account, sql,
        [=](const int code, const std::vector<std::vector<std::string>>* const result) {
            if (code != MYSQL_CODE_SUCCESS) {
                ErrorLog(g_Engine, "OnCacheRequestUpdate mysql query error, sql %s", sql.c_str());
                if (FrameworkSession::Query(sessionId)) {
                    oCacheAnswer asw;
                    asw.set_code(eFrameworkError::MysqlError);
                    asw.set_account(account);
                    asw.set_opt(req.opt());
                    asw.set_tableindex(req.tableindex());
                    asw.set_context(req.context());
                    session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
                }
                return;
            }

            TraceLog(g_Engine, "OnCacheRequestUpdate mysql query succeed, sql %s", sql.c_str());
            if (FrameworkSession::Query(sessionId)) {
                oCacheAnswer asw;
                asw.set_code(eFrameworkError::OK);
                asw.set_account(account);
                asw.set_opt(req.opt());
                asw.set_tableindex(req.tableindex());
                asw.set_context(req.context());
                session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
            }
        }
    );
}

__forceinline void OnCacheRequestRead(IFrameworkSession* const session, const UInt64 sessionId, const UInt64 account, const eTable tab, const oCacheRequest& req) {
    auto itor = s_AccountSQLCache.find(account);
    if (s_AccountSQLCache.end() == itor) {
        itor = s_AccountSQLCache.insert({ account, std::map<eTable, ISQLCacheTable*>() }).first;
    }

    auto tableItor = itor->second.find(tab);
    if (itor->second.end() != tableItor) {
        oCacheAnswer asw;
        asw.set_code(eFrameworkError::OK);
        asw.set_account(account);
        asw.set_opt(req.opt());
        asw.set_tableindex(req.tableindex());
        asw.set_context(req.context());
        tableItor->second->Query(req.conditioncolumn(), req.conditionvalue(),
            [&](ISQLCacheRow* const row) {
                oTableRow* r = asw.add_rows();
                for (int i = 0; i < GetTableTemplate(tab)->_ColumnCount; i++) {
                    oTableColumn* c = r->add_columns();
                    c->set_index(i);
                    c->set_value(row->GetColumn(i)->GetValueString());
                }
            }
        );

        session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
        return;
    }

    std::string sql = "select * from `{TableName}` where {Condition};";
    {
        sql = SafeString::Replace(sql, "{TableName}", GetTableTemplate(tab)->_Name);
        std::string conditionString = "`" + GetTableTemplate(tab)->_ColumnTempates[req.conditioncolumn()]._Name + "` = '" + req.conditionvalue() + "'";
        sql = SafeString::Replace(sql, "{Condition}", conditionString);
    }

    g_DbGroup->MysqlQuery(account, sql,
        [=](const int code, const std::vector<std::vector<std::string>>* const result) {
            if (code != MYSQL_CODE_SUCCESS) {
                if (FrameworkSession::Query(sessionId)) {
                    oCacheAnswer asw;
                    asw.set_code(eFrameworkError::MysqlError);
                    asw.set_context(req.context());
                    asw.set_account(account);
                    asw.set_opt(req.opt());
                    asw.set_tableindex(req.tableindex());

                    session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
                }
                ErrorLog(g_Engine, "OnCacheRequestRead Mysql Error %s", sql.c_str());
                return;
            }

            oCacheAnswer asw;
            asw.set_code(eFrameworkError::OK);
            asw.set_context(req.context());
            asw.set_account(account);
            asw.set_opt(req.opt());
            asw.set_tableindex(req.tableindex());

            ISQLCacheTable* t = g_SQLCache->CreateTable(tab);
            for (int row = 0; row < result->size(); row++) {
                oTableRow* pbr = asw.add_rows();
                t->Insert(
                    [&](ISQLCacheRow* const r) {
                        for (int column = 0; column < (*result)[row].size(); column++) {
                            r->GetColumn(column)->SetValueString((*result)[row][column]);
                            oTableColumn* pbc = pbr->add_columns();
                            pbc->set_index(column);
                            pbc->set_value((*result)[row][column]);
                        }
                    }
                );
            }

            s_AccountSQLCache[account].insert({ tab, t });

            if (FrameworkSession::Query(sessionId)) {
                session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
            }
            TraceLog(g_Engine, "OnCacheRequestRead Mysql Succeed %s", sql.c_str());
        }
    );
}

__forceinline void OnCacheRequestDelete(IFrameworkSession* const session, const UInt64 sessionId, const UInt64 account, const eTable tab, const oCacheRequest& req) {
    auto itor = s_AccountSQLCache.find(account);
    if (s_AccountSQLCache.end() == itor) {
        ErrorLog(g_Engine, "OnCacheRequestDelete cache not exists, account %llu", account);
        return;
    }

    auto tableItor = itor->second.find(tab);
    if (itor->second.end() == tableItor) {
        ErrorLog(g_Engine, "OnCacheRequestDelete cache not exists, account %llu, table %s", account, GetTableTemplate(tab)->_Name.c_str());
        return;
    }

    tableItor->second->Delete(req.conditioncolumn(), req.conditionvalue(),
        [&](ISQLCacheRow* const row) {
            TraceLog(g_Engine, "OnCacheRequestDelete succeed account %llu condition %s value %s",
                account, row->GetColumn(req.conditioncolumn())->GetColumnTemplate()->_Name.c_str(), row->GetColumn(req.conditioncolumn())->GetValueString().c_str());
        }
    );

    std::string sql = "delete from `{TableName}` where {Condition}";
    {
        sql = SafeString::Replace(sql, "{TableName}", tableItor->second->GetTemplate()->_Name);
        std::string conditionString = "`" + GetTableTemplate(tab)->_ColumnTempates[req.conditioncolumn()]._Name + "` = '" + req.conditionvalue() + "'";
        sql = SafeString::Replace(sql, "{Condition}", conditionString);
    }

    g_DbGroup->MysqlQuery(account, sql,
        [=](const int code, const std::vector<std::vector<std::string>>* const result) {
            if (code != MYSQL_CODE_SUCCESS) {
                if (FrameworkSession::Query(sessionId)) {
                    oCacheAnswer asw;
                    asw.set_code(eFrameworkError::MysqlError);
                    asw.set_context(req.context());
                    asw.set_account(account);
                    asw.set_opt(req.opt());
                    asw.set_tableindex(req.tableindex());
                    session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
                }
                ErrorLog(g_Engine, "OnCacheRequestDelete Mysql Error %s", sql.c_str());
                return;
            }
            else {
                if (FrameworkSession::Query(sessionId)) {
                    oCacheAnswer asw;
                    asw.set_code(eFrameworkError::OK);
                    asw.set_context(req.context());
                    asw.set_account(account);
                    asw.set_opt(req.opt());
                    asw.set_tableindex(req.tableindex());
                    session->SendProtobuf(eFrameworkID::CacheAnswer, asw);
                }
                TraceLog(g_Engine, "OnCacheRequestDelete Mysql Succeed %s", sql.c_str());
            }
        }
    );
}

void Cache::OnCacheRequest(IFrameworkSession* const session, const UInt64 sessionId, const oCacheRequest& req) {
    if (req.tableindex() < 0 || req.tableindex() >= (int)eTable::__Count__) {
        XASSERT(false, "wtf");
        ErrorLog(g_Engine, "OnCacheRequest Error table index %d", req.tableindex());
        return;
    }

    switch (req.opt()) {
    case eCacheOpt::Create:
        OnCacheRequestCreate(session, sessionId, req.account(), (eTable)req.tableindex(), req);
        break;
    case eCacheOpt::Update:
        OnCacheRequestUpdate(session, sessionId, req.account(), (eTable)req.tableindex(), req);
        break;
    case eCacheOpt::Read:
        OnCacheRequestRead(session, sessionId, req.account(), (eTable)req.tableindex(), req);
        break;
    case eCacheOpt::Delete:
        OnCacheRequestDelete(session, sessionId, req.account(), (eTable)req.tableindex(), req);
        break;
    case eCacheOpt::ReleaseCache: {
        auto itor = s_AccountSQLCache.find(req.account());
        if (s_AccountSQLCache.end() != itor) {
            for (auto i = itor->second.begin(); i != itor->second.end(); i++) {
                g_SQLCache->ReleaseTable(i->second);
            }
            s_AccountSQLCache.erase(itor);
            TraceLog(g_Engine, "OnCacheRequest eCacheOpt::ReleaseCache %llu", req.account());
        }
        break;
    }
    }
}
