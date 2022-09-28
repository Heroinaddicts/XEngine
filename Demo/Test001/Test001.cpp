#include "Test001.h"

class TcpSession : public iTcpSession {
public:
    virtual ~TcpSession() {}



    // Í¨¹ý iTcpSession ¼Ì³Ð
    virtual int OnReceive(const char* content, const int size) override {
        Send(content, size);
        return size;
    }

    virtual void OnConnected() override {
    }

    virtual void OnDisconnect() override {
    }

    virtual void OnConnectFailed() override {
    }

};

bool Test001::Initialize(iEngine* const engine) {
    engine->GetNetApi()->LaunchTcpServer(this, "127.0.0.1", 12580, 1024, 1024);
    return true;
}

bool Test001::Launch(iEngine* const engine) {
    return true;
}

bool Test001::Destroy(iEngine* const engine) {
    return true;
}

iTcpSession* Test001::OnMallocConnection(const char* remote_ip, const int remote_port) {
    return x_new TcpSession;
}

void Test001::OnError(iTcpSession* session) {
}

void Test001::OnRelease() {

}
