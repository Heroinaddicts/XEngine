#ifndef __Sender_h__
#define __Sender_h__

#include "Header.h"

class SenderSession : public Api::iTcpSession {
public:
    int OnReceive(const void* const content, const int size) override { return size; }

private:
    void OnConnected() override;
    void OnConnectFailed() override;
    void OnDisconnected() override;
};

class Sender : public ISender, public Api::iTimer {
public:
    virtual ~Sender() {}

private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;
};

#endif //__Sender_h__
