#ifndef __Cache_h__
#define __Cache_h__

#include "Header.h"

class Cache : public ICache, public Api::iTcpServer, Api::iTimer {
public:
    virtual ~Cache() {}

private:
    // 通过 ICache 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    static FrameworkSession* CreateGameServerSession();
    static void ReleaseGameServerSession(FrameworkSession* session);
    // 通过 iTcpServer 继承
    Api::iTcpSession* OnMallocTcpSession(const std::string& remoteIp, const UInt16 remotePort) override;
    void OnError(Api::iTcpSession* session) override;
    void OnRelease() override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

    static void OnCacheClientSessionConnected(FrameworkSession* const session);
    static void OnCacheClientSessionDisconnected(FrameworkSession* const session);

    static void OnCacheRequest(IFrameworkSession* const session, const UInt64 sessionId, const oCacheRequest& req);
};

#endif //__Cache_h__
