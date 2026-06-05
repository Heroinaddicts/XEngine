#ifndef __Reciver_h__
#define __Reciver_h__

#include "Header.h"

class ReciverSession : public Api::iTcpSession {
public:
    int OnReceive(const void* const content, const int size) override;

    static ReciverSession* Create();
    static void Release(ReciverSession* session);

private:
    friend XPool<ReciverSession>;
    ReciverSession() {}

    void OnConnected() override;
    void OnDisconnected() override;
};

class Reciver : public IReciver, public Api::iTcpServer, public Api::iTimer {
public:
    virtual ~Reciver() {}

private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    Api::iTcpSession* OnMallocTcpSession(const std::string& remoteIp, const UInt16 remotePort) override;
    void OnError(Api::iTcpSession* session) override;
    void OnRelease() override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;
};

#endif //__Reciver_h__
