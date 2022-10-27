#include "Test001.h"
#include "iNavigationApi.h"

class TcpSession : public iTcpSession, public iTimer {
public:
    virtual ~TcpSession() {}

    // 通过 iTcpSession 继承
    virtual int OnReceive(const char* content, const int size) override {
        //Send(content, size);
        return size;
    }

    virtual void OnConnected() override {
        START_TIMER(g_engine, this, 0, 0, Unlimited, 16, 0);
    }

    virtual void OnDisconnect() override {
        g_engine->GetTimerApi()->KillTimer(this, 0, 0);
    }

    virtual void OnConnectFailed() override {
    }


    // 通过 iTimer 继承
    virtual void OnStart(const int id, void* const context, const int64 timestamp) override {
    }

    virtual void OnTimer(const int id, void* const context, const int64 timestamp) override {
        std::string data = XEngine::SafeString::Int64ToString(XEngine::SafeSystem::Time::GetMilliSecond()) + " ";
        for (int i = 0; i < 100; i++) {
            Send(data.c_str(), data.size());
        }
    }

    virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) override {
    }

    virtual void OnPause(const int id, void* const context, const int64 timestamp) override {
    }

    virtual void OnResume(const int id, void* const context, const int64 timestamp) override {
    }

};

iEngine* g_engine = nullptr;

bool Test001::Initialize(iEngine* const engine) {
    g_engine = engine;
    engine->GetNetApi()->LaunchTcpServer(this, "0.0.0.0", 12580, 1024, 1024);
    return true;
}

bool Test001::Launch(iEngine* const engine) {
    engine->GetNavigationApi()->LoadNavmeshAsync("D:/Svn/Demo/Server/cppserver/bin/windows/Debug/env/scene/Navmesh/LostCity.bin", [](iNavmesh* mesh) {
        XASSERT(mesh, "load mesh error");
        });
    return true;
}

bool Test001::Destroy(iEngine* const engine) {
    return true;
}

iTcpSession* Test001::OnMallocConnection(const char* remote_ip, const int remote_port) {
    return xnew TcpSession;
}

void Test001::OnError(iTcpSession* session) {
}

void Test001::OnRelease() {

}
