#ifndef __IDb_h__
#define __IDb_h__

#include "iModule.h"
#include "iMysqlApi.h"
#include "iRedisApi.h"
UsingXEngine;

class IDbGroup {
public:
    virtual ~IDbGroup() {}

    virtual void MysqlQuery(const UInt64 mask, const std::string& sql, const Api::fQueryCallback& fun) = 0;

    virtual void RedisAsyncGet(const UInt64 mask, const std::string& key, const Api::fStringResult& callback) = 0;
    virtual void RedisAsyncSet(const UInt64 mask, const std::string& key, const std::string& value, const Api::fBoolResult& callback) = 0;

    virtual void RedisAsyncSetWhenNotExists(const UInt64 mask, const std::string& key, const std::string& value, const Api::fBoolResult& callback) = 0;
    virtual void RedisAsyncCAS(const UInt64 mask, const std::string& key, const std::string& compare, const std::string& value, const Api::fBoolResult& callback) = 0;
};

class IDb : public Api::iModule {
public:
    virtual ~IDb() {}

    virtual IDbGroup* CreateGroup(const int count,
        const std::string& mysqlhost, const int mysqlport, const std::string& mysqldb, const std::string& mysqlusername, const std::string& mysqlpassword,
        const std::string& redishost, const int redisport, const std::string& redispassword) = 0;
};

#endif //__IDb_h__
