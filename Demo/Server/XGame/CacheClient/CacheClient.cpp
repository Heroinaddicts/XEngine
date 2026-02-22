#include "CacheClient.h"

Api::iEngine* g_Engine = nullptr;
FrameworkSession* g_CacheServerSession = nullptr;
ISQLCache* g_SQLCache = nullptr;
static CacheClient* s_Self = nullptr;

static std::unordered_map<UInt64, std::map<UInt64, ICacheClient::fResult>> s_fResultMap;
static UInt64 s_fResultKeyAlloca = 0;

__forceinline UInt64 PushFResult(const UInt64 account, const ICacheClient::fResult& ret) {
    UInt64 key = s_fResultKeyAlloca++;
    s_fResultMap[account][key] = ret;
    return key;
}

__forceinline void Pull(const UInt64 account, const UInt64 key, const bool succeed, const ISQLCacheTable* const tab) {
    auto itor = s_fResultMap.find(account);
    if (s_fResultMap.end() != itor) {
        auto i = itor->second.find(key);
        XASSERT(itor->second.end() != i, "wtf");
        i->second(succeed, tab);
        itor->second.erase(i);
        if (itor->second.empty()) {
            s_fResultMap.erase(itor);
        }
    }
}

__forceinline void ClearFResult() {
    for (auto itor = s_fResultMap.begin(); itor != s_fResultMap.end(); itor++) {
        for (auto i = itor->second.begin(); i != itor->second.end(); i++) {
            i->second(false, nullptr);
        }
    }

    s_fResultMap.clear();
}

enum eCacheClientTimeID {
    ReconnectCacheServer = 0,
    ReleaseCacheServerSession = 1
};

static const int s_ReconnectTimerDelay = 100;
static UInt16 s_CachePort = 0;

bool CacheClient::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    IFrameworkSession::SetEngine(g_Engine);
    s_Self = this;
    return true;
}

bool CacheClient::Launch(Api::iEngine* const engine) {
    s_CachePort = engine->GetLaunchParameterUInt16("CachePort");
    START_TIMER(g_Engine, s_Self, eCacheClientTimeID::ReconnectCacheServer, s_ReconnectTimerDelay, 1, s_ReconnectTimerDelay, 0);
    GetModule(g_Engine, g_SQLCache, SQLCache);

    FrameworkSession::RegistServerProtobuf(eFrameworkID::CacheAnswer, OnCacheAnswer);
    return true;
}

bool CacheClient::Destroy(Api::iEngine* const engine) {
    return true;
}

void CacheClient::Create(const UInt64 account, const eTable tab, const fInitializeCache& init, const fResult& ret) {
    if (!g_CacheServerSession) {
        ErrorLog(g_Engine, "where is cache server");
        ret(false, nullptr);
        return;
    }

    ISQLCacheRow* temp = g_SQLCache->CreateTempRow(tab);
    init(temp);

    oCacheRequest req;
    req.set_account(account);
    req.set_opt(eCacheOpt::Create);
    req.set_tableindex((int)tab);
    req.set_context(PushFResult(account, ret));
    for (int i = 0; i < GetTableTemplate(tab)->_ColumnCount; i++) {
        oTableColumn* value = req.add_values();
        value->set_index(i);
        value->set_value(temp->GetColumn(i)->GetValueString());
    }

    g_SQLCache->ReleaseTempRow(temp);
    g_CacheServerSession->SendProtobuf(eFrameworkID::CacheRequest, req);
}

void CacheClient::Query(const UInt64 account, const eTable tab, const int conditionColumn, const std::string& conditionValue, const fResult& ret) {
    if (!g_CacheServerSession) {
        ErrorLog(g_Engine, "where is cache server");
        ret(false, nullptr);
        return;
    }

    oCacheRequest req;
    req.set_account(account);
    req.set_opt(eCacheOpt::Read);
    req.set_tableindex((int)tab);
    req.set_conditioncolumn(conditionColumn);
    req.set_conditionvalue(conditionValue);
    req.set_context(PushFResult(account, ret));
    g_CacheServerSession->SendProtobuf(eFrameworkID::CacheRequest, req);
}


void CacheClient::Update(const UInt64 account, const eTable tab, const int conditionColumn, const std::string& conditionValue, const fInitializeCache& init, const fResult& ret) {
    if (!g_CacheServerSession) {
        ErrorLog(g_Engine, "where is cache server");
        ret(false, nullptr);
        return;
    }

    ISQLCacheRow* temp = g_SQLCache->CreateTempRow(tab);
    init(temp);

    oCacheRequest req;
    req.set_account(account);
    req.set_opt(eCacheOpt::Update);
    req.set_tableindex((int)tab);
    req.set_context(PushFResult(account, ret));
    req.set_conditioncolumn(conditionColumn);
    req.set_conditionvalue(conditionValue);
    for (int i = 0; i < GetTableTemplate(tab)->_ColumnCount; i++) {
        if (temp->GetColumn(i)->HasValue()) {
            oTableColumn* value = req.add_values();
            value->set_index(i);
            value->set_value(temp->GetColumn(i)->GetValueString());
        }
    }

    g_SQLCache->ReleaseTempRow(temp);
    g_CacheServerSession->SendProtobuf(eFrameworkID::CacheRequest, req);
}

void CacheClient::Delete(const UInt64 account, const eTable tab, const int conditionColumn, const std::string& conditionValue, const fResult& ret) {
    if (!g_CacheServerSession) {
        ErrorLog(g_Engine, "where is cache server");
        ret(false, nullptr);
        return;
    }

    oCacheRequest req;
    req.set_account(account);
    req.set_opt(eCacheOpt::Delete);
    req.set_tableindex((int)tab);
    req.set_context(PushFResult(account, ret));
    req.set_conditioncolumn(conditionColumn);
    req.set_conditionvalue(conditionValue);
    g_CacheServerSession->SendProtobuf(eFrameworkID::CacheRequest, req);
}

void CacheClient::ReleaseCache(const UInt64 account) {
    s_fResultMap.erase(account);

    oCacheRequest req;
    req.set_account(account);
    req.set_opt(eCacheOpt::ReleaseCache);
    g_CacheServerSession->SendProtobuf(eFrameworkID::CacheRequest, req);
}

void CacheClient::OnCacheAnswer(IFrameworkSession* const session, const UInt64 sessionId, const oCacheAnswer& answer) {
    if (FrameworkSession::Query(sessionId)) {
        if (answer.code() != eFrameworkError::OK) {
            Pull(answer.account(), answer.context(), false, nullptr);
            return;
        }

        switch (answer.opt()) {
        case eCacheOpt::Create: {
            Pull(answer.account(), answer.context(), true, nullptr);
            break;
        }
        case eCacheOpt::Update: {
            Pull(answer.account(), answer.context(), true, nullptr);
            break;
        }
        case eCacheOpt::Read: {
            ISQLCacheTable* tab = g_SQLCache->CreateTable((eTable)answer.tableindex());
            for (int i = 0; i < answer.rows().size(); i++) {
                tab->Insert(
                    [&](ISQLCacheRow* const row) {
                        auto pbr = answer.rows(i);
                        for (int index = 0; index < pbr.columns_size(); index++) {
                            row->GetColumn(pbr.columns(index).index())->SetValueString(pbr.columns(index).value());
                        }
                    }
                );
            }
            Pull(answer.account(), answer.context(), true, tab);
            g_SQLCache->ReleaseTable(tab);
            break;
        }
        case eCacheOpt::Delete: {
            Pull(answer.account(), answer.context(), true, nullptr);
            break;
        }
        }
    }
}

void CacheClient::OnCacheServerConnected(FrameworkSession* const session) {
    XASSERT(g_CacheServerSession == nullptr, "wtf");
    if (g_CacheServerSession != nullptr) {
        g_CacheServerSession->Close();
        g_CacheServerSession = nullptr;
        ErrorLog(g_Engine, "Double OnCacheServerConnected");
    }

    g_CacheServerSession = session;
    TraceLog(g_Engine, "OnCacheServerConnected");
}

void CacheClient::OnCacheServerDisconnected(FrameworkSession* const session) {
    XASSERT(g_CacheServerSession == session, "wtf");
    if (g_CacheServerSession == session) {
        g_CacheServerSession = nullptr;
        TraceLog(g_Engine, "OnCacheServerDisconnected");
        START_TIMER(g_Engine, s_Self, eCacheClientTimeID::ReconnectCacheServer, s_ReconnectTimerDelay, 1, s_ReconnectTimerDelay, 0);
    }
    else {
        ErrorLog(g_Engine, "Double OnCacheServerDisconnected");
    }

    CacheClient::ReleaseCacheServerSession(session);
    ClearFResult();
}

void CacheClient::OnCacheServerConnectFaild(FrameworkSession* const session) {
    XASSERT(session->_Type == eFrameworkSessionType::Cache, "wtf");
    TraceLog(g_Engine, "OnFrameworkSessionConnectFaild Cache");
    START_TIMER(g_Engine, s_Self, eCacheClientTimeID::ReconnectCacheServer, s_ReconnectTimerDelay, 1, s_ReconnectTimerDelay, 0);
}

FrameworkSession* CacheClient::CreateCacheServerSession() {
    FrameworkSession* session = FrameworkSession::Create(eFrameworkSessionType::Cache);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Connected, CacheClient::OnCacheServerConnected);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Disconnected, CacheClient::OnCacheServerDisconnected);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Connectfaild, CacheClient::OnCacheServerConnectFaild);
    return session;
}

void CacheClient::ReleaseCacheServerSession(FrameworkSession* session) {
    START_TIMER(g_Engine, s_Self, eCacheClientTimeID::ReleaseCacheServerSession, 10, 1, 10, session);
}

void CacheClient::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eCacheClientTimeID::ReconnectCacheServer: {
        FrameworkSession* session = CacheClient::CreateCacheServerSession();
        g_Engine->GetNetApi()->LaunchTcpSession(session, "127.0.0.1", s_CachePort, 20 * SafeSystem::Network::MB);
        TraceLog(g_Engine, "ReconnectCacheServer");
        break;
    }
    case eCacheClientTimeID::ReleaseCacheServerSession: {
        FrameworkSession::Release((FrameworkSession*)context);
        break;
    }
    }
}
