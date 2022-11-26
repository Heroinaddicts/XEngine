#ifndef __Connecter_h__
#define __Connecter_h__ 

#include "Header.h"

class Connecter : public iConnecter, public Api::iTcpServer {
public:
    virtual ~Connecter() {}
    Connecter() {}

    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);

    virtual void RouteReport(const std::string& nodeName, const eRouteEvent ev, const unsigned_int64 guid) const;
    virtual void RouteReport(const unsigned_int32 nodeId, const eRouteEvent ev, const unsigned_int64 guid) const;
    virtual const RouteInfo* GetRouteInfo(const unsigned_int64 guid) const;

    virtual void RegisterRouteEvent(fRouteEvent const fun);

    virtual void RegisterSessionEvent(const eConnectionEvent& ev, fSessionEvent const fun);
    virtual void UnregisterSessionEvent(const eConnectionEvent& ev, fSessionEvent const fun);

    virtual iNodeSession* QueryNodeSession(const int nodeId) const;

    // Í¨¹ý iTcpServer ¼Ì³Ð
    virtual iTcpSession* OnMallocConnection(const char* remote_ip, const int remote_port) override;
    virtual void OnError(iTcpSession* session) override;
    virtual void OnRelease() override;
};

#endif //__Connecter_h__
