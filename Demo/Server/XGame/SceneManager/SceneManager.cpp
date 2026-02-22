#include "SceneManager.h"
#include "Scene.h"

Api::iEngine* g_Engine = nullptr;
IConfig* g_Config = nullptr;
ITilemapManager* g_TilemapManager = nullptr;
IServerManager* g_ServerManager = nullptr;
IGameWorld* g_GameWorld = nullptr;
ICharacterManager* g_CharacterManager = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
SceneManager* g_SceneManager = nullptr;
IFsm* g_Fsm = nullptr;

std::vector<UInt64> s_BroadCastTemp;

static std::string s_TilemapPath;

static std::map<Int64, Scene*> s_PublicScenes;
static std::map<UInt64, Scene*> g_SceneIDMap;
static XEventPool<ISceneManager::eSceneEvent, IScene* const> g_SceneEventPool;

Vector2(*g_GetPositionFunction)(const UInt64 guid) = nullptr;
void(*g_SetPostionFunction)(const UInt64 guid, const Vector2& position) = nullptr;

namespace eTimerID {
	enum {
		ReleaseScene = 1
	};
}

bool SceneManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    g_SceneManager = this;

    s_BroadCastTemp.reserve(1024);
    return true;
}

bool SceneManager::Launch(Api::iEngine* const engine) {
    GetModule(engine, g_Config, Config);
    GetModule(engine, g_TilemapManager, TilemapManager);
    GetModule(engine, g_ServerManager, ServerManager);
    GetModule(engine, g_GameWorld, GameWorld);
    GetModule(engine, g_CharacterManager, CharacterManager);
    GetModule(engine, g_Fsm, Fsm);
    GetModule(engine, g_PlayerManager, PlayerManager);

    s_TilemapPath = g_Engine->GetLaunchParameter("TilemapPath");
    const std::unordered_map<Int64, AutoConfig::Struct::Scene*>& sceneConfigs = g_Config->GetAutoConfig()->GetSceneConfigMap();
    for (auto itor = sceneConfigs.begin(); itor != sceneConfigs.end(); itor++) {
        if (itor->second->SceneType == AutoConfig::Enum::eSceneType::Public || itor->second->SceneType == AutoConfig::Enum::eSceneType::PublicBattle) {
            g_TilemapManager->LoadIsometricTilemap(s_TilemapPath + itor->second->Preload + ".xml",
                [=](IIsometricTilemap* handle) {
                    XASSERT(handle, "wtf");
                    if (handle) {
                        TraceLog(g_Engine, "Load Public Scene %lld %s", itor->second->ID, itor->second->Name);
                        Scene* scene = Scene::CreateScene(g_ServerManager->GeneraterGUID(), itor->second, handle);
                        s_PublicScenes.insert({ itor->second->ID, scene });
                        g_SceneIDMap.insert({ scene->SceneID(), scene });
                        g_SceneEventPool.Trigger(eSceneEvent::Create, scene);
                    }
                    else {
                        ErrorLog(g_Engine, "Load Public Scene %lld %s error", itor->second->ID, itor->second->Name);
                    }
                }
            );
        }
    }

    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Entity, IGameWorld::eGameWorldEvent::Release,
        [](IGameObject* const go) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (scene) {
                scene->ExitScene(go->Guid());
            }
        }
    );

    g_PlayerManager->RegistPlayerProtobuf(eClientID::ExitCopyScene, OnExitCopyScene);
    return true;
}

bool SceneManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void SceneManager::RegistSceneEvent(const eSceneEvent ev, const fSceneEvent& callback) {
    XEventPoolRegist(g_SceneEventPool, ev, callback);
}

void SceneManager::OnExitCopyScene(IPlayer* const player, const UInt64 account, const oExitCopyScene& body) {
    ICharacterDocker* const docker = g_CharacterManager->QueryCharacterDocker(account);
    if (nullptr == docker) {
        ErrorLog(g_Engine, "where is character docker of account %llu", account);
        return;
    }

    IGameObject* const character = docker->GetDeployCharacter();
    if (nullptr == character) {
        ErrorLog(g_Engine, "account %lld do not deploy any character", account);
        return;
    }

    IScene* const scene = g_SceneManager->QueryCopyScene(character);
    if (scene) {
        TraceLog(g_Engine, "Character %llu exit scene copy %llu %s", character->Guid(), scene->SceneID(), scene->Config()->Name);
        scene->ExitScene(character->Guid());

        IScene* publicScene = g_SceneManager->QueryPublicScene(character->GetInt64(Character::LastPublicSceneConfigIDInt64));
        if (nullptr == publicScene) {
            ErrorLog(g_Engine, "character %llu LastPublicSceneConfigIDInt64 %lld error", character->GetInt64(Character::LastPublicSceneConfigIDInt64));
            publicScene = g_SceneManager->QueryPublicScene(AutoConfig::Const::World::DefaultWorld);
        }

        publicScene->EnterScene(character->Guid());
    }

    oGameResultInfoPush push;
    push.set_success(g_Fsm->GetState(character->Guid()) != AutoConfig::Enum::eState::Death);
    g_PlayerManager->SendProtobufToPlayer(account, eServerID::GameResultInfoPush, push);

    g_Fsm->ForceEnterState(character->Guid(), AutoConfig::Enum::eState::Idle, character->Position(), character->Angle());
    character->SetFloat(Entity::HpFloat, character->GetFloat(Entity::MaxHpFloat));
    character->SetFloat(Entity::MpFloat, character->GetFloat(Entity::MaxMpFloat));
}

Scene* SceneManager::QuerySceneByGuid(const UInt64 sceneGuid) const {
    auto it = g_SceneIDMap.find(sceneGuid);
    if (g_SceneIDMap.end() != it) {
        return it->second;
    }
    return nullptr;
}

void SceneManager::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eTimerID::ReleaseScene: {
        Scene* scene = QuerySceneByGuid(context);
        this->ReleaseCopyScene(scene);
        break;
    }
    }
}

void SceneManager::CreateCopyScene(const Int64 configId, const fCreateCopySceneCallback& callback) {
    const AutoConfig::Struct::Scene* const config = g_Config->GetAutoConfig()->QuerySceneConfig(configId);
    if (nullptr == config) {
        ErrorLog(g_Engine, "Error Scene ConfigID %lld", configId);
        callback(nullptr);
        return;
    }

    g_TilemapManager->LoadIsometricTilemap(s_TilemapPath + config->Preload + ".xml",
        [=](IIsometricTilemap* handle) {
            if (handle) {
                TraceLog(g_Engine, "Load Public Scene %lld %s", config->ID, config->Name);
                Scene* scene = Scene::CreateScene(g_ServerManager->GeneraterGUID(), config, handle);
                g_SceneIDMap.insert({ scene->SceneID(), scene });
                g_SceneEventPool.Trigger(eSceneEvent::Create, scene);
                callback(scene);
                START_TIMER(g_Engine, this, eTimerID::ReleaseScene, config->DisappearTime, 1, config->DisappearTime, scene->SceneID());
            }
            else {
                ErrorLog(g_Engine, "Load IIsometricTilemap %s error", config->Preload);
                callback(nullptr);
            }
        }
    );
}

void SceneManager::ReleaseCopyScene(IScene* const scene) {
    STOP_TIMER(g_Engine, this, eTimerID::ReleaseScene, scene->SceneID());
    g_SceneEventPool.Trigger(eSceneEvent::Release, scene);
    g_SceneIDMap.erase(scene->SceneID());
    Scene::ReleaseScene((Scene*)scene);
}

IScene* SceneManager::QueryCopyScene(IGameObject* const character) const {
    UInt64 guid = character->GetUInt64(Character::CopySceneGuidUInt64);

    auto itor = g_SceneIDMap.find(guid);
    if (g_SceneIDMap.end() == itor)
        return nullptr;

    if (itor->second->_Config->SceneType == AutoConfig::Enum::eSceneType::Public
        || itor->second->_Config->SceneType == AutoConfig::Enum::eSceneType::PublicBattle)
        return nullptr;

    return itor->second;
}

IScene* SceneManager::QueryPublicScene(const Int64 configId) const {
    auto itor = s_PublicScenes.find(configId);
    if (s_PublicScenes.end() != itor) return itor->second;
    XASSERT(false, "wtf");
    return nullptr;
}

IScene* SceneManager::QuerySceneByEntityGuid(const UInt64 guid) const {
    IGameObject* go = g_GameWorld->QueryGameObject(guid);
	const UInt64 sceneGuid = go->GetUInt64(Character::SceneGUIDUInt64);
	auto it = g_SceneIDMap.find(sceneGuid);
	if (g_SceneIDMap.end() != it) {
		return it->second;
	}

    return nullptr;
}

IScene* SceneManager::QuerySceneBySceneID(const UInt64 sceneGuid) const {
    return QuerySceneByGuid(sceneGuid);
}
