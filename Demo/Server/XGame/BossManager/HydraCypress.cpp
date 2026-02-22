#include "HydraCypress.h"

enum eTimerID {
    FindCharacter = 0,
    NormalAtk = 1,
    SkillWarning = 2,
    SkillCast = 3,
    HPRecoverStart = 4,
    HPRecover = 5,
    CharacterUpdate = 6,
    ReleaseMonsterBoss = 7,
};

static HydraCypress* s_Self = nullptr;

static std::vector<UInt64> s_BroadcastTemp;

UInt64 HydraCypress::GenerateTimerId() {
    return g_GameWorld->GenerateGuid();
}

HydraCypress::~HydraCypress()
{
    if (_BossGo != nullptr)
    {
        std::vector<int> combatTypes = {
                    eTimerID::HPRecoverStart,
                    eTimerID::HPRecover,
                    eTimerID::NormalAtk,
                    eTimerID::SkillWarning,
                    eTimerID::SkillCast,
                    eTimerID::CharacterUpdate,
                    eTimerID::FindCharacter,
                    eTimerID::ReleaseMonsterBoss,
        };
        StopTimersForBoss(_BossGo->Guid(), combatTypes);
        g_GameWorld->ReleaseGameObjectByGuid(_BossGo->Guid());
        _BossGo = nullptr;
    }
}

IGameObject* HydraCypress::CreateBossHydraCypress(IScene* const scene, const Int64 configId, const Vector2& pos)
{
    const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, configId);
    IGameObject* boss = g_GameWorld->CreateGameObject(eGameObjectType::Boss, configId, AUTO_GUID,
        [&](IGameObject* const go) {
            go->SetFloat(Boss::AtkFloat, monsterBossConfig->Atk);
            go->SetFloat(Boss::SpeedFloat, monsterBossConfig->Speed);
            go->SetFloat(Boss::HpFloat, monsterBossConfig->Hp);
            go->SetFloat(Boss::MaxHpFloat, monsterBossConfig->Hp);
            go->SetFloat(Boss::CriticalFloat, monsterBossConfig->Critical);
            go->SetFloat(Boss::CriticalDamageFloat, monsterBossConfig->CriticalDamage);
            go->SetFloat(Boss::DefendFloat, monsterBossConfig->Defend);
            go->SetFloat(Boss::MagicDefendFloat, monsterBossConfig->MagicDefend);
            go->SetFloat(Boss::BloodFloat, monsterBossConfig->Blood);
            go->SetFloat(Boss::MagicAttackFloat, monsterBossConfig->MagicAttack);
            go->SetFloat(Boss::PhysicalPierceFloat, monsterBossConfig->PhysicalPierce);
            go->SetFloat(Boss::PhysicalPierceRateFloat, monsterBossConfig->PhysicalPierceRate);
            go->SetFloat(Boss::MagicPierceFloat, monsterBossConfig->MagicPierce);
            go->SetFloat(Boss::MagicPierceRateFloat, monsterBossConfig->MagicPierceRate);

            go->SetInt32(Boss::DamagePriorityInt32, (Int32)monsterBossConfig->DamagePriority);
            go->SetFloat(Boss::DodgeFloat, monsterBossConfig->Dodge);
            go->SetFloat(Boss::ResilienceFloat, monsterBossConfig->Resilience);
            go->SetVector2(Boss::BronPositionVector2, pos);
            go->SetString(Boss::NicknameString, monsterBossConfig->Name);
        });

    boss->SetPosition(pos);
    scene->EnterScene(boss->Guid(), nullptr, false);

    TraceLog(g_Engine, "Create Boss HydraCypress Guid:: %llu", boss->Guid());

    _BossGo = boss;

    s_Self = this;

    s_BroadcastTemp.reserve(2048);

    UInt64 findCharacterTimerId = GenerateTimerId();
    BossTimerData* findCharacterData = new BossTimerData{
        boss->Guid(),
        0,
        eTimerID::FindCharacter,
        findCharacterTimerId,
        Vector2(0,0),
        false
    };
    _TimerContexts[findCharacterTimerId] = findCharacterData;

    START_TIMER(g_Engine, this, eTimerID::FindCharacter, 250, Api::Unlimited, 250, findCharacterTimerId);

    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Boss, OnMonsterBossHpChanged);
    g_GameWorld->RegistGameObjectAttributeChangeJudge(eGameObjectType::Boss, OnMonsterBossHPChangeJudge);

    
    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Boss, IGameWorld::eGameWorldEvent::Release,
        [](IGameObject* const go) {
            std::vector<int> combatTypes = {
                    eTimerID::HPRecoverStart,
                    eTimerID::HPRecover,
                    eTimerID::NormalAtk,
                    eTimerID::SkillWarning,
                    eTimerID::SkillCast,
                    eTimerID::CharacterUpdate,
                    eTimerID::FindCharacter,
                    eTimerID::ReleaseMonsterBoss
            };

            s_Self->StopTimersForBoss(go->Guid(), combatTypes);
        }
    );
    return boss;
}

BossTimerData* HydraCypress::GetSkillTimerData(UInt64 bossGuid, Int64 skillConfigId, int timerType)
{
    for (auto& pair : _TimerContexts)
    {
        BossTimerData* data = pair.second;
        if (data && data->guid == bossGuid && data->skillConfig == skillConfigId && data->timerType == timerType)
        {
            return data;
        }
    }

    return nullptr;
}

const Int64 HydraCypress::GetStateNormalAtkConfigID(Int32 index)
{
    const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, _BossGo->ConfigID());
    switch (_BossState)
    {
    case eBossState::Normal:
    {
        if (index == 1)
        {
            return monsterBossConfig->Atk1;
        }
        else if (index == 2)
        {
            return monsterBossConfig->Atk2;
        }
        break;
    }
    case eBossState::Enraged1:
    {
        if (index == 1)
        {
            return monsterBossConfig->SecondAtk1;
        }
        else if (index == 2)
        {
            return monsterBossConfig->SecondAtk2;
        }
        break;
    }
    case eBossState::Enraged2:
    {
        if (index == 1)
        {
            return monsterBossConfig->ThirdAtk1;
        }
        else if (index == 2)
        {
            return monsterBossConfig->ThirdAtk2;
        }
        break;
    }
    }

    return INVALID_GAMEOBJECT_ID;
}

const Int64* HydraCypress::GetStateSkillConfigIDs()
{
    const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, _BossGo->ConfigID());
    switch (_BossState)
    {
    case eBossState::Normal:
    {
        return monsterBossConfig->MonsterSkill1.data();
    }
    case eBossState::Enraged1:
    {
        return monsterBossConfig->MonsterSkill2.data();
    }
    case eBossState::Enraged2:
    {
        return monsterBossConfig->MonsterSkill3.data();
    }
    }

    return INVALID_GAMEOBJECT_ID;
}

void HydraCypress::OnEnd(const int id, const UInt64 context, bool nonviolent, const Int64 timestamp)
{
    UInt64 timerId = context;
    auto it = _TimerContexts.find(timerId);
    if (it != _TimerContexts.end()) {
        BossTimerData* data = it->second;
        it->second = nullptr;
        _TimerContexts.erase(it);
        delete data;
    }
}

void HydraCypress::StopTimersForBoss(UInt64 bossGuid, const std::vector<int>& timerTypes)
{
    std::vector<UInt64> timersToStop;

    for (auto& pair : _TimerContexts) {
        if (pair.second && pair.second->guid == bossGuid) {
            if (timerTypes.empty() ||
                std::find(timerTypes.begin(), timerTypes.end(), pair.second->timerType) != timerTypes.end()) {
                timersToStop.push_back(pair.first);
            }
        }
    }

    for (UInt64 timerId : timersToStop) {
        auto it = _TimerContexts.find(timerId);
        if (it != _TimerContexts.end()) {
            BossTimerData* data = it->second;
            if (data) {
                STOP_TIMER(g_Engine, this, data->timerType, timerId);
            }
        }
    }
}

void HydraCypress::OnTimer(const int id, const UInt64 context, const Int64 timestamp)
{
    UInt64 timerId = context;
    auto it = _TimerContexts.find(timerId);
    if (it == _TimerContexts.end()) {
        return;
    }

    BossTimerData* bossTimerData = it->second;
    if (!bossTimerData) return;
    IGameObject* boss = g_GameWorld->QueryGameObject(bossTimerData->guid);
    if (boss == nullptr)
    {
        ErrorLog(g_Engine, "Where is boss %llu", bossTimerData->guid);
        return;
    }
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(bossTimerData->guid);
    if (nullptr == scene) {
        ErrorLog(g_Engine, "Monster %llu not in any scene", bossTimerData->guid);
        return;
    }
    const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, boss->ConfigID());

    switch (id)
    {
    case eTimerID::FindCharacter:
    {
        scene->GetVisual(boss->Position(), monsterBossConfig->AwarenessRange,
            [&](const std::vector<UInt64>& other)
            {
                for (int i = 0; i < other.size(); i++)
                {
                    IGameObject* go = g_GameWorld->QueryGameObject(other[i]);
                    if (go && go->Type() == eGameObjectType::Character)
                    {
                        if ((go->Position() - boss->Position()).Length() <= monsterBossConfig->AwarenessRange)
                        {
                            boss->SetUInt64(Monster::TargetUInt64, go->Guid());

                            const AutoConfig::Struct::MonsterSkill* monsterSkillConfig = GetConfig(g_Config, MonsterSkill, monsterBossConfig->Atk1);
                            UpdateAtkTimer(boss);

                            BossTimerData* stopFindData = _TimerContexts[bossTimerData->timerId];
                            if (stopFindData) {
                                STOP_TIMER(g_Engine, this, eTimerID::FindCharacter, stopFindData->timerId);
                            }

                            UInt64 updateTimerId = GenerateTimerId();
                            BossTimerData* updateData = new BossTimerData{
                                boss->Guid(),
                                0,
                                eTimerID::CharacterUpdate,
                                updateTimerId,
                                Vector2(0,0),
                                false
                            };
                            _TimerContexts[updateTimerId] = updateData;

                            START_TIMER(g_Engine, this, eTimerID::CharacterUpdate, 250, Api::Unlimited, 250, updateTimerId);
                            return;
                        }
                    }
                }
            });
        break;
    }
    case eTimerID::CharacterUpdate:
    {
        IGameObject* target = g_GameWorld->QueryGameObject(boss->GetUInt64(Monster::TargetUInt64));
        if (target == nullptr || (boss->Position() - target->Position()).Length() > monsterBossConfig->AwarenessRange)
        {
            bool foundTarget = false;

            scene->GetVisual(boss->Position(), monsterBossConfig->AwarenessRange,
                [&](const std::vector<UInt64>& other)
                {
                    for (int i = 0; i < other.size(); i++)
                    {
                        IGameObject* go = g_GameWorld->QueryGameObject(other[i]);
                        if (go && go->Type() == eGameObjectType::Character)
                        {
                            boss->SetUInt64(Monster::TargetUInt64, go->Guid());
                            foundTarget = true;
                            return;
                        }
                    }
                });

            if (!foundTarget)
            {
                std::vector<int> combatTypes = {
                    eTimerID::HPRecoverStart,
                    eTimerID::HPRecover,
                    eTimerID::NormalAtk,
                    eTimerID::SkillWarning,
                    eTimerID::SkillCast,
                    eTimerID::CharacterUpdate,
                };
                StopTimersForBoss(boss->Guid(), combatTypes);

                UInt64 updateTimerId = GenerateTimerId();
                BossTimerData* findCharacterData = new BossTimerData{
                    boss->Guid(),
                    0,
                    eTimerID::FindCharacter,
                    updateTimerId,
                    Vector2(0,0),
                    false
                };
                _TimerContexts[updateTimerId] = findCharacterData;

                START_TIMER(g_Engine, this, eTimerID::FindCharacter, 250, Api::Unlimited, 250, updateTimerId);

                _BossState = eBossState::Normal;
                boss->SetFloat(Entity::HpFloat, boss->GetFloat(Entity::MaxHpFloat));
                return;
            }
        }
        break;
    }
    case eTimerID::NormalAtk:
    {
        if (boss->GetInt64(Entity::LastSkillConfigIDInt64) == GetStateNormalAtkConfigID(1))
        {
            g_SkillManager->SkillCast(boss, INVALID_GAMEOBJECT_ID, GetStateNormalAtkConfigID(2));
        }
        else
        {
            g_SkillManager->SkillCast(boss, INVALID_GAMEOBJECT_ID, GetStateNormalAtkConfigID(1));
        }
        break;
    }
    case eTimerID::SkillWarning:
    {
        const AutoConfig::Struct::MonsterSkill* monsterSkillConfig = GetConfig(g_Config, MonsterSkill, bossTimerData->skillConfig);


        scene->GetVisual(boss->Guid(),
            [&](const std::unordered_set<UInt64>& visual) {
                std::vector<UInt64> visibleCharacters;
                std::vector<UInt64> visibleAccounts;

                for (UInt64 guid : visual) {
                    IGameObject* go = g_GameWorld->QueryGameObject(guid);
                    if (go && go->Type() == eGameObjectType::Character && g_Fsm->GetState(go->Guid()) != AutoConfig::Enum::eState::Death) {
                        float distance = (go->Position() - boss->Position()).Length();
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
                        bossTimerData->isPreviousSuccess = true;
                        bossTimerData->previousPosition = target->Position();

                        oSkillWarningPush push;
                        push.set_castguid(boss->Guid());
                        push.set_skillconfigid(bossTimerData->skillConfig);
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
    case eTimerID::SkillCast:
    {
        const AutoConfig::Struct::MonsterSkill* monsterSkillConfig = GetConfig(g_Config, MonsterSkill, bossTimerData->skillConfig);

        BossTimerData* warningData = GetSkillTimerData(boss->Guid(), bossTimerData->skillConfig, eTimerID::SkillWarning);

        if (monsterSkillConfig->IsTrack && warningData && warningData->isPreviousSuccess)
        {
            //模拟客户端发送技能消息
            g_SkillManager->SkillCast(boss, 0, bossTimerData->skillConfig, 0, Vector2(0, 0), warningData->previousPosition);

            warningData->isPreviousSuccess = false;
        }
        else
        {
            IGameObject* target = g_GameWorld->QueryGameObject(boss->GetUInt64(Monster::TargetUInt64));
            if (target != nullptr && (boss->Position() - target->Position()).Length() <= monsterSkillConfig->MaxSkillDistacne)
            {
                g_SkillManager->SkillCast(boss, 0, bossTimerData->skillConfig);
            }

            if (warningData) {
                warningData->isPreviousSuccess = false;
            }
        }

        break;
    }
    case eTimerID::HPRecoverStart:
    {
        BossTimerData* recoverOldData = _TimerContexts[bossTimerData->timerId];
        if (recoverOldData) {
            STOP_TIMER(g_Engine, this, eTimerID::HPRecoverStart, recoverOldData->timerId);
        }

        UInt64 recoverTimerId = GenerateTimerId();
        BossTimerData* recoverData = new BossTimerData{
            boss->Guid(),
            0,
            eTimerID::HPRecover,
            recoverTimerId,
            Vector2(0,0),
            false
        };
        _TimerContexts[recoverTimerId] = recoverData;

        START_TIMER(g_Engine, this, eTimerID::HPRecover, 0, Api::Unlimited,
            monsterBossConfig->HpRecoverRate, recoverTimerId);
        break;
    }
    case eTimerID::HPRecover:
    {
        float leftHp = boss->GetFloat(Entity::HpFloat) + boss->GetFloat(Entity::MaxHpFloat) * monsterBossConfig->HpRecover;
        boss->SetFloat(Entity::HpFloat, leftHp > boss->GetFloat(Entity::MaxHpFloat) ? boss->GetFloat(Entity::MaxHpFloat) : leftHp);
        break;
    }
    case eTimerID::ReleaseMonsterBoss:
    {
        if (boss)
        {
            /*IScene* scene = g_SceneManager->QuerySceneByEntityGuid(boss->Guid());
            if (nullptr == scene) {
                ErrorLog(g_Engine, "boss is not in any scene %llu", boss->Guid());
            }
            else {
                scene->ExitScene(boss->Guid());
            }*/

            g_GameWorld->ReleaseGameObjectByGuid(boss->Guid());
            _BossGo = nullptr;
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

void HydraCypress::UpdateAtkTimer(IGameObject* boss, bool IsNeedStopOld)
{
    if (IsNeedStopOld)
    {
        std::vector<int> iTimerTypes = {
                    eTimerID::NormalAtk, eTimerID::SkillWarning,
                    eTimerID::SkillCast };
        s_Self->StopTimersForBoss(boss->Guid(), iTimerTypes);
    }

    const AutoConfig::Struct::MonsterSkill* monsterSkillConfig = GetConfig(g_Config, MonsterSkill, GetStateNormalAtkConfigID(1));
    const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, _BossGo->ConfigID());

    //普攻
    UInt64 normalAtkTimerId = GenerateTimerId();
    BossTimerData* normalAtkData = new BossTimerData{
        boss->Guid(),
        0,
        eTimerID::NormalAtk,
        normalAtkTimerId,
        Vector2(0,0),
        false
    };

    _TimerContexts[normalAtkTimerId] = normalAtkData;

    START_TIMER(g_Engine, this, eTimerID::NormalAtk,
        monsterSkillConfig->StartRelease,
        Api::Unlimited,
        monsterSkillConfig->CoolDown,
        normalAtkTimerId);

    //技能
    for (int i = 0; i < monsterBossConfig->MonsterSkill1.size(); i++)
    {
        Int64 monsterSkillID = GetStateSkillConfigIDs()[i];

        const AutoConfig::Struct::MonsterSkill* monsterSkillConfig = GetConfig(g_Config, MonsterSkill, monsterSkillID);
        UInt64 skillCastTimerId = GenerateTimerId();
        BossTimerData* skillCastData = new BossTimerData{
            boss->Guid(),
            monsterSkillID,
            eTimerID::SkillCast,
            skillCastTimerId,
            Vector2(0,0),
            false
        };
        _TimerContexts[skillCastTimerId] = skillCastData;

        START_TIMER(g_Engine, this, eTimerID::SkillCast,
            monsterSkillConfig->StartRelease,
            Api::Unlimited,
            monsterSkillConfig->CoolDown,
            skillCastTimerId);
        if (monsterSkillConfig->IsSheetsPrefab)
        {
            UInt64 warningTimerId = GenerateTimerId();
            BossTimerData* warningData = new BossTimerData{
                boss->Guid(),
                monsterSkillID,
                eTimerID::SkillWarning,
                warningTimerId,
                Vector2(0,0),
                false
            };
            _TimerContexts[warningTimerId] = warningData;

            START_TIMER(g_Engine, this, eTimerID::SkillWarning,
                monsterSkillConfig->StartRelease - monsterSkillConfig->PreviousTime,
                Api::Unlimited,
                monsterSkillConfig->CoolDown,
                warningTimerId);
        }
    }
}

void HydraCypress::UpdateBossState(float newHpValue)
{
    const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, _BossGo->ConfigID());
    if (newHpValue < _BossGo->GetFloat(Entity::MaxHpFloat) * monsterBossConfig->ExchangeMode3)
    {
        if (_BossState != eBossState::Enraged2)
        {
            _BossState = eBossState::Enraged2;
            UpdateAtkTimer(_BossGo, true);
        }
    }
    else if (newHpValue < _BossGo->GetFloat(Entity::MaxHpFloat) * monsterBossConfig->ExchangeMode2)
    {
        if (_BossState != eBossState::Enraged1)
        {
            _BossState = eBossState::Enraged1;
            UpdateAtkTimer(_BossGo, true);
        }
    }
    else
    {
        _BossState = eBossState::Normal;
    }
}

void HydraCypress::OnMonsterBossHpChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue)
{
    if (index == Monster::HpFloat && newValue < oldValue)
    {

        std::vector<int> hpRecoveryTypes = { eTimerID::HPRecover, eTimerID::HPRecoverStart };
        s_Self->StopTimersForBoss(go->Guid(), hpRecoveryTypes);

        // 如果还有血量，启动HP恢复流程
        if (newValue > 0)
        {
            const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, go->ConfigID());

            UInt64 recoverStartTimerId = s_Self->GenerateTimerId();
            BossTimerData* recoverStartData = new BossTimerData{
                go->Guid(),
                0,
                eTimerID::HPRecoverStart,
                recoverStartTimerId,
                Vector2(0,0),
                false
            };
            s_Self->_TimerContexts[recoverStartTimerId] = recoverStartData;

            START_TIMER(g_Engine, s_Self, eTimerID::HPRecoverStart,
                monsterBossConfig->StartRecover,
                1,
                0,
                recoverStartTimerId);
        }
    }

    if (index == Monster::HpFloat)
    {
        if (newValue <= 0)
        {
            if (g_Fsm->EnterState(go->Guid(), AutoConfig::Enum::eState::Death, go->Position(), go->Angle()))
            {
                std::vector<int> iTimerTypes = { eTimerID::HPRecover, eTimerID::HPRecoverStart,
                    eTimerID::FindCharacter,eTimerID::NormalAtk, eTimerID::SkillWarning,
                    eTimerID::SkillCast, eTimerID::CharacterUpdate };
                s_Self->StopTimersForBoss(go->Guid(), iTimerTypes);

                UInt64 releaseTimerId = s_Self->GenerateTimerId();
                BossTimerData* releaseData = new BossTimerData{
                    go->Guid(),
                    0,
                    eTimerID::ReleaseMonsterBoss,
                    releaseTimerId,
                    Vector2(0,0),
                    false
                };
                s_Self->_TimerContexts[releaseTimerId] = releaseData;

                const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, go->ConfigID());
                START_TIMER(g_Engine, s_Self, eTimerID::ReleaseMonsterBoss,
                    monsterBossConfig->Die,
                    1,
                    monsterBossConfig->Die,
                    releaseTimerId);
            }
        }
    }

    if (index == Monster::HpFloat && s_Self)
    {
        s_Self->UpdateBossState(newValue);
    }
}
    
bool HydraCypress::OnMonsterBossHPChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue) {
    return g_Fsm->GetState(go->Guid()) != AutoConfig::Enum::eState::Death;
}