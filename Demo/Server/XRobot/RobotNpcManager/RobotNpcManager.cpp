#include "RobotNpcManager.h"
#include "RobotNpc.h"

Api::iEngine* g_Engine = nullptr;
IGameRobot* g_GameRobot = nullptr;
IConfig* g_Config = nullptr;

static RobotNpcManager* s_Self = nullptr;

static std::unordered_map<UInt64, std::unordered_map<Int64, IRobotNpc* const>> s_Npcs;

bool RobotNpcManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool RobotNpcManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameRobot, GameRobot);

    g_GameRobot->RegistServerProtobuf(eServerID::NpcInfoPush, OnNpcInfoPush);
    return true;
}

bool RobotNpcManager::Destroy(Api::iEngine* const engine) {
    return true;
}

const std::unordered_map<Int64, IRobotNpc* const>& RobotNpcManager::GetNpc(const UInt64 sceneId) const {
    return s_Npcs[sceneId];
}

void RobotNpcManager::OnNpcInfoPush(IRobotSession* session, const UInt64 sessionId, const oNpcInfoPush& push) {
    for (int i = 0; i < push.infos().size(); i++) {
        const AutoConfig::Struct::Npc* config = g_Config->GetAutoConfig()->QueryNpcConfig(push.infos(i).configid());

        auto itor = s_Npcs.find(push.infos(i).sceneid());
        if (s_Npcs.end() == itor) {
            itor = s_Npcs.insert({ push.infos(i).sceneid(), std::unordered_map<Int64, IRobotNpc* const>() }).first;
        }

        auto it = itor->second.find(config->ID);
        if (itor->second.end() == it) {
            itor->second.insert({ config->ID, txnew RobotNpc(config->ID, push.infos(i).sceneid(), Vector2(config->NpcPosition[0], config->NpcPosition[1])) });
            TraceLog(g_Engine, "RobotNpcManager::OnNpcInfoPush Scene %lld %s Position %f %f", push.infos(i).sceneid(), config->Name, config->NpcPosition[0], config->NpcPosition[1]);
        }
    }
}
