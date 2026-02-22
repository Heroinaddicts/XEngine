#include "BuffManager.h"

Api::iEngine* g_Engine = nullptr;
IGameWorld* g_GameWorld = nullptr;
IConfig* g_Config = nullptr;
IFsm* g_Fsm = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
ISceneManager* g_SceneManager = nullptr;

static BuffManager* s_Self = nullptr;
static std::vector<UInt64> s_BroadcastTemp;

struct Buffs {
    std::map<Int64, int> _StackCounts;
    std::set<IGameObject*> _Buffs;
};

static std::map<UInt64, Buffs> s_GameEntityBuffs;
static std::map<UInt64, Buffs> s_CasterEntityBuffs;

enum eBuffTimer {
    BuffBeat = 0,
    BuffRelease = 1,
    BuffCleanEntity = 2,
};

bool BuffManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;

    s_BroadcastTemp.reserve(2048);
    return true;
}

bool BuffManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_Fsm, Fsm);
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_SceneManager, SceneManager);

    g_Fsm->RegistFsmEvent(OnFsmEvent);
    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, OnCantControllerRefChanged);
    g_GameWorld->RegistGameObjectAttributeChangeJudge(eGameObjectType::Entity, OnCantControllerRefChangeJudge);
    g_GameWorld->RegistGameObjectAttributeChangeJudge(eGameObjectType::Entity, OnInvincibleRefChangeJudge);

    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Entity, IGameWorld::eGameWorldEvent::Create, OnGameEntityCreate);
    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Entity, IGameWorld::eGameWorldEvent::Release, OnGameEntityRelease);

    g_PlayerManager->RegistPlayerProtobuf(eClientID::BuffTest, OnBuffTest);

    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Create,
        [](IScene* const scene) {
            scene->RegistAoiEvent(IScene::eAoiEvent::Appear, BuffManager::OnAppear);
            scene->RegistAoiEvent(IScene::eAoiEvent::Disappear, BuffManager::OnDisappear);
        }
    );

    return true;
}

void BuffManager::OnBuffTest(IPlayer* const player, const UInt64 account, const oBuffTest& req)
{
    IGameObject* character = g_GameWorld->QueryGameObject(player->GetDeployCharacterID());
    XASSERT(character != nullptr, "wtf");
    s_Self->AddBuff(NULL, character, req.buffconfigid());
}

void BuffManager::OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState initState) {
    switch (ev) {
    case IFsm::eFsmEvent::FsmCreated: {
        g_Fsm->RegistExitJudge(guid, AutoConfig::Enum::eState::Stuned, BuffManager::OnExitStunedJudge);
        g_Fsm->RegistStateEnter(guid, AutoConfig::Enum::eState::Death, BuffManager::OnEnterDeath);
        break;
    }
    case IFsm::eFsmEvent::FsmReleased: {
        break;
    }
    }
}

bool BuffManager::Destroy(Api::iEngine* const engine) {
    return true;
}

UInt64 BuffManager::AddBuff(IGameObject* const caster, IGameObject* const entity, const Int64 buffConfigID) {

    TraceLog(g_Engine, "AddBuff");
    const AutoConfig::Struct::Buff* buffConfig = GetConfig(g_Config, Buff, buffConfigID);
    if (nullptr == buffConfig) {
        ErrorLog(g_Engine, "Where is buff config %lld", buffConfigID);
        return INVALID_GAMEOBJECT_ID;
    }

    XASSERT(entity->Is(eGameObjectType::Entity), "wtf");
    auto itor = s_GameEntityBuffs.find(entity->Guid());
    if (s_GameEntityBuffs.end() != itor) {
        auto it = itor->second._StackCounts.find(buffConfigID);
        if (itor->second._StackCounts.end() == it)
            it = itor->second._StackCounts.insert({ buffConfigID, 0 }).first;

        if (it->second >= buffConfig->MaxStackCount)
            return INVALID_GAMEOBJECT_ID;

        it->second++;

        IGameObject* buffGo = g_GameWorld->CreateGameObject(eGameObjectType::Buff, buffConfigID, AUTO_GUID,
            [&](IGameObject* const go) {
                go->SetUInt64(Buff::HostGuidUInt64, entity->Guid());
                go->SetFloat(Buff::ChangeValueFloat, 0);
                if (caster != nullptr)
                {
                    go->SetUInt64(Buff::CasterGuidUInt64, caster->Guid());
                }
                else
                {
                    go->SetUInt64(Buff::CasterGuidUInt64, INVALID_GAMEOBJECT_ID);
                }
                go->SetUInt64(Buff::CreateTimeTickUInt64, SafeSystem::Time::GetMilliSecond());
            }
        );

        //
        if (caster != nullptr)
        {
            auto casterItor = s_CasterEntityBuffs.find(caster->Guid());
            if (casterItor == s_CasterEntityBuffs.end())
            {
                Buffs newBuffs;
                newBuffs._Buffs.insert(buffGo);
                s_CasterEntityBuffs.insert({ caster->Guid(), newBuffs });
            }
            else
            {
                casterItor->second._Buffs.insert(buffGo);
            }
        }

        oAddBuffPush push;
        oGameUnit* buff = new oGameUnit();
        PackageGameObjectToPB(buff, buffGo, false);
        push.set_allocated_buff(buff);

        s_BroadcastTemp.clear();

        if (entity->Is(eGameObjectType::Character)) {
            s_BroadcastTemp.push_back(entity->GetUInt64(Character::AccountUInt64));
        }

        IScene* scene = g_SceneManager->QuerySceneByEntityGuid(entity->Guid());
        if (nullptr == scene) {
            ErrorLog(g_Engine, "can not find any scene for game object %llu", entity->Guid());
            return INVALID_GAMEOBJECT_ID;
        }

        scene->GetVisual(entity->Guid(),
            [&](const std::unordered_set<UInt64>& visual) {
                for (auto visualIt = visual.begin(); visualIt != visual.end(); visualIt++) {
                    IGameObject* g = g_GameWorld->QueryGameObject(*visualIt);
                    if (g && g->Is(eGameObjectType::Character)) {
                        s_BroadcastTemp.push_back(g->GetUInt64(Character::AccountUInt64));
                    }
                }
            }
        );

        g_PlayerManager->Broadcast(s_BroadcastTemp, eServerID::AddBuffPush, push);

        if (buffConfig->Interval > 0) {
            START_TIMER(g_Engine, this, eBuffTimer::BuffBeat, buffConfig->Delay, Api::Unlimited, buffConfig->Interval, buffGo->Guid());
            TraceLog(g_Engine, "Create Buff Time111:: %11llu", SafeSystem::Time::GetMilliSecond());
        }
        else {
            OnBuff(buffGo);
        }

        if (buffConfig->LifeCycle > 0) {
            START_TIMER(g_Engine, this, eBuffTimer::BuffRelease, buffConfig->LifeCycle, 1, buffConfig->LifeCycle, buffGo->Guid());
            TraceLog(g_Engine, "Create Buff Time333:: %11llu", SafeSystem::Time::GetMilliSecond());
        }

        itor->second._Buffs.insert(buffGo);

        return buffGo->Guid();
    }
    else {
        ErrorLog(g_Engine, "can not find buffs of entity %llu type %d", entity->Guid(), entity->Type());
    }

    return INVALID_GAMEOBJECT_ID;
}

void BuffManager::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eBuffTimer::BuffBeat: {
        TraceLog(g_Engine, "Create Buff Time222:: %11llu", SafeSystem::Time::GetMilliSecond());
        IGameObject* buffGo = g_GameWorld->QueryGameObject(context);
        if (buffGo) {
            OnBuff(buffGo);
        }
        else {
            STOP_TIMER(g_Engine, this, eBuffTimer::BuffBeat, context);
            ErrorLog(g_Engine, "Buff logic error, where is buff %llu", context);
        }
        break;
    }
    case eBuffTimer::BuffRelease: {
        TraceLog(g_Engine, "Create Buff Time444:: %11llu", SafeSystem::Time::GetMilliSecond());
        TraceLog(g_Engine, "Release Buff Time:: %11llu", SafeSystem::Time::GetMilliSecond());
        IGameObject* buffGo = g_GameWorld->QueryGameObject(context);
        if (buffGo) {
            ReleaseBuff(buffGo);
        }
        break;
    }
    case eBuffTimer::BuffCleanEntity:

        TraceLog(g_Engine, "OnEnterDeath");
        IGameObject* go = g_GameWorld->QueryGameObject(context);
        if (!go || !go->Is(eGameObjectType::Character)) {
            return;
        }

        auto itor = s_GameEntityBuffs.find(context);
        if (itor != s_GameEntityBuffs.end())
        {
            std::vector<IGameObject*> buffsToRelease;

            for (auto it = itor->second._Buffs.begin(); it != itor->second._Buffs.end();)
            {
                IGameObject* buffGo = *it;
                if (buffGo != nullptr)
                {
                    buffsToRelease.push_back(buffGo);
                    it = itor->second._Buffs.erase(it);
                }
                else
                {
                    it = itor->second._Buffs.erase(it);
                }
            }

            itor->second._StackCounts.clear();

            for (IGameObject* buffGo : buffsToRelease)
            {
                UInt64 buffGuid = buffGo->Guid();
                UInt64 casterGuid = buffGo->GetUInt64(Buff::CasterGuidUInt64);

                if (casterGuid != INVALID_GAMEOBJECT_ID)
                {
                    auto casterBuffItor = s_CasterEntityBuffs.find(casterGuid);
                    if (casterBuffItor != s_CasterEntityBuffs.end())
                    {
                        casterBuffItor->second._Buffs.erase(buffGo);
                        if (casterBuffItor->second._Buffs.empty())
                        {
                            s_CasterEntityBuffs.erase(casterBuffItor);
                        }
                    }
                }

                oReleaseBuffPush push;
                push.set_buffguid(buffGo->Guid());

                s_BroadcastTemp.clear();
                if (go->Is(eGameObjectType::Character)) {
                    s_BroadcastTemp.push_back(go->GetUInt64(Character::AccountUInt64));
                }

                IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
                if (scene) {
                    scene->GetVisual(go->Guid(),
                        [&](const std::unordered_set<UInt64>& visual) {
                            for (auto it = visual.begin(); it != visual.end(); it++) {
                                IGameObject* g = g_GameWorld->QueryGameObject(*it);
                                if (g && g->Is(eGameObjectType::Character)) {
                                    s_BroadcastTemp.push_back(g->GetUInt64(Character::AccountUInt64));
                                }
                            }
                        }
                    );
                }

                g_PlayerManager->Broadcast(s_BroadcastTemp, eServerID::ReleaseBuffPush, push);

                STOP_TIMER(g_Engine, s_Self, eBuffTimer::BuffBeat, buffGuid);
                STOP_TIMER(g_Engine, s_Self, eBuffTimer::BuffRelease, buffGuid);

                OffBuff(buffGo);
                g_GameWorld->ReleaseGameObject(buffGo);
            }
        }



    }
}

void BuffManager::ReleaseBuff(IGameObject* buffGo) {
    UInt64 hostGuid = buffGo->GetUInt64(Buff::HostGuidUInt64);
    UInt64 casterGuid = buffGo->GetUInt64(Buff::CasterGuidUInt64);

    // 从施法者buff列表中移除
    if (casterGuid != INVALID_GAMEOBJECT_ID) {
        auto casterItor = s_CasterEntityBuffs.find(casterGuid);
        if (casterItor != s_CasterEntityBuffs.end()) {
            casterItor->second._Buffs.erase(buffGo);
            if (casterItor->second._Buffs.empty()) {
                s_CasterEntityBuffs.erase(casterItor);
            }
        }
    }

    // 从宿主buff列表中移除
    auto itor = s_GameEntityBuffs.find(hostGuid);
    if (s_GameEntityBuffs.end() != itor) {
        auto it = itor->second._StackCounts.find(buffGo->ConfigID());
        if (itor->second._StackCounts.end() != it) {
            it->second--;
            if (it->second == 0) {
                itor->second._StackCounts.erase(it);
            }
        }
        else {
            ErrorLog(g_Engine, "Buff logic error");
        }

        itor->second._Buffs.erase(buffGo);

        IGameObject* host = g_GameWorld->QueryGameObject(hostGuid);
        oReleaseBuffPush push;
        push.set_buffguid(buffGo->Guid());

        s_BroadcastTemp.clear();

        if (host->Is(eGameObjectType::Character))
        {
            s_BroadcastTemp.push_back(host->GetUInt64(Character::AccountUInt64));
        }

        OffBuff(buffGo);

        IScene* scene = g_SceneManager->QuerySceneByEntityGuid(hostGuid);
        if (nullptr == scene) {
            ErrorLog(g_Engine, "can not find any scene for game object %llu", hostGuid);
            return;
        }

        scene->GetVisual(hostGuid,
            [&](const std::unordered_set<UInt64>& visual) {
                for (auto it = visual.begin(); it != visual.end(); it++) {
                    IGameObject* g = g_GameWorld->QueryGameObject(*it);
                    if (g && g->Is(eGameObjectType::Character)) {
                        s_BroadcastTemp.push_back(g->GetUInt64(Character::AccountUInt64));
                    }
                }
            }
        );

        g_PlayerManager->Broadcast(s_BroadcastTemp, eServerID::ReleaseBuffPush, push);
    }
    else {
        ErrorLog(g_Engine, "Buff logic error");
    }

    STOP_TIMER(g_Engine, s_Self, eBuffTimer::BuffBeat, buffGo->Guid());
    STOP_TIMER(g_Engine, s_Self, eBuffTimer::BuffRelease, buffGo->Guid());
    g_GameWorld->ReleaseGameObject(buffGo);
}

void BuffManager::OffBuff(IGameObject* const buffGo) {
    const AutoConfig::Struct::Buff* buffConfig = g_Config->GetAutoConfig()->QueryBuffConfig(buffGo->ConfigID());
    UInt64 hostGuid = buffGo->GetUInt64(Buff::HostGuidUInt64);
    IGameObject* host = g_GameWorld->QueryGameObject(hostGuid);
    if (!host->Is(eGameObjectType::Entity)) {
        ErrorLog(g_Engine, "GameObject Is Not Entity");
        return;
    }

    switch (buffConfig->Attribute)
    {
    case AutoConfig::Enum::eAttribute::Hp:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::HpFloat, host->GetFloat(Entity::HpFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::MoveSpeed:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::SpeedFloat, host->GetFloat(Entity::SpeedFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
            TraceLog(g_Engine, "Rfresh Speed::%f %f", host->GetFloat(Entity::SpeedFloat), buffGo->GetFloat(Buff::ChangeValueFloat));

        }
        break;
    }
    case AutoConfig::Enum::eAttribute::Blood:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::BloodFloat, host->GetFloat(Entity::BloodFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::AddDefend:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::DefendFloat, host->GetFloat(Entity::DefendFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::AddMagicDefend:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::MagicDefendFloat, host->GetFloat(Entity::MagicDefendFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::ExpandVield:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::VisualFieldFloat, host->GetFloat(Entity::VisualFieldFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::Attack:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::AtkFloat, host->GetFloat(Entity::AtkFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
            TraceLog(g_Engine, "Rfresh Atk::%f %f", host->GetFloat(Entity::AtkFloat), buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::CriticalAtk:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::CriticalDamageFloat, host->GetFloat(Entity::CriticalDamageFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::Critical:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::CriticalFloat, host->GetFloat(Entity::CriticalFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::Mp:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::MpFloat, host->GetFloat(Entity::MpFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::AtkSpeed:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::AtkSpeedFloat, host->GetFloat(Entity::AtkSpeedFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::DamageReduction:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::DamageReductionFloat, host->GetFloat(Entity::DamageReductionFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::ExpAdd:
    {
        if (buffConfig->IfRfresh)
        {
            host->SetFloat(Entity::ExtraAddExpFloat, host->GetFloat(Entity::ExtraAddExpFloat) - buffGo->GetFloat(Buff::ChangeValueFloat));
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::CantController:
    {
        host->SetInt32(Entity::CantControllerRefInt32, host->GetInt32(Entity::CantControllerRefInt32) - 1);
        break;
    }
    case AutoConfig::Enum::eAttribute::Silent:
    {
        host->SetInt32(Entity::SilentRefInt32, host->GetInt32(Entity::SilentRefInt32) - 1);
        break;
    }
    case AutoConfig::Enum::eAttribute::Invincible:
    {
        host->SetInt32(Entity::InvincibleRefInt32, host->GetInt32(Entity::InvincibleRefInt32) - 1);
        break;
    }
    }
}

void BuffManager::OnBuff(IGameObject* const buffGo) {
    const AutoConfig::Struct::Buff* buffConfig = g_Config->GetAutoConfig()->QueryBuffConfig(buffGo->ConfigID());
    UInt64 hostGuid = buffGo->GetUInt64(Buff::HostGuidUInt64);
    IGameObject* host = g_GameWorld->QueryGameObject(hostGuid);
    if (!host->Is(eGameObjectType::Entity)) {
        ErrorLog(g_Engine, "GameObject Is Not Entity");
        return;
    }
    TraceLog(g_Engine, "OnBuff");
    switch (buffConfig->Attribute)
    {
    case AutoConfig::Enum::eAttribute::Hp:
    {
        if (host->GetFloat(Entity::HpFloat) < 0)
        {
            return;
        }
        float changeValue = 0;
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (buffConfig->AddType == AutoConfig::Enum::eAddType::Max)
            {
                changeValue = host->GetFloat(Entity::MaxHpFloat) * buffConfig->EffectParameter;
                float a = host->GetFloat(Entity::HpFloat);
                if (host->SetFloat(Entity::HpFloat, host->GetFloat(Entity::HpFloat) + changeValue))
                {
                    buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + changeValue);
                }
            }
            else if (buffConfig->AddType == AutoConfig::Enum::eAddType::Current)
            {
                changeValue = host->GetFloat(Entity::HpFloat) * buffConfig->EffectParameter;
                if (host->SetFloat(Entity::HpFloat, host->GetFloat(Entity::HpFloat) + changeValue))
                {
                    buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + changeValue);
                }
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            changeValue = buffConfig->EffectParameter;
            if (host->SetFloat(Entity::HpFloat, host->GetFloat(Entity::HpFloat) + changeValue))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + changeValue);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::MoveSpeed:
    {
        float changeValue = 0;
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (buffConfig->AddType == AutoConfig::Enum::eAddType::Current)
            {
                changeValue = host->GetFloat(Entity::SpeedFloat) * buffConfig->EffectParameter;
                if (host->SetFloat(Entity::SpeedFloat, host->GetFloat(Entity::SpeedFloat) + changeValue))
                {
                    buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + changeValue);
                }
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            changeValue = buffConfig->EffectParameter;
            if (host->SetFloat(Entity::SpeedFloat, host->GetFloat(Entity::SpeedFloat) + changeValue))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + changeValue);
            }
        }

        TraceLog(g_Engine, "OnBuff Speed::%f %f", host->GetFloat(Entity::SpeedFloat), buffGo->GetFloat(Buff::ChangeValueFloat));
        break;
    }
    case AutoConfig::Enum::eAttribute::Blood:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::BloodFloat, host->GetFloat(Entity::BloodFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::BloodFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::BloodFloat, host->GetFloat(Entity::BloodFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::AddDefend:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::DefendFloat, host->GetFloat(Entity::DefendFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::DefendFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::DefendFloat, host->GetFloat(Entity::DefendFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::AddMagicDefend:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::MagicDefendFloat, host->GetFloat(Entity::MagicDefendFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::MagicDefendFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::MagicDefendFloat, host->GetFloat(Entity::MagicDefendFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::ExpandVield:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::VisualFieldFloat, host->GetFloat(Entity::VisualFieldFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::VisualFieldFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::VisualFieldFloat, host->GetFloat(Entity::VisualFieldFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::Attack:
    {
        float changeValue = 0;
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            changeValue = host->GetFloat(Entity::AtkFloat) * buffConfig->EffectParameter;
            if (host->SetFloat(Entity::AtkFloat, host->GetFloat(Entity::AtkFloat) + changeValue))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + changeValue);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::AtkFloat, host->GetFloat(Entity::AtkFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::CriticalAtk:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::CriticalDamageFloat, host->GetFloat(Entity::CriticalDamageFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::CriticalDamageFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::CriticalDamageFloat, host->GetFloat(Entity::CriticalDamageFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::Critical:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::CriticalFloat, host->GetFloat(Entity::CriticalFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::CriticalFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::CriticalFloat, host->GetFloat(Entity::CriticalFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::Mp:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::MpFloat, host->GetFloat(Entity::MpFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::MpFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::MpFloat, host->GetFloat(Entity::MpFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::DamageReduction:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::DamageReductionFloat, host->GetFloat(Entity::DamageReductionFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::DamageReductionFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::DamageReductionFloat, host->GetFloat(Entity::DamageReductionFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::AtkSpeed:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::AtkSpeedFloat, host->GetFloat(Entity::AtkSpeedFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::AtkSpeedFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::AtkSpeedFloat, host->GetFloat(Entity::AtkSpeedFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::ExpAdd:
    {
        if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Percent)
        {
            if (host->SetFloat(Entity::ExtraAddExpFloat, host->GetFloat(Entity::ExtraAddExpFloat) * (1 + buffConfig->EffectParameter)))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + host->GetFloat(Entity::ExtraAddExpFloat) * buffConfig->EffectParameter);
            }
        }
        else if (buffConfig->CalculationMethod == AutoConfig::Enum::eCalculationMethod::Normal)
        {
            if (host->SetFloat(Entity::ExtraAddExpFloat, host->GetFloat(Entity::ExtraAddExpFloat) + buffConfig->EffectParameter))
            {
                buffGo->SetFloat(Buff::ChangeValueFloat, buffGo->GetFloat(Buff::ChangeValueFloat) + buffConfig->EffectParameter);
            }
        }
        break;
    }
    case AutoConfig::Enum::eAttribute::CantController:
    {
        host->SetInt32(Entity::CantControllerRefInt32, host->GetInt32(Entity::CantControllerRefInt32) + 1);
        break;
    }
    case AutoConfig::Enum::eAttribute::Silent:
    {
        host->SetInt32(Entity::SilentRefInt32, host->GetInt32(Entity::SilentRefInt32) + 1);
        break;
    }
    case AutoConfig::Enum::eAttribute::Invincible:
    {
        host->SetInt32(Entity::InvincibleRefInt32, host->GetInt32(Entity::InvincibleRefInt32) + 1);
        break;
    }
    }
}

bool BuffManager::OnExitStunedJudge(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2&, const float) {
    IGameObject* const entity = g_GameWorld->QueryGameObject(guid);
    if (state == AutoConfig::Enum::eState::Death) {
        return true;
    }

    if (!entity->Is(eGameObjectType::Entity)) {
        ErrorLog(g_Engine, "GameObject Is Not Entity");
        return false;
    }

    if (entity->GetInt32(Entity::CantControllerRefInt32) > 0 || entity->GetInt32(Entity::RestraintRefInt32) > 0) {
        return false;
    }
    return true;
}

bool BuffManager::OnCantControllerRefChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue) {
    //如果无敌属性 > 0
    return true;
}

void BuffManager::OnCantControllerRefChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue) {
    if (index == Entity::CantControllerRefInt32) {
        if (oldValue == 0) {
            if (newValue > 0) {
                g_Fsm->ForceEnterState(go->Guid(), AutoConfig::Enum::eState::Stuned, go->Position(), go->Angle());
            }
        }
        else if (newValue == 0) {
            g_Fsm->EnterState(go->Guid(), AutoConfig::Enum::eState::Idle, go->Position(), go->Angle());
        }
    }
}

bool BuffManager::OnInvincibleRefChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const Int64 oldValue, const Int64 newValue) {
    //如果无敌属性 > 0
    if (index == Entity::HpFloat)
    {
        if (go->GetInt32(Entity::InvincibleRefInt32) > 0 && newValue < oldValue)
        {
            return false;
        }
    }
    return true;
}

void BuffManager::OnGameEntityCreate(IGameObject* const go)
{
    auto itor = s_GameEntityBuffs.find(go->Guid());
    if (itor == s_GameEntityBuffs.end())
    {
        s_GameEntityBuffs.insert({ go->Guid(), {} });
    }
}

void BuffManager::OnGameEntityRelease(IGameObject* const go)
{
    //清理施法者相关的buff
    auto casterItor = s_CasterEntityBuffs.find(go->Guid());
    if (casterItor != s_CasterEntityBuffs.end())
    {
        for (auto buffIt = casterItor->second._Buffs.begin(); buffIt != casterItor->second._Buffs.end();
            ++buffIt)
        {
            IGameObject* buffGo = *buffIt;
            if (buffGo != nullptr)
            {
                const AutoConfig::Struct::Buff* buffConfig = GetConfig(g_Config, Buff, buffGo->ConfigID());
                if (buffConfig->IsHostDisappear)
                {
                    UInt64 hostGuid = buffGo->GetUInt64(Buff::HostGuidUInt64);
                    auto hostItor = s_GameEntityBuffs.find(hostGuid);
                    if (hostItor != s_GameEntityBuffs.end()) {
                        hostItor->second._Buffs.erase(buffGo);

                        auto stackIt = hostItor->second._StackCounts.find(buffGo->ConfigID());
                        if (stackIt != hostItor->second._StackCounts.end()) {
                            stackIt->second--;
                            if (stackIt->second == 0) {
                                hostItor->second._StackCounts.erase(stackIt);
                            }
                        }

                        OffBuff(buffGo);

                        STOP_TIMER(g_Engine, s_Self, eBuffTimer::BuffBeat, buffGo->Guid());
                        STOP_TIMER(g_Engine, s_Self, eBuffTimer::BuffRelease, buffGo->Guid());

                        // 发送buff移除消息
                        IGameObject* host = g_GameWorld->QueryGameObject(hostGuid);
                        if (host) {
                            oReleaseBuffPush push;
                            push.set_buffguid(buffGo->Guid());

                            s_BroadcastTemp.clear();
                            if (host->Is(eGameObjectType::Character)) {
                                s_BroadcastTemp.push_back(host->GetUInt64(Character::AccountUInt64));
                            }

                            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(hostGuid);
                            if (scene) {
                                scene->GetVisual(hostGuid,
                                    [&](const std::unordered_set<UInt64>& visual) {
                                        for (auto it = visual.begin(); it != visual.end(); it++) {
                                            IGameObject* g = g_GameWorld->QueryGameObject(*it);
                                            if (g && g->Is(eGameObjectType::Character)) {
                                                s_BroadcastTemp.push_back(g->GetUInt64(Character::AccountUInt64));
                                            }
                                        }
                                    }
                                );
                            }

                            g_PlayerManager->Broadcast(s_BroadcastTemp, eServerID::ReleaseBuffPush, push);
                        }

                        g_GameWorld->ReleaseGameObject(buffGo);
                    }
                }
            }
        }
        s_CasterEntityBuffs.erase(casterItor);
    }

    auto itor = s_GameEntityBuffs.find(go->Guid());
    if (itor != s_GameEntityBuffs.end())
    {
        for (auto it = itor->second._Buffs.begin(); it != itor->second._Buffs.end(); )
        {
            IGameObject* buffGo = *it;
            if (buffGo != nullptr)
            {
                // 从施法者列表中移除
                UInt64 casterGuid = buffGo->GetUInt64(Buff::CasterGuidUInt64);
                if (casterGuid != INVALID_GAMEOBJECT_ID) {
                    auto casterBuffItor = s_CasterEntityBuffs.find(casterGuid);
                    if (casterBuffItor != s_CasterEntityBuffs.end()) {
                        casterBuffItor->second._Buffs.erase(buffGo);
                        if (casterBuffItor->second._Buffs.empty()) {
                            s_CasterEntityBuffs.erase(casterBuffItor);
                        }
                    }
                }

                STOP_TIMER(g_Engine, s_Self, eBuffTimer::BuffBeat, buffGo->Guid());
                STOP_TIMER(g_Engine, s_Self, eBuffTimer::BuffRelease, buffGo->Guid());

                g_GameWorld->ReleaseGameObject(buffGo);
                ++it;
            }
            else
            {
                it = itor->second._Buffs.erase(it);
            }
        }

        s_GameEntityBuffs.erase(itor);
    }
}

void BuffManager::OnEnterDeath(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& pos, const float angle)
{
    START_TIMER(g_Engine, s_Self, eBuffTimer::BuffCleanEntity, 100, 1, 0, guid);
}

void BuffManager::OnAppear(IScene* const, const UInt64 guid, const std::vector<UInt64>&)
{

}
void BuffManager::OnDisappear(IScene* const, const UInt64 guid, const std::vector<UInt64>&)
{

}