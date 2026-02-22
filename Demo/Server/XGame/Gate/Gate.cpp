#include "Gate.h"
#include "Client.h"

Api::iEngine* g_Engine = nullptr;
IDb* g_Db = nullptr;
IDbGroup* g_DbGroup = nullptr;
static Gate* s_Self = nullptr;
XEventPool<IGate::eGateEvents, IClient* const> g_GateEventsPool;
std::map<UInt16, IGate::fClientMessager> g_ClientMessagePool;

UInt64 g_ClientSessionCount = 0;

FrameworkSession* g_GameServerSession = nullptr;
static UInt16 s_GameServerPort = 0;
enum eGateTimeID {
    ReconnectGameServer = 0,
    ReleaseGameServerSession = 1,
    PrintClientSessionCount = 2
};

static const int s_ReconnectTimerDelay = 100;

bool Gate::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    IFrameworkSession::SetEngine(g_Engine);
    s_Self = this;

    UInt16 portForClient = engine->GetLaunchParameterUInt16("PortForClient");
    engine->GetNetApi()->LaunchTcpServer(this, "0.0.0.0", portForClient, 16 * SafeSystem::Network::KB);

    s_GameServerPort = engine->GetLaunchParameterUInt16("GameServerPort");
    START_TIMER(g_Engine, s_Self, eGateTimeID::ReconnectGameServer, s_ReconnectTimerDelay, 1, s_ReconnectTimerDelay, 0);
    START_TIMER(g_Engine, s_Self, eGateTimeID::PrintClientSessionCount, 1000, Api::Unlimited, 1000, 0);
    return true;
}

__forceinline void OnClientLogined(IClient* const client) {
    oGateNotifyGameAccountEvent ev;
    ev.set_event(eAccountEvent::Login);
    ev.set_account(client->Account());
    g_GameServerSession->SendProtobuf(eFrameworkID::GateNotifyGameAccountEvent, ev);
}

__forceinline void OnClientDisconneted(IClient* const client) {
    if (client->Account() == INVALID_ACCOUNT) {
        return;
    }

    oGateNotifyGameAccountEvent ev;
    ev.set_event(eAccountEvent::Disconnect);
    ev.set_account(client->Account());
    g_GameServerSession->SendProtobuf(eFrameworkID::GateNotifyGameAccountEvent, ev);
}

__forceinline void OnClientLogout(IClient* const client) {
    oGateNotifyGameAccountEvent ev;
    ev.set_event(eAccountEvent::Logout);
    ev.set_account(client->Account());
    g_GameServerSession->SendProtobuf(eFrameworkID::GateNotifyGameAccountEvent, ev);
}

bool Gate::Launch(Api::iEngine* const engine) {
    std::string mysqlhost = g_Engine->GetLaunchParameter("MysqlHost");
    const int mysqlport = g_Engine->GetLaunchParameterInt32("MysqlPort");
    const std::string mysqldb = g_Engine->GetLaunchParameter("MysqlDb");
    const std::string mysqlusername = g_Engine->GetLaunchParameter("MysqlUsername");
    const std::string mysqlpassword = g_Engine->GetLaunchParameter("MysqlPassword");
    const std::string redishost = g_Engine->GetLaunchParameter("RedisHost");
    const int redisport = g_Engine->GetLaunchParameterInt32("RedisPort");
    const std::string redispassword = g_Engine->GetLaunchParameter("RedisPassword") ? g_Engine->GetLaunchParameter("RedisPassword") : "";

    const int dbGroupCount = g_Engine->GetLaunchParameterInt32("DbGroupCount");

    GetModule(g_Engine, g_Db, Db);
    g_DbGroup = g_Db->CreateGroup(dbGroupCount, mysqlhost, mysqlport, mysqldb, mysqlusername, mysqlpassword, redishost, redisport, redispassword);

    RegistGateEvent(eGateEvents::ClientLogind, OnClientLogined);
    RegistGateEvent(eGateEvents::ClientDsiconnected, OnClientDisconneted);
    RegistGateEvent(eGateEvents::ClientLogout, OnClientLogout);

    FrameworkSession::RegistServerProtobuf(eFrameworkID::GameSendMessageToClient, Gate::OnGameSendMessageToClient);
    return true;
}

bool Gate::Destroy(Api::iEngine* const engine) {
    return true;
}

FrameworkSession* Gate::CreateGameServerSession() {
    FrameworkSession* session = FrameworkSession::Create(eFrameworkSessionType::Game);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Connectfaild, Gate::OnGameServerConnectFaild);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Connected, Gate::OnGameServerConnected);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Disconnected, Gate::OnGameServerDisconnected);
    return session;
}

void Gate::ReleaseGameServerSession(FrameworkSession* session) {
    START_TIMER(g_Engine, s_Self, eGateTimeID::ReleaseGameServerSession, 10, 1, 10, session);
}

void Gate::OnGameServerConnectFaild(FrameworkSession* const session) {
    TraceLog(g_Engine, "OnGameServerConnectFaild");
    Gate::ReleaseGameServerSession(session);
    START_TIMER(g_Engine, s_Self, eGateTimeID::ReconnectGameServer, s_ReconnectTimerDelay, 1, s_ReconnectTimerDelay, 0);
}

void Gate::OnGameServerConnected(FrameworkSession* const session) {
    if (g_GameServerSession != nullptr) {
        ErrorLog(g_Engine, "OnGameServerConnected double connected");
        auto temp = g_GameServerSession;
        g_GameServerSession = nullptr;
        temp->Close();
    }

    TraceLog(g_Engine, "OnGameServerConnected");
    g_GameServerSession = session;
}

void Gate::OnGameServerDisconnected(FrameworkSession* const session) {
    if (g_GameServerSession == session) {
        TraceLog(g_Engine, "OnGameServerDisconnected double connected");
        START_TIMER(g_Engine, s_Self, eGateTimeID::ReconnectGameServer, s_ReconnectTimerDelay, 1, s_ReconnectTimerDelay, 0);
        g_GameServerSession = nullptr;
    }
    Gate::ReleaseGameServerSession(session);

    Client::ReleaseAll();
}

void Gate::OnGameSendMessageToClient(IFrameworkSession* const session, const UInt64 sessionId, const oGameSendMessageToClient& body) {
    for (int i = 0; i < body.accounts_size(); i++) {
        //DEBUG(g_Engine, "Game::OnGameSendMessageToClient account %llu messageId %u", body.accounts(i), body.messageid());
        Client* client = Client::QueryByAccount(body.accounts(i));
        if (client) {
            client->SendMessage((UInt16)body.messageid(), body.body().c_str(), body.body().length());
        }
    }
}

void Gate::RegistGateEvent(const eGateEvents ev, const fGateEvent callback) {
    XEventPoolRegist(g_GateEventsPool, ev, callback);
}

IClient* Gate::QueryClient(const UInt64 clientId) const {
    return Client::Query(clientId);
}

void Gate::RegistClientMessage(const UInt16 messageId, const fClientMessager callback) {
    XASSERT(g_ClientMessagePool.find(messageId) == g_ClientMessagePool.end(), "wtf");
    g_ClientMessagePool.insert({ messageId, callback });
}

Api::iTcpSession* Gate::OnMallocTcpSession(const std::string& remoteIp, const UInt16 remotePort) {
    return Client::Create();
}

void Gate::OnError(Api::iTcpSession* session) {

}

void Gate::OnRelease() {

}

void Gate::OnStart(const int id, const UInt64 context, const Int64 timestamp) {

}

void Gate::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eGateTimeID::ReconnectGameServer: {
        FrameworkSession* session = Gate::CreateGameServerSession();
        g_Engine->GetNetApi()->LaunchTcpSession(session, "127.0.0.1", s_GameServerPort, 20 * SafeSystem::Network::MB);
        break;
    }
    case eGateTimeID::ReleaseGameServerSession: {
        FrameworkSession::Release((FrameworkSession*)context);
        break;
    }
    case eGateTimeID::PrintClientSessionCount: {
        TraceLog(g_Engine, "ClientCount %llu", g_ClientSessionCount);
        break;
    }
    }
}

void Gate::OnEnd(const int id, const UInt64 context, bool nonviolent, const Int64 timestamp) {

}
