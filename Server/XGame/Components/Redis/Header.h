#ifndef __Header_h__
#define __Header_h__

#include "iRedis.h"

#include "SafeSystem.h"
#include "SafeQueue.h"
#include "SafeThread.h"

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_Engine;
class Redis;
extern Redis* g_Redis;


enum class eRedisOptType {
    Unknown,
    Read,
    Write,
    Delete
};

struct RedisOpt {
    eRedisOptType _Type;
    std::string _Key;
    int _Id;
    unsigned_int64 _Context;
    void* _Data;
    int _Len;

    RedisOpt() : _Type(eRedisOptType::Unknown), _Key(""), _Id(0), _Context(0), _Data(nullptr), _Len(0) {}
};

#endif //__Header_h__
