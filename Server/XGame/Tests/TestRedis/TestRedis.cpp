#include "TestRedis.h"
#include "TestRedisSession.h"

iEngine* g_Engine = nullptr;
TestRedis* g_TestRedis = nullptr;
iRedis* g_Redis = nullptr;

bool TestRedis::Initialize(iEngine* const engine) {
    g_TestRedis = this;
    g_Engine = engine;
    return true;
}

bool TestRedis::Launch(iEngine* const engine) {
    g_Redis = engine->GetModule<iRedis>("Redis");
    XASSERT(g_Redis, "wtf");

    TestRedisSession* session = xnew TestRedisSession;
    g_Redis->ConnectRedis(session, "10.0.6.100", 6379, "123123");

    return true;
}

bool TestRedis::Destroy(iEngine* const engine) {
    return true;
}
