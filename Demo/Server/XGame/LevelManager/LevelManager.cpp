#include "LevelManager.h"

Api::iEngine* g_Engine = nullptr;
IGameWorld* g_GameWorld = nullptr;
IFsm* g_Fsm = nullptr;
IConfig* g_Config = nullptr;

static LevelManager* s_Self = nullptr;

XEventPool<ILevelManager::eEvent, IGameObject* const, const UInt64, const UInt64> g_EventPool;

bool LevelManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool LevelManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_Fsm, Fsm);
    GetModule(g_Engine, g_Config, Config);

    g_Fsm->RegistFsmEvent(
        [](const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState) {
            IGameObject* go = g_GameWorld->QueryGameObject(guid);
            switch (go->Type()) {
            case eGameObjectType::Monster:
            case eGameObjectType::Boss:
                g_Fsm->RegistStateEnter(guid, AutoConfig::Enum::eState::Death, OnMonsterBossEnterDeath);
                break;
            }
        }
    );

    return true;
}

bool LevelManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void LevelManager::RegistEvent(const eEvent ev, const fEvent fun, const char* file, const int line) {
    g_EventPool.Register(ev, fun, file, line);
}

void LevelManager::OnMonsterBossEnterDeath(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2&, const float) {
    IGameObject* go = g_GameWorld->QueryGameObject(guid);
    XASSERT(go, "wtf");
    if (go) {
		IGameObject* dealGo = g_GameWorld->QueryGameObject(go->GetUInt64(Entity::WhoAttkMeUInt64));
		if (dealGo && dealGo->Type() == eGameObjectType::Character) {
            float getExp = 0.0f;
            switch (go->Type()) {
            case eGameObjectType::Monster:
                getExp = g_Config->GetAutoConfig()->QueryMonsterConfig(go->ConfigID())->Exp;
                break;
            case eGameObjectType::Boss:
                getExp = g_Config->GetAutoConfig()->QueryMonsterBossConfig(go->ConfigID())->Exp;
                break;
            }

            Int64 currentExp = dealGo->GetInt64(Character::ExpInt64) + getExp;
            Int64 level = dealGo->GetInt64(Character::LevelInt64);
            const AutoConfig::Struct::CharacterLevel* characterLevelConfig = g_Config->GetAutoConfig()->QueryCharacterLevelConfig(level);
			while (characterLevelConfig && currentExp >= characterLevelConfig->UpExp) {
				currentExp -= characterLevelConfig->UpExp;
				level += 1;
				dealGo->SetInt64(Character::LevelInt64, level);
				characterLevelConfig = g_Config->GetAutoConfig()->QueryCharacterLevelConfig(level);
			}
			
            dealGo->SetInt64(Character::ExpInt64, currentExp);
		}
    }
    else {
        ErrorLog(g_Engine, "where is gameobject %llu", guid);
    }
}
