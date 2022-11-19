#include "Gate.h"

iEngine* g_Engine = nullptr;
Gate* g_Gate = nullptr;
iConnecter* g_Connecter = nullptr;

iNodeSession* Gate::s_LobbyNodeSession = nullptr;

bool Gate::Initialize(iEngine* const engine) {
    g_Gate = this;
    g_Engine = engine;
    return true;
}

bool Gate::Launch(iEngine* const engine) {
    g_Connecter = engine->GetModule<iConnecter>("Connecter");
    XASSERT(g_Connecter, "where is module Connecter");

    g_Connecter->RegisterSessionEvent(iConnecter::eConnectionEvent::NodeAppear, Gate::OnNodeSessionAppear);
    g_Connecter->RegisterSessionEvent(iConnecter::eConnectionEvent::NodeDisappear, Gate::OnNodeSessionDisappear);
    return true;
}

bool Gate::Destroy(iEngine* const engine) {
    return true;
}

void Gate::Update(iEngine* const engine) {
    TRACE(g_Engine, "Gate::Update");
}

void Gate::FixedUpdate(iEngine* const engine) {
    TRACE(g_Engine, "Gate::FixedUpdate");
}

void Gate::OnNodeSessionAppear(iNodeSession* const session) {
    if (session->GetName() == NodeDefine::LobbyName) {
        XASSERT(s_LobbyNodeSession == nullptr, "Lobby node session already exists");
        if (nullptr == s_LobbyNodeSession) {
            s_LobbyNodeSession = session;
            TRACE(g_Engine, "Connect Lobby is successed");
        }
        else {
            ERROR(g_Engine, "Check your module Connecter's logic");
        }
    }
}

void Gate::OnNodeSessionDisappear(iNodeSession* const session) {
    if (session->GetName() == NodeDefine::LobbyName) {
        XASSERT(s_LobbyNodeSession != nullptr, "wtf");
        if (s_LobbyNodeSession) {
            s_LobbyNodeSession = nullptr;
            ERROR(g_Engine, "Lobby Lose");
        }
        else {
            ERROR(g_Engine, "Check your module Connecter's logic");
        }

        // to do 这里需要补充 给所有客户端玩家发送系统错误信息 并且主动断开所有客户端逻辑
    }
}
