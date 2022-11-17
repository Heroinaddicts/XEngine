#ifndef __Shutdown_h__
#define __Shutdown_h__

#include "Header.h"

class Shutdown : public Api::iTcpServer {
public:
    virtual ~Shutdown() {}

    // Í¨¹ý iTcpServer ¼Ì³Ð
    virtual iTcpSession* OnMallocConnection(const char* remote_ip, const int remote_port) override;
    virtual void OnError(iTcpSession* session) override;
    virtual void OnRelease() override;
};

#endif //__Shutdown_h__