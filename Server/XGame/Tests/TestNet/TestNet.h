#ifndef __TestNet_h__
#define __TestNet_h__ 

#include "Header.h"

class TestNet : public iTestNet, public Api::iTcpServer, public Api::iTimer {
public:
    virtual ~TestNet() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);

    // 通过 iTcpServer 继承
    virtual iTcpSession* OnMallocConnection(const char* remote_ip, const int remote_port) override;
    virtual void OnError(iTcpSession* session) override;
    virtual void OnRelease() override;

    // 通过 iTimer 继承
    virtual void OnStart(const int id, void* const context, const int64 timestamp) override;
    virtual void OnTimer(const int id, void* const context, const int64 timestamp) override;
    virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) override;
    virtual void OnPause(const int id, void* const context, const int64 timestamp) override;
    virtual void OnResume(const int id, void* const context, const int64 timestamp) override;
};

#endif //__TestNet_h__
