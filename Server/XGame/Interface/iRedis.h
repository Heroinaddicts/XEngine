#ifndef __iRedis_h__
#define __iRedis_h__

#include "iModule.h"
using namespace XEngine;

typedef void(*fRedisResult)(const bool success, const std::string& key, const void* data, const int len, const unsigned_int64 context);

class iRedisClient {
public:
    virtual ~iRedisClient() {}

    virtual void Write(const std::string& key, const void* data, const int len, const unsigned_int64 context, const fRedisResult fun) = 0;
    virtual void Read(const std::string& key, const unsigned_int64 context, const fRedisResult fun) = 0;
    virtual void Delete(const std::string& key, const unsigned_int64 context, const fRedisResult fun) = 0;

    virtual void Close() = 0;
};

typedef void(*fConnectRedisResult)();
class iRedis : public Api::iModule {
public:
    virtual ~iRedis() {}

};

#endif //__iRedis_h__
