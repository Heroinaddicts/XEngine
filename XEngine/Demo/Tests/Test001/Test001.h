#ifndef __Test001_h__
#define __Test001_h__

#include "Header.h"

class Test001 : public iModule, public iTcpServer {
public:
    virtual ~Test001() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);

    // Í¨¹ý iTcpServer ¼Ì³Ð
    virtual iTcpSession* OnMallocConnection(const char* remote_ip, const int remote_port) override;
    virtual void OnError(iTcpSession* session) override;
    virtual void OnRelease() override;
};

#endif //__Test001_h__
