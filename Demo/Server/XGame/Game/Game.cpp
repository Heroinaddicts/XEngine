#include "Game.h"

Api::iEngine* g_Engine = nullptr;
IGameWorld* g_GameWorld = nullptr;
IServerManager* g_ServerManager = nullptr;

Api::iRedisSession* g_RedisSession = nullptr;

static Game* s_Self = nullptr;
XEventPool<IGame::eGameEvent, const UInt64> g_GameEventPool;
static std::unordered_set<UInt64> s_AccountSet;

static std::unordered_map<UInt16, IGame::fClientMessageCall> s_ClientMessagePool;

FrameworkSession* g_GateServerSession = nullptr;
FrameworkSession* g_CacheServerSession = nullptr;

enum eGameTimeID {
    ReleaseGateServerSession = 0
};

static const int s_ReconnectTimerDelay = 100;
static const int s_LogoutAccountDelay = 5 * SafeSystem::Time::Minute;

static int s_CacheServerPort = 0;

void Game::OnGateForwordClientMessageToGame(IFrameworkSession* const session, const UInt64 sessionId, const oGateForwordClientMessageToGame& pb) {
    //DEBUG(g_Engine, "Game::OnGateForwordClientMessageToGame account %llu messageId %u", pb.account(), pb.messageid());
    auto itor = s_ClientMessagePool.find((UInt16)pb.messageid());
    if (s_ClientMessagePool.end() != itor) {
        itor->second(pb.account(), pb.body().c_str(), pb.body().size());
    }
}

bool Game::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    IFrameworkSession::SetEngine(g_Engine);
    s_Self = this;

    return true;
}

bool Game::Launch(Api::iEngine* const engine) {
    UInt16 portForGate = engine->GetLaunchParameterUInt16("PortForGate");
    engine->GetNetApi()->LaunchTcpServer(this, "0.0.0.0", portForGate, 20 * SafeSystem::Network::MB);
    FrameworkSession::RegistServerProtobuf(eFrameworkID::GateNotifyGameAccountEvent, Game::OnGateNotifyGameAccountEvent);
    FrameworkSession::RegistServerProtobuf(eFrameworkID::GateForwordClientMessageToGame, Game::OnGateForwordClientMessageToGame);

    const std::string redishost = g_Engine->GetLaunchParameter("RedisHost");
    const UInt16 redisport = g_Engine->GetLaunchParameterUInt16("RedisPort");
    const std::string redispassword = g_Engine->GetLaunchParameter("RedisPassword") ? g_Engine->GetLaunchParameter("RedisPassword") : "";

    engine->GetRedisApi()->ConnectRedis(redishost, redisport, redispassword,
        [=](Api::iRedisSession* const redis) {
            if (redis) {
                TraceLog(g_Engine, "Connect redis succeed");
                g_RedisSession = redis;
            }
            else {
                XASSERT(false, "wtf");
                ErrorLog(g_Engine, "Connect redis %s:%d error", redishost.c_str(), redisport);
            }
        }
    );

    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_ServerManager, ServerManager);

    g_GameWorld->SetGuidAlloca(
        []() {
            return g_ServerManager->GeneraterGUID();
        }
    );

    s_AccountSet.reserve(4096);
    return true;
}

bool Game::Destroy(Api::iEngine* const engine) {
    return true;
}

void Game::LogoutAccount(const UInt64 account) {
}

void Game::RegistClientMessage(const UInt16 messageId, const fClientMessageCall call) {
    s_ClientMessagePool.insert({ messageId, call });
}

void Game::OnGateNotifyGameAccountEvent(IFrameworkSession* const session, const UInt64 sessionId, const oGateNotifyGameAccountEvent& ev) {
    switch (ev.event()) {
    case eAccountEvent::Login: {
        XASSERT(s_AccountSet.find(ev.account()) == s_AccountSet.end(), "wtf");
        s_AccountSet.insert(ev.account());
        g_GameEventPool.Trigger(eGameEvent::AccountConnect, ev.account());
        TraceLog(g_Engine, "eGameEvent::AccountConnect %llu", ev.account());
        break;
    }
    case eAccountEvent::Disconnect:
    case eAccountEvent::Logout: {
        if (s_AccountSet.find(ev.account()) != s_AccountSet.end()) {
            s_AccountSet.erase(ev.account());
            g_GameEventPool.Trigger(eGameEvent::AccountDisconnect, ev.account());
            TraceLog(g_Engine, "eGameEvent::AccountDisconnect %llu", ev.account());
        }
        break;
    }
    }
}

FrameworkSession* Game::CreateGateServerSession() {
    FrameworkSession* session = FrameworkSession::Create(eFrameworkSessionType::Gate);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Connected, Game::OnGateServerSessionConnected);
    session->RegistTcpSessionEventCallback<FrameworkSession>(Api::eTcpSessionEvent::Disconnected, Game::OnGateServerSessionDisconnected);
    return session;
}

void Game::ReleaseGateServerSession(FrameworkSession* session) {
    START_TIMER(g_Engine, s_Self, eGameTimeID::ReleaseGateServerSession, 10, 1, 10, session);
}

void Game::OnGateServerSessionConnected(FrameworkSession* const session) {
    switch (session->_Type) {
    case eFrameworkSessionType::Gate: {
        XASSERT(g_GateServerSession == nullptr, "wtf");
        if (g_GateServerSession != nullptr) {
            g_GateServerSession->Close();
        }

        TraceLog(g_Engine, "OnGateServerSessionConnected");
        g_GateServerSession = session;

        for (auto itor = s_AccountSet.begin(); itor != s_AccountSet.end(); itor++) {
            g_GameEventPool.Trigger(eGameEvent::AccountDisconnect, *itor);
        }
        s_AccountSet.clear();
        break;
    }
    default:
        XASSERT(false, "wtf");
        break;
    }
}

void Game::OnGateServerSessionDisconnected(FrameworkSession* const session) {
    switch (session->_Type) {
    case eFrameworkSessionType::Gate: {
        Game::ReleaseGateServerSession(session);
        g_GateServerSession = nullptr;
        TraceLog(g_Engine, "OnGateServerSessionDisconnected");

        for (auto itor = s_AccountSet.begin(); itor != s_AccountSet.end(); itor++) {
            g_GameEventPool.Trigger(eGameEvent::AccountDisconnect, *itor);
        }
        s_AccountSet.clear();
        break;
    }
    default:
        XASSERT(false, "wtf");
        break;
    }
}

void Game::RegistGameEvent(const eGameEvent ev, const fGameEvent callback) {
    XEventPoolRegist(g_GameEventPool, ev, callback);
}

void Game::SendMessageToClient(const UInt64 account, const UInt16 messageId, const void* const data, const int size) const {
    if (s_AccountSet.find(account) == s_AccountSet.end()) {
        return;
    }

    if (g_GateServerSession) {
        oGameSendMessageToClient body;
        body.add_accounts(account);
        body.set_messageid(messageId);
        body.set_body(data, size);

        g_GateServerSession->SendProtobuf(eFrameworkID::GameSendMessageToClient, body);
    }
    else {
        ErrorLog(g_Engine, "Gate Server Session does not exists");
    }
}

void Game::Broadcast(const std::vector<UInt64>& list, const UInt16 messageId, const void* const data, const int size) const {
    if (g_GateServerSession) {
        oGameSendMessageToClient body;
        for (int i = 0; i < list.size(); i++) {
            if (s_AccountSet.find(list[i]) == s_AccountSet.end()) {
                continue;
            }
            body.add_accounts(list[i]);
        }

        if (body.accounts_size() > 0) {
            body.set_messageid(messageId);
            body.set_body(data, size);
            g_GateServerSession->SendProtobuf(eFrameworkID::GameSendMessageToClient, body);
        }
    }
    else {
        ErrorLog(g_Engine, "Gate Server Session does not exists");
    }
}

void Game::PickName(const std::string& name, const fSetNameCallback& callback) {
    g_RedisSession->AsyncSetWhenNotExists(RedisDef::GenerateRedisKey(RedisDef::Nickname, name.c_str()), name,
        [=](const bool succeed) {
            TraceLog(g_Engine, "Pickname %s %s", name.c_str(), succeed ? "succeed" : "falid");
            callback(succeed);
        }
    );
}

Api::iTcpSession* Game::OnMallocTcpSession(const std::string& remoteIp, const unsigned short remotePort) {
    return Game::CreateGateServerSession();
}

void Game::OnError(Api::iTcpSession* session) {

}

void Game::OnRelease() {

}

void Game::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eGameTimeID::ReleaseGateServerSession: {
        FrameworkSession::Release((FrameworkSession*)context);
        break;
    }
    }
}

