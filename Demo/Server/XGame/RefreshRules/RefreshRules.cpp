#include "RefreshRules.h"

Api::iEngine* g_Engine = nullptr;
ISceneManager* g_SceneManager = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
IMonsterManager* g_MonsterManager = nullptr;
IBossManager* g_BossManager = nullptr;

static RefreshRules* s_Self = nullptr;

static std::unordered_map<UInt64, std::unordered_set<UInt64>> s_SceneRefreshRuless;

enum eTimerID {
    Refresh = 0
};

bool RefreshRules::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool RefreshRules::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_MonsterManager, MonsterManager);
    GetModule(g_Engine, g_BossManager, BossManager);

    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Create, RefreshRules::OnSceneCreate);
    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Release, RefreshRules::OnSceneRelease);

    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Monster, IGameWorld::eGameWorldEvent::Release,
        [](IGameObject* const monster) {
            IGameObject* refreshRule = g_GameWorld->QueryGameObject(monster->GetUInt64(Monster::RefreshRuleGuidUInt64));
            if (refreshRule) {
                refreshRule->SetInt32(RefreshRule::CurrentRefreshCountInt32, refreshRule->GetInt32(RefreshRule::CurrentRefreshCountInt32) - 1);
            }
        }
    );
    return true;
}

bool RefreshRules::Destroy(Api::iEngine* const engine) {
    return true;
}

void RefreshRules::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eTimerID::Refresh: {
        IGameObject* refreshRule = g_GameWorld->QueryGameObject(context);
        if (nullptr == refreshRule) {
            ErrorLog(g_Engine, "Where is refresh rule %llu", context);
            STOP_TIMER(g_Engine, this, eTimerID::Refresh, context);
            return;
        }

        const AutoConfig::Struct::RefreshRule* refreshConfig = g_Config->GetAutoConfig()->QueryRefreshRuleConfig(refreshRule->ConfigID());
        if (refreshRule->GetInt32(RefreshRule::CurrentRefreshCountInt32) >= refreshConfig->Count) {
            return;
        }

        IScene* const scene = g_SceneManager->QuerySceneBySceneID(refreshRule->GetUInt64(RefreshRule::SceneGuidUInt64));
        if (nullptr == scene) {
            ErrorLog(g_Engine, "Where is scene %llu", refreshRule->GetUInt64(RefreshRule::SceneGuidUInt64));
            STOP_TIMER(g_Engine, this, eTimerID::Refresh, context);
            return;
        }

        const Vector2 orign(refreshConfig->MonsterPosition[0], refreshConfig->MonsterPosition[1]);
        const float angle = SafeTools::Rand() % 360;
        Vector2 hit;
        scene->GroundTileLayer()->Raycast(orign, angle, refreshConfig->PositionRange / (float)(1 + SafeTools::Rand() % 5), hit);

        refreshRule->SetInt32(RefreshRule::CurrentRefreshCountInt32, refreshRule->GetInt32(RefreshRule::CurrentRefreshCountInt32) + 1);
        refreshRule->SetInt32(RefreshRule::TotleRefreshCountInt32, refreshRule->GetInt32(RefreshRule::TotleRefreshCountInt32) + 1);
        switch (refreshConfig->RefreshThing) {
        case AutoConfig::Enum::eRefreshThing::Monster: {
            g_MonsterManager->CreateMonster(scene, refreshConfig->RelevantId, hit,
                [&](IGameObject* const go) {
                    go->SetUInt64(Monster::RefreshRuleGuidUInt64, refreshRule->Guid());
                }
            );
            break;
        }
        case AutoConfig::Enum::eRefreshThing::MonsterBoss: {
            g_BossManager->CreateBoss(scene, refreshConfig->RelevantId, hit,
                [&](IGameObject* const go) {
                    go->SetUInt64(Monster::RefreshRuleGuidUInt64, refreshRule->Guid());
                }
            );
            break;
        }
        }

        if (refreshRule->GetInt32(RefreshRule::TotleRefreshCountInt32) >= refreshConfig->MaxCount) {
            STOP_TIMER(g_Engine, this, eTimerID::Refresh, context);
        }
        break;
    }
    }
}

void RefreshRules::OnSceneCreate(IScene* const scene) {
    auto itor = s_SceneRefreshRuless.insert({ scene->SceneID(), std::unordered_set<UInt64>() }).first;

    const AutoConfig::Struct::Scene* const sceneConfig = scene->Config();

    for (int i = 0; i < sceneConfig->RefreshRule.size(); i++) {
        IGameObject* refreshRule = g_GameWorld->CreateGameObject(eGameObjectType::RefreshRule, sceneConfig->RefreshRule[i], AUTO_GUID,
            [&](IGameObject* const go) {
                go->SetUInt64(RefreshRule::SceneGuidUInt64, scene->SceneID());
                go->SetInt32(RefreshRule::CurrentRefreshCountInt32, 0);
                go->SetInt32(RefreshRule::TotleRefreshCountInt32, 0);
            }
        );

        const AutoConfig::Struct::RefreshRule* refreshConfig = g_Config->GetAutoConfig()->QueryRefreshRuleConfig(refreshRule->ConfigID());
        START_TIMER(g_Engine, s_Self, eTimerID::Refresh, refreshConfig->Interval, Api::Unlimited, refreshConfig->Interval, refreshRule->Guid());
        itor->second.insert(refreshRule->Guid());
    }
}

void RefreshRules::OnSceneRelease(IScene* const scene) {
    auto itor = s_SceneRefreshRuless.find(scene->SceneID());
    if (s_SceneRefreshRuless.end() != itor) {
		for (auto it = itor->second.begin(); it != itor->second.end(); it++) {
			IGameObject* refreshRule = g_GameWorld->QueryGameObject(*it);
			STOP_TIMER(g_Engine, s_Self, eTimerID::Refresh, refreshRule->Guid());
			g_GameWorld->ReleaseGameObject(refreshRule);
		}
		s_SceneRefreshRuless.erase(itor);
    }
}
