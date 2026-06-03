#include "RobotSceneManager.h"
#include "RobotScene.h"

Api::iEngine* g_Engine = nullptr;
ITilemapManager* g_TilemapManager = nullptr;
IConfig* g_Config = nullptr;
IGameRobot* g_GameRobot = nullptr;
IGameWorld* g_GameWorld = nullptr;

static RobotSceneManager* s_Self = nullptr;
std::map<Int64, IIsometricTilemap*> g_Tilemaps;

static std::map<UInt64, RobotScene* > s_SceneMap;
static std::map<Int64, RobotScene*> s_PublicSceneMap;

bool RobotSceneManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool RobotSceneManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_TilemapManager, TilemapManager);
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameRobot, GameRobot);
    GetModule(g_Engine, g_GameWorld, GameWorld);

    g_GameRobot->RegistServerProtobuf(eServerID::SceneInfoPush, OnSceneInfoPush);

    std::string tilemapPath = g_Engine->GetLaunchParameter("TilemapPath");
    for (auto itor = g_Config->GetAutoConfig()->GetSceneConfigMap().begin(); itor != g_Config->GetAutoConfig()->GetSceneConfigMap().end(); itor++) {
        std::string path = tilemapPath + itor->second->Preload + ".xml";

        g_TilemapManager->LoadIsometricTilemap(
            path,
            [&](IIsometricTilemap* handle) {
                XASSERT(handle, "wtf");
                TraceLog(g_Engine, "Load tilemap %s", path.c_str());
                g_Tilemaps.insert({ itor->second->ID, handle });
            },
            true
        );
    }

    return true;
}

bool RobotSceneManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void RobotSceneManager::OnSceneInfoPush(IRobotSession* const session, const UInt64 sessionId, const oSceneInfoPush& push) {
    auto itor = s_SceneMap.find(push.info().sceneid());
    if (s_SceneMap.end() == itor) {
        const AutoConfig::Struct::Scene* config = g_Config->GetAutoConfig()->QuerySceneConfig(push.info().configid());
        XASSERT(config, "wtf");
        RobotScene* scene = txnew RobotScene(push.info().sceneid(), config, g_Tilemaps[config->ID]->GetLayer("Ground"));
        itor = s_SceneMap.insert({ push.info().sceneid(), scene }).first;

        if (config->SceneType == AutoConfig::Enum::eSceneType::Public) {
            s_PublicSceneMap.insert({ config->ID, scene });
        }
        TraceLog(g_Engine, "OnSceneInfoPush Scene %llu ConfigID %lld", push.info().sceneid(), push.info().configid());
    }

    XASSERT(itor->second->_Config->ID == push.info().configid(), "wtf");
}

IRobotScene* RobotSceneManager::QuerySceneByEntityGuid(const UInt64 sceneId) const {
    auto itor = s_SceneMap.find(sceneId);
    if (s_SceneMap.end() == itor) {
        return nullptr;
    }

    return itor->second;
}

IRobotScene* RobotSceneManager::QueryCharacterScene(const UInt64 characterId) const {
    IGameObject* character = g_GameWorld->QueryGameObject(characterId);
    if (character) {
        {
            auto itor = s_SceneMap.find(character->GetUInt64(Character::SceneGUIDUInt64));
            if (s_SceneMap.end() != itor)
                return itor->second;
        }

        {
            auto itor = s_PublicSceneMap.find(character->GetInt64(Character::LastPublicSceneConfigIDInt64));
            XASSERT(s_PublicSceneMap.end() != itor, "wtf");
            return itor->second;
        }
    }

    XASSERT(false, "wtf");
    return nullptr;
}
