#include "RobotAi.h"

Api::iEngine* g_Engine = nullptr;
IGameRobot* g_GameRobot = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
IRobotNpcManager* g_RobotNpcManager = nullptr;
IRobotSceneManager* g_RobotSceneManager = nullptr;
IRobotMovement* g_RobotMovement = nullptr;

static RobotAi* s_Self = nullptr;

bool RobotAi::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool RobotAi::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameRobot, GameRobot);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_RobotSceneManager, RobotSceneManager);
    GetModule(g_Engine, g_RobotMovement, RobotMovement);
    GetModule(g_Engine, g_RobotNpcManager, RobotNpcManager);

    g_GameRobot->RegistEvent(IGameRobot::eEvent::RobotCreated, OnRobotCreated);
    g_GameRobot->RegistEvent(IGameRobot::eEvent::RobotRelease, OnRobotRelease);
    return true;
}

bool RobotAi::Destroy(Api::iEngine* const engine) {
    return true;
}

void RobotAi::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    if (nullptr == g_RobotSceneManager->QueryCharacterScene(context)) {
        return;
    }

    const std::unordered_map<Int64, IRobotNpc* const>& npcs = g_RobotNpcManager->GetNpc(g_RobotSceneManager->QueryCharacterScene(context)->_SceneID);
    static int s_Index = 0;
    int index = s_Index++ % npcs.size();
    IRobotNpc* npc = nullptr;
    for (auto itor = npcs.begin(); itor != npcs.end(); itor++) {
        if (0 == index) {
            npc = itor->second;
            break;
        }

        index--;
    }
    XASSERT(npc, "wtf");

    g_RobotMovement->MoveTo(context, npc->Position(),
        [&](const UInt64 characterId, const Vector2& pos, const bool isArrive) {
            if (isArrive) {
                TraceLog(g_Engine, "Robot is arrived %s", isArrive ? "true" : "false");
            }
            START_TIMER(g_Engine, s_Self, 0, SafeTools::Rand(1000) + 1000, 1, 10000, context);
        }
    );
}

void RobotAi::OnRobotCreated(IRobotSession* const session) {
    START_TIMER(g_Engine, s_Self, 0, SafeTools::Rand(1000) + 1000, 1, 1000, session->DeployCharacterID());
}

void RobotAi::OnRobotRelease(IRobotSession* const session) {
    STOP_TIMER(g_Engine, s_Self, 0, session->DeployCharacterID());
}
