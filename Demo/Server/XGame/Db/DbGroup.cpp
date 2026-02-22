#include "DbGroup.h"

static XPool<DbGroup> s_Pool;

DbGroup* DbGroup::Create(const int count,
    const std::string& mysqlhost, const int mysqlport, const std::string& mysqldb, const std::string& mysqlusername, const std::string& mysqlpassword,
    const std::string& redishost, const int redisport, const std::string& redispassword) {
    return XPOOL_CREATE(s_Pool, count, mysqlhost, mysqlport, mysqldb, mysqlusername, mysqlpassword, redishost, redisport, redispassword);
}

void DbGroup::Release(DbGroup*& group) {
    XPOOL_RELEASE(s_Pool, group);
    group = nullptr;
}

void DbGroup::MysqlQuery(const UInt64 mask, const std::string& sql, const Api::fQueryCallback& fun) {
    if (_MysqlSessions.size() == 0) {
        ErrorLog(g_Engine, "no mysql session for use");
        fun(-1, nullptr);
        return;
    }

    const int index = (int)(mask % _MysqlSessions.size());
    _MysqlSessions[index]->Query(sql, fun);
}

void DbGroup::RedisAsyncGet(const UInt64 mask, const std::string& key, const Api::fStringResult& callback) {
    if (_RedisSessions.size() == 0) {
        ErrorLog(g_Engine, "no redis session for use");
        callback(false, nullptr);
        return;
    }

    const int index = (int)(mask % _RedisSessions.size());
    _RedisSessions[index]->AsyncGet(key, callback);
}

void DbGroup::RedisAsyncSet(const UInt64 mask, const std::string& key, const std::string& value, const Api::fBoolResult& callback) {
    if (_RedisSessions.size() == 0) {
        ErrorLog(g_Engine, "no redis session for use");
        callback(false);
        return;
    }

    const int index = (int)(mask % _RedisSessions.size());
    _RedisSessions[index]->AsyncSet(key, value, callback);
}

void DbGroup::RedisAsyncSetWhenNotExists(const UInt64 mask, const std::string& key, const std::string& value, const Api::fBoolResult& callback) {
    if (_RedisSessions.size() == 0) {
        ErrorLog(g_Engine, "no redis session for use");
        callback(false);
        return;
    }

    const int index = (int)(mask % _RedisSessions.size());
    _RedisSessions[index]->AsyncSetWhenNotExists(key, value, callback);
}

void DbGroup::RedisAsyncCAS(const UInt64 mask, const std::string& key, const std::string& compare, const std::string& value, const Api::fBoolResult& callback) {
    if (_RedisSessions.size() == 0) {
        ErrorLog(g_Engine, "no redis session for use");
        callback(false);
        return;
    }

    const int index = (int)(mask % _RedisSessions.size());
    _RedisSessions[index]->AsyncCAS(key, compare, value, callback);
}

DbGroup::DbGroup(const int count, const std::string& mysqlhost, const int mysqlport, const std::string& mysqldb, const std::string& mysqlusername, const std::string& mysqlpassword, const std::string& redishost, const int redisport, const std::string& redispassword) {
    for (int i = 0; i < count; i++) {
        g_Engine->GetMysqlApi()->ConnectMysql(mysqlhost, mysqlport, mysqldb, mysqlusername, mysqlpassword,
            [=](Api::iMysqlSession* session) {
                if (session) {
                    TraceLog(g_Engine, "Connect mysql %s:%d succeed", mysqlhost.c_str(), mysqlport);
                    _MysqlSessions.push_back(session);
                }
                else {
                    ErrorLog(g_Engine, "Connect mysql %s:%d error", mysqlhost.c_str(), mysqlport);
                }
            }
        );

        g_Engine->GetRedisApi()->ConnectRedis(redishost, redisport, redispassword,
            [=](Api::iRedisSession* const session) {
                if (session) {
                    TraceLog(g_Engine, "Connect redis %s:%d succeed", redishost.c_str(), redisport);
                    _RedisSessions.push_back(session);
                }
                else {
                    ErrorLog(g_Engine, "Connect redis %s:%d error", redishost.c_str(), redisport);
                }
            }
        );
    }
}

