#include "MonsterManager.h"

Api::iEngine* g_Engine = nullptr;
ISceneManager* g_SceneManager = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
IMovement* g_Movement = nullptr;
IFsm* g_Fsm = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
ISkillManager* g_SkillManager = nullptr;

static MonsterManager* s_Self = nullptr;
static std::vector<UInt64> s_BroadcastTemp;

static std::unordered_map<UInt64, std::unordered_set<IGameObject*>> s_SceneMonsterMap;

enum eTimerID {
    MonsterPatrol = 0,
    FindCharacter = 1,
    TraceCharacter = 2,
    AttkCharacter = 3,
    MonsterSkillCast = 4,
    MonsterSkillWarning = 5,
    ReleaseMonster = 6,
    GoBackToBronPosition = 7,
};

bool MonsterManager::Initialize(Api::iEngine* const engine) {
    s_BroadcastTemp.reserve(2048);

    g_Engine = engine;
    s_Self = this;
    return true;
}

bool MonsterManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_Movement, Movement);
    GetModule(g_Engine, g_Fsm, Fsm);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_SkillManager, SkillManager);

    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Create, MonsterManager::OnSceneCreate);
    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Release, MonsterManager::OnSceneRelease);

    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Monster, IGameWorld::eGameWorldEvent::Create,
        [](IGameObject* const go) {
            g_Fsm->CreateFsm(go->Guid());
        }
    );

    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Monster, IGameWorld::eGameWorldEvent::Release,
        [](IGameObject* const go) {
            g_Fsm->ReleaseFsm(go->Guid());
        }
    );

    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Monster, IGameWorld::eGameWorldEvent::Release,
        [](IGameObject* const go) {
            STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterPatrol, go->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::FindCharacter, go->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::TraceCharacter, go->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::AttkCharacter, go->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillCast, go->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillWarning, go->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::ReleaseMonster, go->Guid());
        }
    );

    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Monster, OnMonsterHPChanged);
    g_GameWorld->RegistGameObjectAttributeChangeJudge(eGameObjectType::Monster, OnMonsterHPChangeJudge);
    return true;
}

bool MonsterManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void MonsterManager::MonsterGoBackToBronPosition(IGameObject* const monster) {
    STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterPatrol, monster->Guid());
    STOP_TIMER(g_Engine, s_Self, eTimerID::FindCharacter, monster->Guid());
    STOP_TIMER(g_Engine, s_Self, eTimerID::TraceCharacter, monster->Guid());
    STOP_TIMER(g_Engine, s_Self, eTimerID::AttkCharacter, monster->Guid());

    g_Movement->MoveTo(monster, monster->GetVector2(Monster::BronPositionVector2),
        [=](IGameObject* const go, const bool arrived) {
            if (arrived) {
                if (monster->GetFloat(Entity::SpeedFloat) > 0.0001f)
                {
                    START_TIMER(g_Engine, s_Self, eTimerID::MonsterPatrol, 1000, 1, 1000, monster->Guid());
                }
                START_TIMER(g_Engine, s_Self, eTimerID::FindCharacter, 250, Api::Unlimited, 250, monster->Guid());
                DEBUG(g_Engine, "Monster Move To BronPositionVector2 End %2f,%2f arrived %s", go->Position().x, go->Position().y, arrived ? "true" : "false");
            }
            else {
                START_TIMER(g_Engine, s_Self, eTimerID::GoBackToBronPosition, 100, 1, 100, monster->Guid());
            }
        }
    );
}

void MonsterManager::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    IGameObject* monster = g_GameWorld->QueryGameObject(context);
    if (nullptr == monster) {
        TraceLog(g_Engine, "where is monster %llu", context);
        return;
    }
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(context);
    if (nullptr == scene) {
        ErrorLog(g_Engine, "Monster %llu not in any scene", context);
        return;
    }
    const AutoConfig::Struct::Monster* const monsterConfig = g_Config->GetAutoConfig()->QueryMonsterConfig(monster->ConfigID());

    const AutoConfig::Struct::MonsterSkill* monsterSkillConfig = nullptr;
    if (monsterConfig->MonsterSkill > 0)
    {
        monsterSkillConfig = GetConfig(g_Config, MonsterSkill, monsterConfig->MonsterSkill);
    }

    switch (id) {
    case eTimerID::MonsterPatrol: {
        Vector2 orign = monster->GetVector2(Monster::BronPositionVector2);
        const float angle = SafeTools::Rand() % 360;
        const float distance = 15.0f / (1 + SafeTools::Rand() % 5);

        Vector2 hit;
        scene->GroundTileLayer()->Raycast(orign, angle, distance, hit);
        g_Movement->MoveTo(monster, hit,
            [=](IGameObject* const go, const bool arrived) {
                if (arrived) {
                    START_TIMER(g_Engine, s_Self, eTimerID::MonsterPatrol, 200, 1, 200, monster->Guid());
                }
            }
        );

        break;
    }
    case eTimerID::FindCharacter: {
        scene->GetVisual(monster->Position(), monsterConfig->AwarenessRange,
            [&](const std::vector<UInt64>& other) {
                for (int i = 0; i < other.size(); i++) {
                    IGameObject* go = g_GameWorld->QueryGameObject(other[i]);
                    if (go && go->Type() == eGameObjectType::Character) {
                        if ((go->Position() - monster->Position()).Length() <= monsterConfig->AwarenessRange) {
                            g_Movement->StopMoveTo(monster);
                            STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterPatrol, monster->Guid());
                            STOP_TIMER(g_Engine, s_Self, eTimerID::FindCharacter, monster->Guid());
                            if (monster->GetFloat(Entity::SpeedFloat) > 0.0001f)
                            {
                                START_TIMER(g_Engine, s_Self, eTimerID::TraceCharacter, AutoConfig::Const::Monster::TraceInterval, Api::Unlimited, AutoConfig::Const::Monster::TraceInterval, monster->Guid());
                                monster->SetUInt64(Monster::TargetUInt64, go->Guid());
                            }

                            if (monsterSkillConfig != nullptr)
                            {
                                START_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillCast, monsterSkillConfig->StartRelease, Api::Unlimited, monsterSkillConfig->CoolDown, monster->Guid());
                                if (monsterSkillConfig->IsSheetsPrefab)
                                {
                                    START_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillWarning, monsterSkillConfig->StartRelease - monsterSkillConfig->PreviousTime, Api::Unlimited, monsterSkillConfig->CoolDown, monster->Guid());
                                }
                                DEBUG(g_Engine, "Monster %llu find target %llu, Start TraceCharacter", monster->Guid(), go->Guid());
                            }
                            return;
                        }
                    }
                }
            }
        );

        break;
    }
    case eTimerID::TraceCharacter: {
        IGameObject* target = g_GameWorld->QueryGameObject(monster->GetUInt64(Monster::TargetUInt64));
        if (nullptr == target || g_SceneManager->QuerySceneByEntityGuid(target->Guid()) != scene) {
            DEBUG(g_Engine, "target %llu disappear, Stop TraceCharacter, Start FindCharacter", monster->GetUInt64(Monster::TargetUInt64));
            monster->SetUInt64(Monster::TargetUInt64, INVALID_GAMEOBJECT_ID);
            STOP_TIMER(g_Engine, s_Self, eTimerID::TraceCharacter, monster->Guid());
            MonsterGoBackToBronPosition(monster);
            return;
        }

        if (monsterSkillConfig != nullptr && (monster->Position() - target->Position()).Length() <= monsterSkillConfig->MaxSkillDistacne) {
            DEBUG(g_Engine, "Monster in atk range, STOP eTimerID::TraceCharacter");
            g_Movement->StopMoveTo(monster);
            STOP_TIMER(g_Engine, s_Self, eTimerID::TraceCharacter, monster->Guid());
            START_TIMER(g_Engine, s_Self, eTimerID::AttkCharacter, 0, Api::Unlimited, 250, monster->Guid());
            return;
        }

        DEBUG(g_Engine, "Monster Start Move to %2f,%2f", target->Position().x, target->Position().y);
        g_Movement->MoveTo(monster, target->Position(),
            [=](IGameObject* const go, const bool arrived) {
                if (go) {
                    DEBUG(g_Engine, "Monster Move End %2f,%2f arrived %s", go->Position().x, go->Position().y, arrived ? "true" : "false");
                }
            }
        );

        if ((monster->Position() - monster->GetVector2(Monster::BronPositionVector2)).Length() >= monsterConfig->ExitRange) {
            monster->SetUInt64(Monster::TargetUInt64, INVALID_GAMEOBJECT_ID);
            STOP_TIMER(g_Engine, s_Self, eTimerID::TraceCharacter, monster->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillCast, monster->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillWarning, monster->Guid());
            MonsterGoBackToBronPosition(monster);
            return;
        }

        break;
    }
    case eTimerID::AttkCharacter: {
        IGameObject* target = g_GameWorld->QueryGameObject(monster->GetUInt64(Monster::TargetUInt64));
        if (nullptr == target || g_SceneManager->QuerySceneByEntityGuid(target->Guid()) != scene) {
            DEBUG(g_Engine, "target %llu disappear, Stop AttkCharacter, Start FindCharacter", monster->GetUInt64(Monster::TargetUInt64));
            monster->SetUInt64(Monster::TargetUInt64, INVALID_GAMEOBJECT_ID);
            MonsterGoBackToBronPosition(monster);
            return;
        }

        if (monsterSkillConfig != nullptr && (monster->Position() - target->Position()).Length() > monsterSkillConfig->MaxSkillDistacne) {
            DEBUG(g_Engine, "Monster %llu Start TraceCharacter, out off atk range", target->Guid());
            START_TIMER(g_Engine, s_Self, eTimerID::TraceCharacter, AutoConfig::Const::Monster::TraceInterval, Api::Unlimited, AutoConfig::Const::Monster::TraceInterval, monster->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::AttkCharacter, monster->Guid());
            return;
        }

        break;
    }
    case eTimerID::MonsterSkillWarning: {
        scene->GetVisual(monster->Guid(),
            [&](const std::unordered_set<UInt64>& visual) {
                std::vector<UInt64> visibleCharacters;
                std::vector<UInt64> visibleAccounts;

                for (UInt64 guid : visual) {
                    IGameObject* go = g_GameWorld->QueryGameObject(guid);
                    if (go && go->Type() == eGameObjectType::Character && g_Fsm->GetState(go->Guid()) != AutoConfig::Enum::eState::Death) {
                        float distance = (go->Position() - monster->Position()).Length();
                        if (distance <= monsterSkillConfig->MaxSkillDistacne) {
                            visibleCharacters.push_back(guid);
                            visibleAccounts.push_back(go->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }

                if (!visibleCharacters.empty()) {
                    UInt64 targetGuid = visibleCharacters[0];
                    IGameObject* target = g_GameWorld->QueryGameObject(targetGuid);

                    if (target) {
                        monster->SetBool(Monster::IsPreviousSuccessBool, true);
                        monster->SetVector2(Monster::PreviousPositionVector2, target->Position());

                        oSkillWarningPush push;
                        push.set_castguid(monster->Guid());
                        push.set_skillconfigid(monsterConfig->MonsterSkill);
                        push.set_angle(0);
                        oVector2* pos = push.mutable_targetpos();
                        pos->set_x(target->Position().x);
                        pos->set_y(target->Position().y);

                        g_PlayerManager->Broadcast(visibleAccounts, eServerID::SkillWarningPush, push);
                    }
                }
            }
        );

        break;
    }
    case eTimerID::MonsterSkillCast:
    {
        if (monsterSkillConfig->IsTrack && monster->GetBool(Monster::IsPreviousSuccessBool))
        {

            //模拟客户端发送技能消息
            g_SkillManager->SkillCast(monster, 0, monsterConfig->MonsterSkill, 0, Vector2(0, 0), monster->GetVector2(Monster::PreviousPositionVector2));
        }
        else
        {
            IGameObject* target = g_GameWorld->QueryGameObject(monster->GetUInt64(Monster::TargetUInt64));
            if (nullptr == target || g_Fsm->GetState(target->Guid()) == AutoConfig::Enum::eState::Death || g_SceneManager->QuerySceneByEntityGuid(target->Guid()) != scene) {
                DEBUG(g_Engine, "target %llu disappear, Stop TraceCharacter, Start FindCharacter", monster->GetUInt64(Monster::TargetUInt64));
                monster->SetUInt64(Monster::TargetUInt64, INVALID_GAMEOBJECT_ID);
                STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillCast, monster->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillWarning, monster->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::AttkCharacter, monster->Guid());
                MonsterGoBackToBronPosition(monster);
                return;
            }
            float angle = GeometryUtils::CalculateAngleBetweenPoints(monster->Position(), target->Position());
            if ((monster->Position() - target->Position()).Length() <= monsterSkillConfig->MaxSkillDistacne)
            {
                g_SkillManager->SkillCast(monster, 0, monsterConfig->MonsterSkill, angle);
            }
        }
        monster->SetBool(Monster::IsPreviousSuccessBool, false);
        break;
    }
    case eTimerID::ReleaseMonster: {
        IGameObject* monster = g_GameWorld->QueryGameObject(context);
        if (monster) {
            /*IScene* scene = g_SceneManager->QuerySceneByEntityGuid(monster->Guid());
            if (nullptr == scene) {
                ErrorLog(g_Engine, "monster is not in any scene %llu", monster->Guid());
            }
            else {
                scene->ExitScene(monster->Guid());
            }*/
            auto itor = s_SceneMonsterMap.find(scene->SceneID());
            if (itor != s_SceneMonsterMap.end())
            {
                auto itor2 = itor->second.find(monster);
                if (itor2 != itor->second.end())
                {
                    itor->second.erase(itor2);
                }
            }
            else
            {
                ErrorLog(g_Engine, "SceneMonsterMap has not sceneId %llu data", scene->SceneID());
            }
            g_GameWorld->ReleaseGameObjectByGuid(context);
        }
        else {
            ErrorLog(g_Engine, "where is monster %llu", context);
        }
        break;
    }
    case eTimerID::GoBackToBronPosition: {
        IGameObject* monster = g_GameWorld->QueryGameObject(context);
        if (monster) {
            MonsterGoBackToBronPosition(monster);
        }
        break;
    }
    }
}

void MonsterManager::OnSceneCreate(IScene* const scene) {

}

void MonsterManager::OnSceneRelease(IScene* const scene) {
    if (s_SceneMonsterMap.find(scene->SceneID()) != s_SceneMonsterMap.end())
    {
        for (IGameObject* monster : s_SceneMonsterMap.find(scene->SceneID())->second)
        {
            if (monster->Is(eGameObjectType::Monster))
            {
                STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterPatrol, monster->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::FindCharacter, monster->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::TraceCharacter, monster->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::AttkCharacter, monster->Guid());

                STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillCast, monster->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillWarning, monster->Guid());

                STOP_TIMER(g_Engine, s_Self, eTimerID::GoBackToBronPosition, monster->Guid());
                g_GameWorld->ReleaseGameObject(monster);
            }
        }

        s_SceneMonsterMap.erase(scene->SceneID());
    }
}

IGameObject* MonsterManager::CreateMonster(IScene* const scene, const Int64 configId, const Vector2& pos, const fInitMonster& initer) {
    if (s_SceneMonsterMap.find(scene->SceneID()) == s_SceneMonsterMap.end())
    {
        s_SceneMonsterMap.insert({ scene->SceneID(), {} });
    }
    
    const AutoConfig::Struct::Monster* monsterConfig = g_Config->GetAutoConfig()->QueryMonsterConfig(configId);
    IGameObject* monster = g_GameWorld->CreateGameObject(eGameObjectType::Monster, configId, AUTO_GUID,
        [&](IGameObject* const go) {
            go->SetFloat(Monster::AtkFloat, monsterConfig->Atk);
            go->SetFloat(Monster::SpeedFloat, monsterConfig->Speed);
            go->SetFloat(Monster::HpFloat, monsterConfig->Hp);
            go->SetFloat(Monster::MaxHpFloat, monsterConfig->Hp);
            go->SetFloat(Monster::CriticalFloat, monsterConfig->Critical);
            go->SetFloat(Monster::CriticalDamageFloat, monsterConfig->CriticalDamage);
            go->SetFloat(Monster::DefendFloat, monsterConfig->Defend);
            go->SetFloat(Monster::MagicDefendFloat, monsterConfig->MagicDefend);
            go->SetFloat(Monster::BloodFloat, monsterConfig->Blood);
            go->SetFloat(Monster::MagicAttackFloat, monsterConfig->MagicAttack);
            go->SetFloat(Monster::PhysicalPierceFloat, monsterConfig->PhysicalPierce);
            go->SetFloat(Monster::PhysicalPierceRateFloat, monsterConfig->PhysicalPierceRate);
            go->SetFloat(Monster::MagicPierceFloat, monsterConfig->MagicPierce);
            go->SetFloat(Monster::MagicPierceRateFloat, monsterConfig->MagicPierceRate);

            go->SetInt32(Monster::DamagePriorityInt32, (Int32)monsterConfig->DamagePriority);
            go->SetFloat(Monster::DodgeFloat, monsterConfig->Dodge);
            go->SetFloat(Monster::ResilienceFloat, monsterConfig->Resilience);
            go->SetVector2(Monster::BronPositionVector2, pos);
            go->SetString(Monster::NicknameString, monsterConfig->Name);

            if (monsterConfig->IsHurtAppear)
            {
                go->SetBool(Monster::IsCantDamagedBool, true);
            }

            initer(go);
        }
    );

    monster->SetPosition(pos);
    scene->EnterScene(monster->Guid(), nullptr, false);

    s_SceneMonsterMap.find(scene->SceneID())->second.insert(monster);

    if (monster->GetFloat(Entity::SpeedFloat) > 0.0001f) {
        START_TIMER(g_Engine, s_Self, eTimerID::MonsterPatrol, 0, 1, 1000, monster->Guid());
    }
    START_TIMER(g_Engine, s_Self, eTimerID::FindCharacter, 250, Api::Unlimited, 250, monster->Guid());
    TraceLog(g_Engine, "CreateMonster %s Position %2f, %2f", monsterConfig->Name, pos.x, pos.y);
    return monster;
}

void MonsterManager::OnMonsterHPChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue) {
    if (index == Monster::HpFloat && !go->Is(eGameObjectType::Boss)) {
        if (newValue <= 0) {
            TraceLog(g_Engine, "Monster %llu HP changed to %f, enter Death state", go->Guid(), newValue);
            if (g_Fsm->EnterState(go->Guid(), AutoConfig::Enum::eState::Death, go->Position(), go->Angle())) {
                STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterPatrol, go->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::FindCharacter, go->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::TraceCharacter, go->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::AttkCharacter, go->Guid());

                STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillCast, go->Guid());
                STOP_TIMER(g_Engine, s_Self, eTimerID::MonsterSkillWarning, go->Guid());

                STOP_TIMER(g_Engine, s_Self, eTimerID::GoBackToBronPosition, go->Guid());
                START_TIMER(g_Engine, s_Self, eTimerID::ReleaseMonster, 1000, 1, 1000, go->Guid());
            }
        }
    }
}

bool MonsterManager::OnMonsterHPChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue) {
    return g_Fsm->GetState(go->Guid()) != AutoConfig::Enum::eState::Death;
}
