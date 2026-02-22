#include "NpcManager.h"

Api::iEngine* g_Engine = nullptr;
ISceneManager* g_SceneManager = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
ICharacterManager* g_CharacterManager = nullptr;
IPlayerManager* g_PlayerManager = nullptr;

static NpcManager* s_Self = nullptr;

static std::unordered_map<UInt64, std::unordered_set<IGameObject*>> s_Npcs;

bool NpcManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool NpcManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_CharacterManager, CharacterManager);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);

    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Create, OnSceneCreate);
    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Release, OnSceneRelease);

    g_PlayerManager->RegistPlayerEvent(IPlayerManager::ePlayerEvent::PlayerLoaded, OnPlayerCreated);
    return true;
}

bool NpcManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void NpcManager::OnSceneCreate(IScene* const scene) {
    std::unordered_set<IGameObject*> npcs;
    for (int i = 0; i < scene->Config()->TaskNpc.size(); i++) {
        const AutoConfig::Struct::Npc* const config = g_Config->GetAutoConfig()->QueryNpcConfig(scene->Config()->TaskNpc[i]);
        IGameObject* npc = g_GameWorld->CreateGameObject(eGameObjectType::Npc, config->ID, AUTO_GUID,
            [&](IGameObject* const npc) {
                npc->SetPosition(config->NpcPosition[0], config->NpcPosition[1]);
            }
        );
        npcs.insert(npc);

        TraceLog(g_Engine, "Scene %llu Create Npc %s Config %lld", scene->SceneID(), config->Name, config->ID);
    }

    s_Npcs.insert({ scene->SceneID(), npcs });

    scene->RegistEvent(IScene::eEvent::Enter, OnEnterScene);
    scene->RegistEvent(IScene::eEvent::Leave, OnLeaveScene);
}

void NpcManager::OnSceneRelease(IScene* const scene) {

}

__forceinline void SendNpcInfoToPlayer(IScene* const scene, IGameObject* const character) {
    auto itor = s_Npcs.find(scene->SceneID());
    if (s_Npcs.end() == itor) {
        ErrorLog(g_Engine, "Scene %llu has no npc", scene->SceneID());
        return;
    }

    if (itor->second.size()) {
        oNpcInfoPush push;
        for (auto it = itor->second.begin(); it != itor->second.end(); it++) {
            oNpcInfo* info = push.add_infos();
            info->set_guid((*it)->Guid());
            info->set_sceneid(scene->SceneID());
            info->set_configid((*it)->ConfigID());
            info->mutable_pos()->set_x((*it)->Position().x);
            info->mutable_pos()->set_y((*it)->Position().y);
        }

        g_CharacterManager->SendProtobuf(character->Guid(), eServerID::NpcInfoPush, push);
    }
}

void NpcManager::OnPlayerCreated(IPlayer* const player, const UInt64 account) {
    IGameObject* character = g_GameWorld->QueryGameObject(player->GetDeployCharacterID());
    if (character) {
        IScene* scene = g_SceneManager->QuerySceneByEntityGuid(character->Guid());
        XASSERT(scene, "wtf");
        if (nullptr == scene) {
            ErrorLog(g_Engine, "Character %llu not in any scene", character->Guid());
            return;
        }

        SendNpcInfoToPlayer(scene, character);
    }
}

void NpcManager::OnEnterScene(IScene* const scene, const UInt64 guid) {
    IGameObject* go = g_GameWorld->QueryGameObject(guid);
    if (nullptr == go) {
        ErrorLog(g_Engine, "where is go %llu", guid);
        return;
    }

    if (eGameObjectType::Character == go->Type()) {
        SendNpcInfoToPlayer(scene, go);
    }
}

void NpcManager::OnLeaveScene(IScene* const, const UInt64 guid) {

}
