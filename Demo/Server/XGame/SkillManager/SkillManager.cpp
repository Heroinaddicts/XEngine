#include "SkillManager.h"

Api::iEngine* g_Engine = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
ICharacterManager* g_CharacterManager = nullptr;
ICacheClient* g_CacheClient = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
ISkillBullet* g_SkillBullet = nullptr;
ISkillLauncher* g_SkillLauncher = nullptr;
ISceneManager* g_SceneManager = nullptr;
ISkillMove* g_SkillMove = nullptr;
ISkillDamage* g_SkillDamage = nullptr;

static SkillManager* s_Self = nullptr;

enum  eLauncherTestTimer
{
    Launcher1 = 0,
    Launcher2 = 1,
    Launcher3 = 2
};

static std::unordered_map<UInt64, SkillInfo> s_CharacterSkillMap;

static std::vector<UInt64> s_BroadcastTemp;

bool SkillManager::Initialize(Api::iEngine* const engine) {
    s_BroadcastTemp.reserve(2048);

    g_Engine = engine;
    s_Self = this;
    return true;
}

bool SkillManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_CharacterManager, CharacterManager);
    GetModule(g_Engine, g_CacheClient, CacheClient);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_SkillLauncher, SkillLauncher);
    GetModule(g_Engine, g_SkillMove, SkillMove);
    GetModule(g_Engine, g_Fsm, Fsm);

    g_CharacterManager->RegistCharacterDockerEvent(ICharacterManager::eCharacterDockerEvent::CharacterDockerCreated, SkillManager::OnCharacterDockerLoaded);
    g_CharacterManager->RegistCharacterDockerEvent(ICharacterManager::eCharacterDockerEvent::CharacterDockerLoaded, SkillManager::OnCharacterDockerLoaded);

    g_Fsm->RegistFsmEvent(OnFsmEvent);

    g_PlayerManager->RegistPlayerProtobuf(eClientID::ConfigureSkillSlot, OnConfigureSkillSlot);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::SkillCastReq, OnSkillCastReq);
    return true;
}

void SkillManager::OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState initState) {
    switch (ev) {
    case IFsm::eFsmEvent::FsmCreated: {
        g_Fsm->RegistActionJudge(guid, AutoConfig::Enum::eAction::Casting, SkillManager::OnActionJudge);
        g_Fsm->RegistRunAction(guid, AutoConfig::Enum::eAction::Casting, SkillManager::OnRunAction);
        g_Fsm->RegistEnterJudge(guid, AutoConfig::Enum::eState::Moving, SkillManager::OnEnterMovingJudge);
        break;
    }
    case IFsm::eFsmEvent::FsmReleased: {
        break;
    }
    }
}

bool SkillManager::OnEnterMovingJudge(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& position, const float angle)
{
    IGameObject* entity = g_GameWorld->QueryGameObject(guid);
    if (entity == nullptr)
    {
        return false;
    }


    if (entity->Is(eGameObjectType::Character))
    {
        if (entity->GetInt64(Character::LastSkillConfigIDInt64) == 0)
        {
            return true;
        }

        const AutoConfig::Struct::Skill* skillConfig = GetConfig(g_Config, Skill, entity->GetInt64(Character::LastSkillConfigIDInt64));
        if (skillConfig == nullptr)
        {
            TraceLog(g_Engine, "SkillConfig is not exixt :: %d", entity->GetInt64(Character::LastSkillConfigIDInt64));
            return false;
        }

        if (SafeSystem::Time::GetMilliSecond() - entity->GetUInt64(Character::LastSkillUseTimeUInt64) < skillConfig->CantMoveTime)
        {
            return false;
        }
    }

    return true;
}

void SkillManager::OnRunAction(const UInt64 guid, const AutoConfig::Enum::eAction action, const Int64 actionConfigID, const Vector2& position, const float angle)
{

}

bool SkillManager::OnActionJudge(const UInt64 guid, const AutoConfig::Enum::eAction action, const Int64 actionConfigID, const Vector2& position, const float angle)
{
    IGameObject* go = g_GameWorld->QueryGameObject(guid);
    if (go == nullptr)
    {
        return false;
    }

    if (g_Fsm->GetState(go->Guid()) == AutoConfig::Enum::eState::Death)
    {
        return false;
    }

    if (go->GetInt32(Entity::CantControllerRefInt32) > 0 || go->GetInt32(Entity::SilentRefInt32) > 0)
    {
        return false;
    }

    return true;
}

bool SkillManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void SkillManager::OnCharacterDockerLoaded(ICharacterDocker* const docker, IGameObject* const gameObject) {
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterCreated, SkillManager::OnCharacterCreated);
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterLoaded, SkillManager::OnCharacterLoaded);
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterRelease, SkillManager::OnCharacterRelease);
}

void SkillManager::OnCharacterCreated(ICharacterDocker* const, IGameObject* const character) {
    const AutoConfig::Struct::Character* characterConfig = GetConfig(g_Config, Character, character->ConfigID());
    if (nullptr == characterConfig) {
        ErrorLog(g_Engine, "Error Character Config Id %lld", character->ConfigID());
        return;
    }

    SkillInfo info(character->Guid());
    oSkillInfoPush push;
    push.set_characterid(character->Guid());
    for (int i = 0; i < characterConfig->InitialSkill.size(); i++) {
        const AutoConfig::Struct::Skill* skillConfig = GetConfig(g_Config, Skill, characterConfig->InitialSkill[i]);
        if (nullptr == skillConfig) {
            ErrorLog(g_Engine, "Error Skill Config Id %lld", characterConfig->InitialSkill[i]);
            return;
        }

        const UInt64 account = character->GetUInt64(Character::AccountUInt64);

        oSkillInfo* skillInfo = push.add_skills();
        IGameObject* skillGo = g_GameWorld->CreateGameObject(eGameObjectType::Skill, skillConfig->ID, AUTO_GUID,
            [&](IGameObject* const go) {
                go->SetUInt64(Skill::AccountUInt64, character->GetUInt64(Character::AccountUInt64));
                go->SetUInt64(Skill::CharacterIDUInt64, character->Guid());
            }
        );

        skillInfo->set_skillguid(skillGo->Guid());
        skillInfo->set_skillconfigid(skillGo->ConfigID());
        skillInfo->set_skillslot(skillGo->GetString(Skill::SkillSlotString));

        g_CacheClient->Create(account, eTable::t_skill,
            [&](ISQLCacheRow* row) {
                row->GetColumn(t_skill::Guid__UInt64__PrimaryUnique)->SetUInt64(skillGo->Guid());
                row->GetColumn(t_skill::Account__UInt64__)->SetUInt64(account);
                row->GetColumn(t_skill::CharacterID__UInt64__)->SetUInt64(character->Guid());
                row->GetColumn(t_skill::ConfigID__Int64__)->SetInt64(skillConfig->ID);
            },
            [=](const bool succeed, const ISQLCacheTable* const tab) {
                if (succeed) {
                    TraceLog(g_Engine, "Initialize Character %llu Skill %llu To Database succeed", character->Guid(), skillGo->Guid());
                }
                else {
                    ErrorLog(g_Engine, "Initialize Character %llu Skill %llu To Database error", character->Guid(), skillGo->Guid());
                }
            }
        );

        info._SkillMap.insert({ characterConfig->InitialSkill[i], skillGo });
    }

    g_CharacterManager->SendProtobuf(character->Guid(), eServerID::SkillInfoPush, push);
    info._IsLoaded = true;
    s_CharacterSkillMap.insert({ character->Guid(), info });
}

void SkillManager::OnCharacterLoaded(ICharacterDocker* const, IGameObject* const character) {
    auto itor = s_CharacterSkillMap.find(character->Guid());
    if (s_CharacterSkillMap.end() != itor) {
        if (itor->second._IsLoaded) {
            oSkillInfoPush push;
            push.set_characterid(character->Guid());
            for (auto it = itor->second._SkillMap.begin(); it != itor->second._SkillMap.end(); it++) {
                oSkillInfo* skillInfo = push.add_skills();
                skillInfo->set_skillguid(it->second->Guid());
                skillInfo->set_skillconfigid(it->second->ConfigID());
                skillInfo->set_skillslot(it->second->GetString(Skill::SkillSlotString));
                skillInfo->set_lastcasttick(it->second->GetUInt64(Skill::LastCastTickUInt64));
            }

            g_CharacterManager->SendProtobuf(character->Guid(), eServerID::SkillInfoPush, push);
        }

        return;
    }

    SkillInfo info(character->Guid());
    info._IsLoaded = false;
    s_CharacterSkillMap.insert({ character->Guid(), info });

    const UInt64 account = character->GetUInt64(Character::AccountUInt64);
    const UInt64 characterId = character->Guid();
    g_CacheClient->Query(account, eTable::t_skill, t_skill::CharacterID__UInt64__, character->Guid(),
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (!succeed) {
                ErrorLog(g_Engine, "g_CacheClient->Query %llu eTable::t_skill error", character->Guid());
                return;
            }

            auto itor = s_CharacterSkillMap.find(characterId);
            if (s_CharacterSkillMap.end() == itor) {
                return;
            }

            oSkillInfoPush push;
            push.set_characterid(character->Guid());

            tab->Query(t_skill::CharacterID__UInt64__, characterId,
                [&](ISQLCacheRow* const row) {
                    IGameObject* skillGo = g_GameWorld->CreateGameObject(
                        eGameObjectType::Skill,
                        row->GetColumn(t_skill::ConfigID__Int64__)->GetInt64(),
                        row->GetColumn(t_skill::Guid__UInt64__PrimaryUnique)->GetUInt64(),
                        [&](IGameObject* const go) {
                            go->SetUInt64(Skill::AccountUInt64, row->GetColumn(t_skill::Account__UInt64__)->GetUInt64());
                            go->SetUInt64(Skill::CharacterIDUInt64, row->GetColumn(t_skill::CharacterID__UInt64__)->GetUInt64());
                            go->SetString(Skill::SkillSlotString, row->GetColumn(t_skill::SkillSlot__String__)->GetString());
                            go->SetUInt64(Skill::LastCastTickUInt64, row->GetColumn(t_skill::LastCastTick__UInt64__)->GetUInt64());
                        }
                    );

                    oSkillInfo* skillInfo = push.add_skills();
                    skillInfo->set_skillguid(skillGo->Guid());
                    skillInfo->set_skillconfigid(skillGo->ConfigID());
                    skillInfo->set_skillslot(skillGo->GetString(Skill::SkillSlotString));
                    skillInfo->set_lastcasttick(skillGo->GetUInt64(Skill::LastCastTickUInt64));
                    itor->second._SkillMap.insert({ skillGo->ConfigID(), skillGo });
                    TraceLog(g_Engine,
                        "CharacterID %llu, SkillGuid %llu, SkillAccount %llu, SkillConfig %lld, Slot %s",
                        skillGo->GetUInt64(Skill::CharacterIDUInt64),
                        skillGo->Guid(),
                        skillGo->GetUInt64(Skill::AccountUInt64),
                        skillGo->ConfigID(),
                        skillGo->GetString(Skill::SkillSlotString).c_str()
                    );
                }
            );

            g_CharacterManager->SendProtobuf(characterId, eServerID::SkillInfoPush, push);
            itor->second._IsLoaded = true;
        }
    );
}

void SkillManager::OnCharacterRelease(ICharacterDocker* const docker, IGameObject* const character) {
    auto itor = s_CharacterSkillMap.find(character->Guid());
    XASSERT(s_CharacterSkillMap.end() != itor, "wtf");
    if (s_CharacterSkillMap.end() != itor) {
        if (itor->second._IsLoaded) {
            for (auto it = itor->second._SkillMap.begin(); it != itor->second._SkillMap.end(); it++) {
                IGameObject* skillGo = it->second;
                g_GameWorld->ReleaseGameObject(skillGo);
            }
        }

        s_CharacterSkillMap.erase(itor);
    }
}

void SkillManager::OnConfigureSkillSlot(IPlayer* const player, const UInt64 account, const oConfigureSkillSlot& pb) {
    IGameObject* skillGo = GetSkillGameObject(pb.characterid(), pb.skillconfigid());

    if (skillGo && skillGo->GetUInt64(Skill::AccountUInt64) == account) {
        skillGo->SetString(Skill::SkillSlotString, pb.slot());
        SaveSkill(skillGo);
    }
}

void SkillManager::SkillCast(IGameObject* const cast, const UInt64 skillGuid, const Int64 skillConfigId, const float angle, const Vector2& targetPos, const Vector2& placePos, const UInt64 targetGuid)
{
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(cast->Guid());

    if (cast->Is(eGameObjectType::Character))
    {
        const AutoConfig::Struct::Skill* skillConfig = GetConfig(g_Config, Skill, skillConfigId);
        if (skillConfig == nullptr)
        {
            TraceLog(g_Engine, "SkillConfig is not exixt :: %d", skillConfigId);
            return;
        }
        for (int i = 0; i < skillConfig->SkillEffect.size(); i++)
        {
            if (skillConfig->SkillEffect[i] == AutoConfig::Enum::eSkillEffect::Launcher)
            {
                if (g_SkillLauncher->AddLauncher(cast, skillConfig->SkillEffectConfigID[i], angle, targetPos, placePos, targetGuid) == INVALID_GAMEOBJECT_ID)
                {
                    return;
                }
            }
            else if (skillConfig->SkillEffect[i] == AutoConfig::Enum::eSkillEffect::Move)
            {
                if (g_SkillMove->AddMove(cast, 0, skillConfigId, skillConfig->SkillEffectConfigID[i], angle, targetGuid) == INVALID_GAMEOBJECT_ID)
                {
                    return;
                }
            }
        }
    }
    else
    {
        const AutoConfig::Struct::MonsterSkill* monsterSkillConfig = GetConfig(g_Config, MonsterSkill, skillConfigId);
        if (monsterSkillConfig == nullptr)
        {
            TraceLog(g_Engine, "SkillConfig is not exixt :: %d", skillConfigId);
            return;
        }

        for (int i = 0; i < monsterSkillConfig->SkillEffect.size(); i++)
        {
            if (monsterSkillConfig->SkillEffect[i] == AutoConfig::Enum::eSkillEffect::Launcher)
            {
                cast->SetBool(Entity::CastIsTrackOnCollisionBool, monsterSkillConfig->SelfIsTrack);
                if (g_SkillLauncher->AddLauncher(cast, monsterSkillConfig->SkillEffectConfigID[i], angle, targetPos, placePos, targetGuid) == INVALID_GAMEOBJECT_ID)
                {
                    return;
                }
            }
            else if (monsterSkillConfig->SkillEffect[i] == AutoConfig::Enum::eSkillEffect::Move)
            {
                if (g_SkillMove->AddMove(cast, 0, skillConfigId, monsterSkillConfig->SkillEffectConfigID[i], angle, targetGuid) == INVALID_GAMEOBJECT_ID)
                {
                    return;
                }
            }
        }
    }

    oSkillCastPush push;
    push.set_castguid(cast->Guid());
    push.set_skillguid(skillGuid);
    push.set_skillconfigid(skillConfigId);
    push.set_angle(angle);
    oVector2* pos = push.mutable_targetpos();
    pos->set_x(targetPos.x);
    pos->set_y(targetPos.y);
    oVector2* pos2 = push.mutable_placepos();
    pos2->set_x(placePos.x);
    pos2->set_y(placePos.y);
    push.set_targetguid(targetGuid);
    push.set_startcasttick(SafeSystem::Time::GetMilliSecond());

    s_BroadcastTemp.clear();
    if (cast->Is(eGameObjectType::Character)) {
        s_BroadcastTemp.push_back(cast->GetUInt64(Character::AccountUInt64));
    }
    scene->GetVisual(cast->Guid(),
        [&](const std::unordered_set<UInt64>& visual) {
            for (auto visualIt = visual.begin(); visualIt != visual.end(); visualIt++) {
                IGameObject* g = g_GameWorld->QueryGameObject(*visualIt);
                if (g && g->Is(eGameObjectType::Character)) {
                    s_BroadcastTemp.push_back(g->GetUInt64(Character::AccountUInt64));
                }
            }
        }
    );

    g_PlayerManager->Broadcast(s_BroadcastTemp, eServerID::SkillCastPush, push);
}

void SkillManager::OnSkillCastReq(IPlayer* const player, const UInt64 account, const oSkillCastReq& pb) {

    IGameObject* character = g_GameWorld->QueryGameObject(player->GetDeployCharacterID());
    const AutoConfig::Struct::Skill* skillConfig = GetConfig(g_Config, Skill, pb.skillconfigid());
    if (skillConfig == nullptr)
    {
        TraceLog(g_Engine, "SkillConfig is not exixt :: %d", pb.skillconfigid());
        return;
    }

    IGameObject* skillGo = GetSkillGameObject(character->Guid(), pb.skillconfigid());
    if (skillGo == nullptr)
    {
        TraceLog(g_Engine, "Character Has Not Skill::%d", pb.skillconfigid());
        return;
    }

    if (skillConfig->Consume == AutoConfig::Enum::eConsumeType::Mp && character->GetFloat(Character::MpFloat) < skillConfig->ConsumeCount)
    {
        TraceLog(g_Engine, "Resource Mp is not enough %f", character->GetFloat(Character::MpFloat));
        return;
    }
    character->SetFloat(Character::MpFloat, character->GetFloat(Character::MpFloat) - skillConfig->ConsumeCount);

    if (skillConfig->IsNromalAtk)
    {
        const AutoConfig::Struct::Character* characterConfig = GetConfig(g_Config, Character, character->ConfigID());

        if (pb.skillconfigid() == characterConfig->Atk1)
        {
            IGameObject* atk3Go = GetSkillGameObject(player->GetDeployCharacterID(), characterConfig->Atk3);
            if (SafeSystem::Time::GetMilliSecond() - atk3Go->GetUInt64(Skill::LastCastTickUInt64) < (UInt64)characterConfig->InitialAtkSpeed3)
            {
                TraceLog(g_Engine, "NormalAtk1 Is Not Invalid %llu, %llu, %llu, %llu", player->GetDeployCharacterID(), SafeSystem::Time::GetMilliSecond(), atk3Go->GetUInt64(Skill::LastCastTickUInt64), (UInt64)characterConfig->InitialAtkSpeed3);
                return;
            }
        }
        else if (pb.skillconfigid() == characterConfig->Atk2)
        {
            IGameObject* atk1Go = GetSkillGameObject(player->GetDeployCharacterID(), characterConfig->Atk1);
            if (SafeSystem::Time::GetMilliSecond() - atk1Go->GetUInt64(Skill::LastCastTickUInt64) > characterConfig->AtkInteval || SafeSystem::Time::GetMilliSecond() - atk1Go->GetUInt64(Skill::LastCastTickUInt64) < (UInt64)characterConfig->InitialAtkSpeed1)
            {
                TraceLog(g_Engine, "NormalAtk2 Is Not Invalid");
                return;
            }
        }
        else if (pb.skillconfigid() == characterConfig->Atk3)
        {
            IGameObject* atk2Go = GetSkillGameObject(player->GetDeployCharacterID(), characterConfig->Atk2);
            if (SafeSystem::Time::GetMilliSecond() - atk2Go->GetUInt64(Skill::LastCastTickUInt64) > characterConfig->AtkInteval || SafeSystem::Time::GetMilliSecond() - atk2Go->GetUInt64(Skill::LastCastTickUInt64) < (UInt64)characterConfig->InitialAtkSpeed2)
            {
                TraceLog(g_Engine, "NormalAtk3 Is Not Invalid");
                return;
            }
        }
    }
    else
    {
        if (SafeSystem::Time::GetMilliSecond() - skillGo->GetUInt64(Skill::LastCastTickUInt64) < skillConfig->CoolDown)
        {
            TraceLog(g_Engine, "Skill Is CoolingDown :: %d", pb.skillconfigid());
            return;
        }
    }

    Vector2 position(pb.castposition().x(), pb.castposition().y());

    //矫正前后端放技能位置
    if ((character->Position() - position).Length() > AutoConfig::Const::AllowableError::PositionErrorRange) {
        position = character->Position();
        oCorrectPosition ocp;
        oVector2* pos = ocp.mutable_position();
        pos->set_x(character->Position().x);
        pos->set_y(character->Position().y);

        g_PlayerManager->SendProtobufToPlayer(account, eServerID::CorrectPosition, ocp);
    }

    character->SetPosition(position);

    //矫正客户端攻击位置目标
    if (skillConfig->SelectedType == AutoConfig::Enum::eSelectedType::Aim)
    {
        IGameObject* target = g_GameWorld->QueryGameObject(pb.targetguid());
        if (target == nullptr)
        {
            TraceLog(g_Engine, "Target Is Nor Exist");
            return;
        }

        Vector2 dir = target->Position() - character->Position();
        if (dir.Length() > skillConfig->MaxSkillDistacne + AutoConfig::Const::GameSetting::Skill)
        {
            TraceLog(g_Engine, "Distance Is Out Max::%f", dir.Length());
            return;
        }
    }
    else if (skillConfig->SelectedType == AutoConfig::Enum::eSelectedType::UnAim && skillConfig->CanSeparated == true)
    {
        Vector2 dir = Vector2(pb.placepos().x(), pb.placepos().y()) - character->Position();
        if (dir.Length() > skillConfig->MaxSkillDistacne + AutoConfig::Const::GameSetting::Skill)
        {
            TraceLog(g_Engine, "Distance Is Out Max::%f", dir.Length());
            return;
        }
    }

    if (g_Fsm->RunAction(player->GetDeployCharacterID(), AutoConfig::Enum::eAction::Casting, pb.skillconfigid(), character->Position(), pb.angle()))
    {
        skillGo->SetUInt64(Skill::LastCastTickUInt64, SafeSystem::Time::GetMilliSecond());

        g_Fsm->EnterState(character->Guid(), AutoConfig::Enum::eState::Idle, character->Position(), character->Angle(), false);

        if (!skillConfig->CanMove)
        {
            character->SetInt64(Character::LastSkillConfigIDInt64, pb.skillconfigid());
            character->SetUInt64(Character::LastSkillUseTimeUInt64, SafeSystem::Time::GetMilliSecond());
        }
        s_Self->SkillCast(character, pb.skillguid(), pb.skillconfigid(), pb.angle(), Vector2(pb.targetpos().x(), pb.targetpos().y()), Vector2(pb.placepos().x(), pb.placepos().y()), pb.targetguid());
    }
}

IGameObject* SkillManager::GetSkillGameObject(const UInt64 characterId, const Int64 skillConfigId) {
    auto itor = s_CharacterSkillMap.find(characterId);
    if (s_CharacterSkillMap.end() == itor) {
        return nullptr;
    }

    auto it = itor->second._SkillMap.find(skillConfigId);
    if (itor->second._SkillMap.end() != it) {
        return it->second;
    }

    return nullptr;
}

void SkillManager::SaveSkill(IGameObject* skillGo) {
    g_CacheClient->Update(
        skillGo->GetUInt64(Skill::AccountUInt64),
        eTable::t_skill,
        t_skill::Guid__UInt64__PrimaryUnique,
        skillGo->Guid(),
        [&](ISQLCacheRow* const row) {
            row->GetColumn(t_skill::ConfigID__Int64__)->SetInt64(skillGo->ConfigID());
            row->GetColumn(t_skill::SkillSlot__String__)->SetString(skillGo->GetString(Skill::SkillSlotString));
            row->GetColumn(t_skill::LastCastTick__UInt64__)->SetUInt64(skillGo->GetUInt64(Skill::LastCastTickUInt64));
        },
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (succeed) {
                TraceLog(g_Engine, "Update Skill %llu to database succeed");
            }
            else {
                ErrorLog(g_Engine, "Update Skill %llu to database faild");
            }
        }
    );
}
