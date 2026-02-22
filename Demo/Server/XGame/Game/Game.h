#ifndef __Game_h__
#define __Game_h__

#include "Header.h"

class Game : public IGame, public Api::iTcpServer, Api::iTimer {
public:
    virtual ~Game() {}

private:
    // 通过 IGame 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    static FrameworkSession* CreateGateServerSession();
    static void ReleaseGateServerSession(FrameworkSession* session);
    // 通过 iTcpServer 继承
    Api::iTcpSession* OnMallocTcpSession(const std::string& remoteIp, const unsigned short remotePort) override;
    void OnError(Api::iTcpSession* session) override;
    void OnRelease() override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

    static void OnGateServerSessionConnected(FrameworkSession* const session);
    static void OnGateServerSessionDisconnected(FrameworkSession* const session);

    // 通过 IGame 继承
    void RegistGameEvent(const eGameEvent ev, const fGameEvent callback) override;
    void SendMessageToClient(const UInt64 account, const UInt16 messageId, const void* const data, const int size) const override;
    void Broadcast(const std::vector<UInt64>& list, const UInt16 messageId, const void* const data, const int size) const override;

    void PickName(const std::string& name, const fSetNameCallback& callback) override;
private:
    static void OnGateNotifyGameAccountEvent(IFrameworkSession* const session, const UInt64 sessionId, const oGateNotifyGameAccountEvent& ev);
    static void OnGateForwordClientMessageToGame(IFrameworkSession* const session, const UInt64 sessionId, const oGateForwordClientMessageToGame& pb);

    static void LogoutAccount(const UInt64 account);

    // 通过 IGame 继承
    void RegistClientMessage(const UInt16 messageId, const fClientMessageCall call) override;
};

#endif //__Game_h__
