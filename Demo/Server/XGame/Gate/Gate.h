#ifndef __Gate_h__
#define __Gate_h__

#include "Header.h"

class Gate : public IGate, public Api::iTcpServer, Api::iTimer {
public:
    virtual ~Gate() {}
    // 通过 IGate 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    // 通过 IGate 继承
    IClient* QueryClient(const UInt64 clientId) const override;
    void RegistGateEvent(const eGateEvents ev, const fGateEvent callback) override;
    void RegistClientMessage(const UInt16 messageId, const fClientMessager callback) override;

private:

    // 通过 iTcpServer 继承
    Api::iTcpSession* OnMallocTcpSession(const std::string& remoteIp, const UInt16 remotePort) override;
    void OnError(Api::iTcpSession* session) override;
    void OnRelease() override;

    static FrameworkSession* CreateGameServerSession();
    static void ReleaseGameServerSession(FrameworkSession* session);

    void OnStart(const int id, const UInt64 context, const Int64 timestamp) override;
    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;
    void OnEnd(const int id, const UInt64 context, bool nonviolent, const Int64 timestamp) override;

    static void OnGameServerConnectFaild(FrameworkSession* const session);
    static void OnGameServerConnected(FrameworkSession* const session);
    static void OnGameServerDisconnected(FrameworkSession* const session);

    static void OnGameSendMessageToClient(IFrameworkSession* const session, const UInt64 sessionId, const oGameSendMessageToClient& body);
};

#endif //__Gate_h__
