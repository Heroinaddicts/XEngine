#include "TestRedis.h"
#include "TestRedisSession.h"

iEngine* g_Engine = nullptr;
TestRedis* g_TestRedis = nullptr;
iRedis* g_Redis = nullptr;

int g_RedisSessionID = 0;

bool TestRedis::Initialize(iEngine* const engine) {
    g_TestRedis = this;
    g_Engine = engine;
    return true;
}

bool TestRedis::Launch(iEngine* const engine) {
    g_Redis = engine->GetModule<iRedis>("Redis");
    XASSERT(g_Redis, "wtf");

    START_TIMER(g_Engine, this, 0, 2000, Api::Unlimited, 2000, 0);
    return true;
}

bool TestRedis::Destroy(iEngine* const engine) {
    return true;
}

void TestRedis::OnStart(const int id, void* const context, const int64 timestamp) {

}

void TestRedis::OnTimer(const int id, void* const context, const int64 timestamp) {
    for (int i = 0; i < 100; i++) {
        TestRedisSession* session = xnew TestRedisSession;
        g_Redis->ConnectRedis(session, "10.0.6.100", 6379, "123123");
    }
}

void TestRedis::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {

}

void TestRedis::OnPause(const int id, void* const context, const int64 timestamp) {

}

void TestRedis::OnResume(const int id, void* const context, const int64 timestamp) {

}
