#ifndef __Forwarder_h__
#define __Forwarder_h__

#include "Header.h"

class ForwarderSenderSession : public Api::iTcpSession {
public:
    int OnReceive(const void* const content, const int size) override;

    static ForwarderSenderSession* Create();
    static void Release(ForwarderSenderSession* session);

private:
    friend XPool<ForwarderSenderSession>;
    ForwarderSenderSession() {}

    void OnConnected() override;
    void OnDisconnected() override;
};

class ForwarderReciverSession : public Api::iTcpSession {
public:
    int OnReceive(const void* const content, const int size) override { return size; }

private:
    void OnConnected() override;
    void OnConnectFailed() override;
    void OnDisconnected() override;
};

class Forwarder : public IForwarder, public Api::iTcpServer, public Api::iTimer {
public:
    virtual ~Forwarder() {}

private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    Api::iTcpSession* OnMallocTcpSession(const std::string& remoteIp, const UInt16 remotePort) override;
    void OnError(Api::iTcpSession* session) override;
    void OnRelease() override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;
};

#endif //__Forwarder_h__
