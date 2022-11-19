#include "Balance.h"

iEngine* g_Engine = nullptr;
Balance* g_Balance = nullptr;
iConnecter* g_Connecter = nullptr;

int g_SelectIndexGateList = 0;
std::vector<GateInfo> g_GateList;

bool Balance::Initialize(iEngine* const engine) {
    g_Balance = this;
    g_Engine = engine;
    return true;
}

bool Balance::Launch(iEngine* const engine) {
    g_Connecter = engine->GetModule<iConnecter>("Connecter");
    XASSERT(g_Connecter, "where is Connecter");

    g_Connecter->RegisterSessionEvent(iConnecter::eConnectionEvent::NodeAppear, Balance::OnNodeSessionAppear);
    g_Connecter->RegisterSessionEvent(iConnecter::eConnectionEvent::NodeDisappear, Balance::OnNodeSessionDisappear);
    return true;
}

bool Balance::Destroy(iEngine* const engine) {
    return true;
}

void Balance::OnNodeSessionAppear(iNodeSession* const session) {
    XASSERT(session->GetName() == NodeDefine::GateName, "Error Session %s", session->GetName().c_str());
    if (session->GetName() == NodeDefine::GateName) {
        TRACE(g_Engine, "Gate %d connected", session->GetId());
        g_GateList.push_back(GateInfo(session));
    }
    else {
        ERROR(g_Engine, "Error Session %s connected", session->GetName().c_str());
        session->Close();
    }
}

void Balance::OnNodeSessionDisappear(iNodeSession* const session) {
    if (session->GetName() == NodeDefine::GateName) {
        for (int i = 0; i < g_GateList.size(); i++) {
            if (g_GateList[i]._Session == session) {
                ERROR(g_Engine, "Gate %d disconnected", session->GetId());
                if (g_SelectIndexGateList > i) { //这里如果游标如果在当前节点右边 那就要往左移一位 
                    g_SelectIndexGateList--;
                }
                g_GateList.erase(g_GateList.begin() + i);
                return;
            }
        }

        ERROR(g_Engine, "Check your module Connecter's logic");
    }
}
