#ifndef __DbGroup_h__
#define __DbGroup_h__

#include "Header.h"

class DbGroup : public IDbGroup {
public:
    virtual ~DbGroup() {}

    static DbGroup* Create(const int count,
        const std::string& mysqlhost, const int mysqlport, const std::string& mysqldb, const std::string& mysqlusername, const std::string& mysqlpassword,
        const std::string& redishost, const int redisport, const std::string& redispassword);
    static void Release(DbGroup*& group);

    // Í¨¹ý IDbGroup ¼Ì³Ð
    void MysqlQuery(const UInt64 mask, const std::string& sql, const Api::fQueryCallback& fun) override;

    void RedisAsyncGet(const UInt64 mask, const std::string& key, const Api::fStringResult& callback) override;
    void RedisAsyncSet(const UInt64 mask, const std::string& key, const std::string& value, const Api::fBoolResult& callback) override;

    void RedisAsyncSetWhenNotExists(const UInt64 mask, const std::string& key, const std::string& value, const Api::fBoolResult& callback) override;
    void RedisAsyncCAS(const UInt64 mask, const std::string& key, const std::string& compare, const std::string& value, const Api::fBoolResult& callback) override;
private:
    DbGroup(const int count,
        const std::string& mysqlhost, const int mysqlport, const std::string& mysqldb, const std::string& mysqlusername, const std::string& mysqlpassword,
        const std::string& redishost, const int redisport, const std::string& redispassword);
    friend XPool<DbGroup>;

    std::vector<Api::iMysqlSession*> _MysqlSessions;
    std::vector<Api::iRedisSession*> _RedisSessions;
};

#endif //__DbGroup_h__
