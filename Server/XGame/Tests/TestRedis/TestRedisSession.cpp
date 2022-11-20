#include "TestRedisSession.h"

void TestRedisSession::OnConnect(bool success) {
    if (success) {
        TRACE(g_Engine, "TestRedisSession OnConnected");
        START_TIMER(g_Engine, this, 1, 100, Api::Unlimited, 100, 0);
    }
    else {
        TRACE(g_Engine, "TestRedisSession OnConnect failed, retry");
        START_TIMER(g_Engine, this, 0, 100, 1, 100, 0);
    }
}

void TestRedisSession::OnDisconnect() {
    TRACE(g_Engine, "TestRedisSession OnDisconnect");
    g_Engine->GetTimerApi()->KillTimer(this, 1, 0);

    START_TIMER(g_Engine, this, 0, 100, 1, 100, 0);
}
void TestRedisSession::OnWriteResponse(bool success, const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context) {
    std::string dataString((char*)data, len);
    if (success) {
        TRACE(g_Engine, "TestRedisSession OnWriteResponse %s", dataString.c_str());
    }
    else {
        TRACE(g_Engine, "TestRedisSession OnWriteResponse false");
    }
}

void TestRedisSession::OnReadResponse(bool success, const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context) {

}

void TestRedisSession::OnDeleteResponse(bool success, const std::string& key, const int logicId, const unsigned_int64 context) {

}

void TestRedisSession::OnStart(const int id, void* const context, const int64 timestamp) {

}

void TestRedisSession::OnTimer(const int id, void* const context, const int64 timestamp) {
    if (id == 1) {
        for (int i = 0; i < 100; i++) {
            std::string key = "max";
            key += SafeString::Int64ToString(i);

            std::string data = "hello redis, I'm doing test for you!!!!";
            data += SafeString::Int64ToString(i);

            Write(key, 1, data.c_str(), data.size(), 0);
        }
    }

    if (id == 0) {
        g_Redis->ConnectRedis(this, "10.0.6.100", 6379, "123123");
    }
}

void TestRedisSession::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {

}

void TestRedisSession::OnPause(const int id, void* const context, const int64 timestamp) {

}

void TestRedisSession::OnResume(const int id, void* const context, const int64 timestamp) {

}

