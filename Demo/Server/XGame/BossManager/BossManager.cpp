#include "BossManager.h"

Api::iEngine* g_Engine = nullptr;
static BossManager* s_Self = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
ISkillManager* g_SkillManager = nullptr;
ISceneManager* g_SceneManager = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
IFsm* g_Fsm = nullptr;

static std::unordered_map<UInt64, IBoss*> s_SceneBossMap;

bool BossManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool BossManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_SkillManager, SkillManager);
    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_Fsm, Fsm);

    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Create, BossManager::OnSceneCreate);
    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Release, BossManager::OnSceneRelease);

    
    return true;
}

void BossManager::OnSceneCreate(IScene* const scene) {

}

void BossManager::OnSceneRelease(IScene* const scene) {
    auto itor = s_SceneBossMap.find(scene->SceneID());
    if (itor != s_SceneBossMap.end())
    {
        IBoss* boss = itor->second;
        if (boss != nullptr)
        {
            txdel boss;
            boss = nullptr;
        }
        s_SceneBossMap.erase(itor);
    }
}

bool BossManager::Destroy(Api::iEngine* const engine) {
    return true;
}

IGameObject* BossManager::CreateBoss(IScene* const scene, const Int64 configId, const Vector2& pos, const fInitBoss& initer) {
    const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, configId);
    if (monsterBossConfig == nullptr)
    {
        ErrorLog(g_Engine, ",MonsterBoss Is Not Exist::%d", configId);
    }
    
    IBoss* boss = nullptr;

    switch (monsterBossConfig->BossType)
    {
    case AutoConfig::Enum::eBoss::HydraCypress:
    { 
        boss = CreateHydraCypress(scene, configId, pos);
        s_SceneBossMap.insert({ scene->SceneID(), boss });
        return boss->_BossGo;
    }
    }

    return nullptr;
}


IHydraCypress* BossManager::CreateHydraCypress(IScene* const scene, const Int64 configId, const Vector2& pos) {
    IHydraCypress* hydraCypress = txnew HydraCypress();
    hydraCypress->CreateBossHydraCypress(scene, configId, pos);
    return hydraCypress;
}
                      