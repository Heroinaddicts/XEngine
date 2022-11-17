#ifndef __TestClient_h__
#define __TestClient_h__

#include "Header.h"

class TestClient : public Api::iTcpSession, public Api::iTimer {
public:
    virtual ~TestClient() {}

    // ͨ�� iTcpSession �̳�
    virtual int OnReceive(const char* content, const int size) override;
    virtual void OnConnected() override;
    virtual void OnDisconnect() override;
    virtual void OnConnectFailed() override;

    // ͨ�� iTimer �̳�
    virtual void OnStart(const int id, void* const context, const int64 timestamp) override;
    virtual void OnTimer(const int id, void* const context, const int64 timestamp) override;
    virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) override;
    virtual void OnPause(const int id, void* const context, const int64 timestamp) override;
    virtual void OnResume(const int id, void* const context, const int64 timestamp) override;

private:
    int _SendSize;
    int _RecvSize;
    int64 _StartTick;
};

#endif //__TestClient_h__
