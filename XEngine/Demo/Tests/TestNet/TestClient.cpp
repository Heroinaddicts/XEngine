#include "TestClient.h"

int TestClient::OnReceive(const char* content, const int size) {
    //this->Send(content, size);
    _RecvSize += size;
    return size;
}

void TestClient::OnConnected() {
    g_ClientCount++;

    _SendSize = 0;
    _RecvSize = 0;
    _StartTick = SafeSystem::Time::GetMilliSecond();

    START_TIMER(g_Engine, this, TimeConfig::eTimeID::TraceBandwidth, 5 * SafeSystem::Time::Second, Api::Unlimited, 5 * SafeSystem::Time::Second, this);
    START_TIMER(g_Engine, this, TimeConfig::eTimeID::RandomSendData, 200, Api::Unlimited, 200, this);
    START_TIMER(g_Engine, this, TimeConfig::eTimeID::RandomClose, (SafeTools::Rand(4) + 1) * SafeSystem::Time::Minute, 1, 200, this);
}

void TestClient::OnDisconnect() {
    g_ClientCount--;
    g_Engine->GetTimerApi()->KillTimer(this, TimeConfig::eTimeID::TraceBandwidth, this);
    g_Engine->GetTimerApi()->KillTimer(this, TimeConfig::eTimeID::RandomSendData, this);
    g_Engine->GetTimerApi()->KillTimer(this, TimeConfig::eTimeID::RandomClose, this);
    xdel this;
}

void TestClient::OnConnectFailed() {
    xdel this;
}

void TestClient::OnStart(const int id, void* const context, const int64 timestamp) {
}

void TestClient::OnTimer(const int id, void* const context, const int64 timestamp) {
    switch (id) {
    case TimeConfig::eTimeID::TraceBandwidth: {
        float second = (SafeSystem::Time::GetMilliSecond() - _StartTick) / 1000.0f;
        TRACE(g_Engine, "Recv size %.2f KB, bandwidth %.2f KB/S, Send size %.2f KB, badwidth %.2f KB/S",
            _RecvSize / 1024.0f, (_RecvSize / 1024.0f) / second,
            _SendSize / 1024.0f, (_SendSize / 1024.0f) / second);
        break;
    }
    case TimeConfig::eTimeID::RandomSendData: {
        int len = SafeTools::Rand(512) + 64;
        const char* temp = (const char*)alloca(len);
        this->Send(temp, len);
        _SendSize += len;
        break;
    }
    case TimeConfig::eTimeID::RandomClose: {
        this->Close();
    }
    }
}

void TestClient::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {
}

void TestClient::OnPause(const int id, void* const context, const int64 timestamp) {
}

void TestClient::OnResume(const int id, void* const context, const int64 timestamp) {
}
