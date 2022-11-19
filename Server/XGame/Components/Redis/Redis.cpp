#include "Redis.h"

#ifdef WIN32
#pragma  comment(lib, "ws2_32.lib")
#endif //WIN32

iEngine* g_Engine = nullptr;
Redis* g_Redis = nullptr;

bool Redis::Initialize(iEngine* const engine) {
    g_Redis = this;
    g_Engine = engine;
    return true;
}

bool Redis::Launch(iEngine* const engine) {
    return true;
}

bool Redis::Destroy(iEngine* const engine) {
    return true;
}

void Redis::Update(iEngine* const engine) {
    unsigned_int64 tick = SafeSystem::Time::GetMilliSecond();
    RedisOpt opt;
    while (_ResponseQueue.Pull(opt)) {

    }
}

void Redis::Write(const std::string& key, const void* data, const int len, const unsigned_int64 context, const fRedisResult fun) {
    RedisOpt opt;
    opt._Type = eRedisOptType::Write;
    opt._Key = key;
}

void Redis::Read(const std::string& key, const unsigned_int64 context, const fRedisResult fun) {

}

void Redis::Run(void* constext) {
    while (true) {
        RedisOpt opt;
        if (_RequestQueue.Pull(opt)) {

        }
        else {
            SafeSystem::Time::MillisecondSleep(5);
        }
    }
}
