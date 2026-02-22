#include "CharacterManager.h"
#include "CharacterDocker.h"

Api::iEngine* g_Engine = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
ICacheClient* g_CacheClient = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
ISceneManager* g_SceneManager = nullptr;
IFsm* g_Fsm = nullptr;
IBackpack* g_Backpack = nullptr;

std::vector<UInt64> g_BroadcastTemp;
static CharacterManager* s_Self = nullptr;
XEventPool<ICharacterManager::eCharacterDockerEvent, ICharacterDocker*, IGameObject*> g_CharacterDockerEvent;
std::unordered_map<UInt64, UInt64> s_PlayerSwitchCharacterTick;

enum eTimerID
{
    CharacterBack = 0,
    CharacterHpRecover = 1,
    CharacterMpRecover = 2,
};

bool CharacterManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    g_BroadcastTemp.resize(2048);
    return true;
}

bool CharacterManager::Launch(Api::iEngine* const engine) {
    GetModule(engine, g_Config, Config);
    GetModule(engine, g_GameWorld, GameWorld);
    GetModule(engine, g_CacheClient, CacheClient);
    GetModule(engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_Fsm, Fsm);
    GetModule(g_Engine, g_Backpack, Backpack);

    g_GameWorld->RegistGameObjectAttributeChangeJudge(eGameObjectType::Character, 
        [](IGameObject* const character, const int index, const bool sync, const bool share, const float oldValue, const float newValue) {
            if (index == Character::HpFloat || index == Character::MpFloat) {
                if (g_SceneManager->QuerySceneByEntityGuid(character->Guid())->Config()->SceneType == AutoConfig::Enum::eSceneType::Public) {
                    if (oldValue > newValue) {
                        return false;
                    }
                }
            }

            return true;
        }
    );

    g_PlayerManager->RegistPlayerEvent(IPlayerManager::ePlayerEvent::NewPlayerCreated, OnNewPlayerCreated);
    g_PlayerManager->RegistPlayerEvent(IPlayerManager::ePlayerEvent::PlayerLoading, OnPlayerLoading);
    g_PlayerManager->RegistPlayerEvent(IPlayerManager::ePlayerEvent::PlayerRelease, OnPlayerRelease);

    g_PlayerManager->RegistPlayerProtobuf(eClientID::StateChange, OnStateChange);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::EquipReq, OnEquipReq);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::UnEquipReq, OnUnEquipReq);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::EnterCopySceneTest, OnEnterCopySceneTest);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::SwitchCharacterReq, OnSwitchCharacterReq);

    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Create,
        [](IScene* const scene) {
            scene->RegistAoiEvent(IScene::eAoiEvent::Appear, CharacterManager::OnAppear);
            scene->RegistAoiEvent(IScene::eAoiEvent::Disappear, CharacterManager::OnDisappear);
        }
    );

    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Character, OnCharacterHpChanged);
    g_GameWorld->RegistGameObjectAttributeChangeJudge(eGameObjectType::Character, OnCharacterHPChangeJudge);

    g_Fsm->RegistFsmEvent(OnFsmEvent);
    return true;
}

void CharacterManager::OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState initState) {
    switch (ev) {
    case IFsm::eFsmEvent::FsmCreated: {
        g_Fsm->RegistExitJudge(guid, AutoConfig::Enum::eState::Death, CharacterManager::OnExitDeathJudge);
        break;
    }
    case IFsm::eFsmEvent::FsmReleased: {
        break;
    }
    }
}

bool CharacterManager::OnExitDeathJudge(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2&, const float) {

    IGameObject* const entity = g_GameWorld->QueryGameObject(guid);
    if (!entity->Is(eGameObjectType::Entity)) {
        ErrorLog(g_Engine, "GameObject Is Not Entity");
        return false;
    }

    if (entity->GetFloat(Entity::HpFloat) <= 0)
    {
        return false;
    }

    return true;
}

bool CharacterManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void CharacterManager::OnNewPlayerCreated(IPlayer* const player, const UInt64 account) {
    CharacterDocker* docker = CharacterDocker::CreateCharacterDocker(account);

	g_CharacterDockerEvent.Trigger(eCharacterDockerEvent::CharacterDockerLoaded, docker, nullptr);
    std::vector<IGameObject*> temp;
    for (int i = 0; i < AutoConfig::Const::Character::InitializeCharacterConfigID.size(); i++) {
        temp.push_back(docker->__CreateCharacters(AutoConfig::Const::Character::InitializeCharacterConfigID[i]));
    }
	docker->SendCharacterInfosToClient();

    for (int i = 0; i < temp.size(); i++) {
		if (i == 0)
            docker->TriggerEvent(ICharacterDocker::eCharacterEvent::DefaultCharacterCreated, temp[i]);
        docker->TriggerEvent(ICharacterDocker::eCharacterEvent::CharacterCreated, temp[i]);
    }

    const AutoConfig::Struct::Character* characterConfig = GetConfig(g_Config, Character, docker->GetDeployCharacter()->ConfigID());
    START_TIMER(g_Engine, s_Self, eTimerID::CharacterHpRecover, characterConfig->HpReserveSpeed, Api::Unlimited, characterConfig->HpReserveSpeed, docker->GetDeployCharacter()->Guid());
    START_TIMER(g_Engine, s_Self, eTimerID::CharacterMpRecover, characterConfig->MpReserveSpeed, Api::Unlimited, characterConfig->MpReserveSpeed, docker->GetDeployCharacter()->Guid());
}

void CharacterManager::OnTimer(const int id, const UInt64 context, const Int64 timestamp)
{
    IGameObject* character = g_GameWorld->QueryGameObject(context);
    if (character == nullptr)
    {
        ErrorLog(g_Engine, "Character is Not Exist :: %llu", context);
        return;
    }
    switch (id) {
    case eTimerID::CharacterBack: {
        const Int64 lastPublicSceneConfigId = character->GetInt64(Entity::LastPublicSceneConfigIDInt64);
        IScene* scene = g_SceneManager->QueryPublicScene(lastPublicSceneConfigId);
        if (nullptr == scene) {
            ErrorLog(g_Engine, "Can not find Scene %lld", lastPublicSceneConfigId);
            scene = g_SceneManager->QueryPublicScene(AutoConfig::Const::World::DefaultWorld);
        }
        s_Self->EnterScene(character, scene);
        break;
    }
    case eTimerID::CharacterHpRecover:
    {
        float newHp = std::min(character->GetFloat(Character::HpFloat) + character->GetFloat(Character::HpRecoverFloat), character->GetFloat(Character::MaxHpFloat));

        character->SetFloat(Character::HpFloat, newHp);
        break;
    }
    case eTimerID::CharacterMpRecover:
    {
        float newMp = std::min(character->GetFloat(Character::MpFloat) + character->GetFloat(Character::MpRecoverFloat), character->GetFloat(Character::MaxMpFloat));

        character->SetFloat(Character::MpFloat, newMp);
        break;
    }
    }
}

void CharacterManager::OnPlayerLoading(IPlayer* const player, const UInt64 account) {
    CharacterDocker* docker = CharacterDocker::QueryCharacterDocker(account);
    if (docker) {
        IGameObject* deployCharacter = docker->GetDeployCharacter();
        if (deployCharacter) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(deployCharacter->Guid());
            XASSERT(scene, "wtf");

			oSceneInfoPush sceneInfoPush;
			sceneInfoPush.mutable_info()->set_configid(scene->Config()->ID);
			sceneInfoPush.mutable_info()->set_sceneid(scene->SceneID());
			g_PlayerManager->SendProtobufToPlayer(docker->_Account, eServerID::SceneInfoPush, sceneInfoPush);

			docker->SendCharacterInfosToClient();

			oSwitchCharacterPush switchPush;
			switchPush.set_characterid(deployCharacter->Guid());
			switchPush.set_x(deployCharacter->Position().x);
			switchPush.set_y(deployCharacter->Position().y);
			g_PlayerManager->SendProtobufToPlayer(docker->_Account, eServerID::SwitchCharacterPush, switchPush);

            scene->GetVisual(deployCharacter->Guid(),
                [&](const std::unordered_set<UInt64>& others) {
                    oGameObjectAppearPush appearPush;
                    for (auto it = others.begin(); it != others.end(); it++) {
                        IGameObject* const go = g_GameWorld->QueryGameObject(*it);
                        XASSERT(go, "wtf");
                        if (go) {
                            oGameUnit* info = appearPush.add_gos();
                            PackageGameObjectToPB(info, go, true);
                        }
                    }

                    player->SendProtobuf(eServerID::GameObjectAppearPush, appearPush);
                }
            );
            const AutoConfig::Struct::Character* characterConfig = GetConfig(g_Config, Character, docker->GetDeployCharacter()->ConfigID());
            START_TIMER(g_Engine, s_Self, eTimerID::CharacterHpRecover, characterConfig->HpReserveSpeed, Api::Unlimited, characterConfig->HpReserveSpeed, docker->GetDeployCharacter()->Guid());
            START_TIMER(g_Engine, s_Self, eTimerID::CharacterMpRecover, characterConfig->MpReserveSpeed, Api::Unlimited, characterConfig->MpReserveSpeed, docker->GetDeployCharacter()->Guid());
        }
        else {
            docker->SendCharacterInfosToClient();
        }
        docker->TriggerLoaded();
    }
    else {
        docker = CharacterDocker::CreateCharacterDocker(account);
        docker->Load(
            [=](const bool succeed) {
                IGameObject* character = g_GameWorld->QueryGameObject(player->GetDeployCharacterID());
                const AutoConfig::Struct::Character* characterConfig = GetConfig(g_Config, Character, character->ConfigID());
                START_TIMER(g_Engine, s_Self, eTimerID::CharacterHpRecover, characterConfig->HpReserveSpeed, Api::Unlimited, characterConfig->HpReserveSpeed, player->GetDeployCharacterID());
                START_TIMER(g_Engine, s_Self, eTimerID::CharacterMpRecover, characterConfig->MpReserveSpeed, Api::Unlimited, characterConfig->MpReserveSpeed, player->GetDeployCharacterID());
            }
        );
    }
}

void CharacterManager::OnPlayerRelease(IPlayer* const player, const UInt64 account) {
    CharacterDocker* docker = CharacterDocker::QueryCharacterDocker(account);

    auto itor = s_PlayerSwitchCharacterTick.find(account);
    if (itor != s_PlayerSwitchCharacterTick.end())
    {
        s_PlayerSwitchCharacterTick.erase(account);
    }

    if (docker) {
        IGameObject* character = docker->GetDeployCharacter();
        XASSERT(character, "wtf");
        if (character) {
            STOP_TIMER(g_Engine, s_Self, eTimerID::CharacterBack, character->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::CharacterHpRecover, character->Guid());
            STOP_TIMER(g_Engine, s_Self, eTimerID::CharacterMpRecover, character->Guid());
            //to do  这里要补充如果在副本中 要先退出副本
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(character->Guid());
            XASSERT(scene, "wtf");
            if (scene) {
                scene->ExitScene(character->Guid());
            }
        }

        docker->SaveDocker();
        s_Self->ReleaseCharacterDocker(account);
    }
}

void CharacterManager::OnStateChange(IPlayer* const player, const UInt64 account, const oStateChange& pb) {
    if (pb.state() < 0 || pb.state() >= (int)AutoConfig::Enum::eState::__Count__) {
        XASSERT(false, "wtf");
        return;
    }

    ICharacterDocker* docker = s_Self->QueryCharacterDocker(account);
    XASSERT(docker, "wtf");
    if (nullptr == docker) {
        ErrorLog(g_Engine, "Where is account %llu %s's docker", account, player->Nickname().c_str());
        return;
    }

    IGameObject* deployCharacter = docker->GetDeployCharacter();
    if (nullptr == deployCharacter) {
        ErrorLog(g_Engine, "Account %lld %s never deploy a character, system error", account, player->Nickname().c_str());
        return;
    }

    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(deployCharacter->Guid());
    if (nullptr == scene) {
        ErrorLog(g_Engine, "Query Scene on character %llu error", deployCharacter->Guid());
        return;
    }

    Vector2 position(pb.pos().x(), pb.pos().y());
    if (!scene->CanMove(position)) {
        DEBUG(g_Engine, "CharacterManager::OnStateChange Pos %f, %f can not move", pb.pos().x(), pb.pos().y());
        return;
    }

    if ((deployCharacter->Position() - position).Length() > AutoConfig::Const::AllowableError::PositionErrorRange) {
        position = deployCharacter->Position();
        oCorrectPosition ocp;
        oVector2* pos = ocp.mutable_position();
        pos->set_x(deployCharacter->Position().x);
        pos->set_y(deployCharacter->Position().y);

        g_PlayerManager->SendProtobufToPlayer(account, eServerID::CorrectPosition, ocp);
    }

    g_Fsm->EnterState(deployCharacter->Guid(), (AutoConfig::Enum::eState)pb.state(), position, (float)pb.angle());
}

void CharacterManager::OnSwitchCharacterReq(IPlayer* const player, const UInt64 account, const oSwitchCharacterReq& test)
{
    CharacterDocker* docker = CharacterDocker::QueryCharacterDocker(account);
    if (nullptr == docker) {
        ErrorLog(g_Engine, "Where is account %llu %s's docker", account, player->Nickname().c_str());
        return;
    }

    auto itor = s_PlayerSwitchCharacterTick.find(account);
    if (itor == s_PlayerSwitchCharacterTick.end())
    {
        s_PlayerSwitchCharacterTick.insert({ account, 0 });
    }

    itor = s_PlayerSwitchCharacterTick.find(account);
    if (SafeSystem::Time::GetMilliSecond() - itor->second <= AutoConfig::Const::GameSetting::ChangeCharacter)
    {
        TraceLog(g_Engine, "Cant SwicthCharacter in %llu ms", AutoConfig::Const::GameSetting::ChangeCharacter);
        return;
    }
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(docker->GetDeployCharacter()->Guid());
    if (scene->Config()->SceneType == AutoConfig::Enum::eSceneType::BattleCopy)
    {
        TraceLog(g_Engine, "Cant SwicthCharacter In Battle Copy Scene");
        return;
    }


    itor->second = SafeSystem::Time::GetMilliSecond();

    STOP_TIMER(g_Engine, s_Self, eTimerID::CharacterBack, docker->GetDeployCharacter()->Guid());
    STOP_TIMER(g_Engine, s_Self, eTimerID::CharacterHpRecover, docker->GetDeployCharacter()->Guid());
    STOP_TIMER(g_Engine, s_Self, eTimerID::CharacterMpRecover, docker->GetDeployCharacter()->Guid());

    IGameObject* character = docker->DeployCharacter(test.characterid());
    const AutoConfig::Struct::Character* characterConfig = GetConfig(g_Config, Character, character->ConfigID());
    START_TIMER(g_Engine, s_Self, eTimerID::CharacterHpRecover, characterConfig->HpReserveSpeed, Api::Unlimited, characterConfig->HpReserveSpeed, character->Guid());
    START_TIMER(g_Engine, s_Self, eTimerID::CharacterMpRecover, characterConfig->MpReserveSpeed, Api::Unlimited, characterConfig->MpReserveSpeed, character->Guid());
}

void CharacterManager::OnEnterCopySceneTest(IPlayer* const player, const UInt64 account, const oEnterCopySceneTest& test) {
    CharacterDocker* docker = CharacterDocker::QueryCharacterDocker(account);
    if (nullptr == docker) {
        ErrorLog(g_Engine, "Where is account %llu %s's docker", account, player->Nickname().c_str());
        return;
    }

    IGameObject* deployCharacter = docker->GetDeployCharacter();
    if (nullptr == deployCharacter) {
        ErrorLog(g_Engine, "Account %lld %s never deploy a character, system error", account, player->Nickname().c_str());
        return;
    }

    IScene* scene = g_SceneManager->QueryCopyScene(deployCharacter);
    if (nullptr != scene) {
        ErrorLog(g_Engine, "Character %llu already in Copy Scene %llu %s", deployCharacter->Guid(), scene->SceneID(), scene->Config()->Name);
        return;
    }

    g_SceneManager->CreateCopyScene(test.sceneconfigid(),
        [=](IScene* const scene) {
            s_Self->EnterScene(deployCharacter, scene);
        }
    );
}

void CharacterManager::OnEquipReq(IPlayer* const player, const UInt64 account, const oEquipReq& req) {
    CharacterDocker* docker = CharacterDocker::QueryCharacterDocker(account);
    if (nullptr == docker) {
        ErrorLog(g_Engine, "Where is account %llu %s's docker", account, player->Nickname().c_str());
        return;
    }


    IGameObject* deployCharacter = docker->GetDeployCharacter();
    if (nullptr == deployCharacter) {
        ErrorLog(g_Engine, "Account %lld %s never deploy a character, system error", account, player->Nickname().c_str());
        return;
    }

    oEquipAsw asw;
    asw.set_code(eError::Unkown);
    if (!g_Backpack->Equip(deployCharacter->Guid(), req.equipguid())) {
        s_Self->SendProtobuf(deployCharacter->Guid(), eServerID::EquipAsw, asw);
        return;
    }

    IGameObject* equip = g_GameWorld->QueryGameObject(req.equipguid());
    if (nullptr == equip) {
        s_Self->SendProtobuf(deployCharacter->Guid(), eServerID::EquipAsw, asw);
        ErrorLog(g_Engine, "Can not find Equip %llu", req.equipguid());
        return;
    }

    AutoConfig::Enum::eEquipmentType equipType = (AutoConfig::Enum::eEquipmentType)equip->GetInt32(Equip::EquipTypeInt32);
    asw.set_code(eError::Succeed);
    asw.set_equipguid(equip->Guid());
    switch (equipType) {
    case AutoConfig::Enum::eEquipmentType::Shose: {
        deployCharacter->SetUInt64(Character::SlotShoesUInt64, equip->Guid());
        deployCharacter->SetInt64(Character::ShoesConfigInt64, equip->ConfigID());
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Weapon: {
        deployCharacter->SetUInt64(Character::SlotWeaponUInt64, equip->Guid());
        deployCharacter->SetInt64(Character::WeaponConfigInt64, equip->ConfigID());
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Pants: {
        deployCharacter->SetUInt64(Character::SlotPantsUInt64, equip->Guid());
        deployCharacter->SetInt64(Character::PantsConfigInt64, equip->ConfigID());
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Cloths: {
        deployCharacter->SetUInt64(Character::SlotClothsUInt64, equip->Guid());
        deployCharacter->SetInt64(Character::ClothsConfigInt64, equip->ConfigID());
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Belt: {
        deployCharacter->SetUInt64(Character::SlotBeltUInt64, equip->Guid());
        deployCharacter->SetInt64(Character::BeltConfigInt64, equip->ConfigID());
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Jewelry: {
        deployCharacter->SetUInt64(Character::SlotJewelryUInt64, equip->Guid());
        deployCharacter->SetInt64(Character::JewelryConfigInt64, equip->ConfigID());
        break;
    default:
        asw.set_code(eError::Unkown);
        s_Self->SendProtobuf(deployCharacter->Guid(), eServerID::EquipAsw, asw);
        ErrorLog(g_Engine, "error equip type %d", equipType);
        return;
    }
    }

    s_Self->SendProtobuf(deployCharacter->Guid(), eServerID::EquipAsw, asw);
    oEquipPush push;
    push.set_characterid(deployCharacter->Guid());
    push.set_configid(equip->ConfigID());
    push.set_equptype((int)equipType);

    docker->SaveCharacterData(deployCharacter);
    s_Self->Broadcast(deployCharacter->Guid(), eServerID::EquipPush, push, true);
}

void CharacterManager::OnUnEquipReq(IPlayer* const player, const UInt64 account, const oUnEquipReq& req) {
    CharacterDocker* docker = CharacterDocker::QueryCharacterDocker(account);
    if (nullptr == docker) {
        ErrorLog(g_Engine, "Where is account %llu %s's docker", account, player->Nickname().c_str());
        return;
    }


    IGameObject* deployCharacter = docker->GetDeployCharacter();
    if (nullptr == deployCharacter) {
        ErrorLog(g_Engine, "Account %lld %s never deploy a character, system error", account, player->Nickname().c_str());
        return;
    }

    oUnEquipAsw asw;
    asw.set_code(eError::Unkown);
    if (!g_Backpack->UnEquip(deployCharacter->Guid(), req.equipguid())) {
        s_Self->SendProtobuf(deployCharacter->Guid(), eServerID::UnEquipAsw, asw);
        return;
    }

    IGameObject* equip = g_GameWorld->QueryGameObject(req.equipguid());
    if (nullptr == equip) {
        s_Self->SendProtobuf(deployCharacter->Guid(), eServerID::UnEquipAsw, asw);
        ErrorLog(g_Engine, "Can not find Equip %llu", req.equipguid());
        return;
    }

    AutoConfig::Enum::eEquipmentType equipType = (AutoConfig::Enum::eEquipmentType)equip->GetInt32(Equip::EquipTypeInt32);
    asw.set_code(eError::Succeed);
    asw.set_equipguid(equip->Guid());
    switch (equipType) {
    case AutoConfig::Enum::eEquipmentType::Shose: {
        deployCharacter->SetUInt64(Character::SlotShoesUInt64, INVALID_EQUIP_ID);
        deployCharacter->SetInt64(Character::ShoesConfigInt64, 0);
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Weapon: {
        deployCharacter->SetUInt64(Character::SlotWeaponUInt64, INVALID_EQUIP_ID);
        deployCharacter->SetInt64(Character::WeaponConfigInt64, 0);
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Pants: {
        deployCharacter->SetUInt64(Character::SlotPantsUInt64, INVALID_EQUIP_ID);
        deployCharacter->SetInt64(Character::PantsConfigInt64, 0);
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Cloths: {
        deployCharacter->SetUInt64(Character::SlotClothsUInt64, INVALID_EQUIP_ID);
        deployCharacter->SetInt64(Character::ClothsConfigInt64, 0);
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Belt: {
        deployCharacter->SetUInt64(Character::SlotBeltUInt64, INVALID_EQUIP_ID);
        deployCharacter->SetInt64(Character::BeltConfigInt64, 0);
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Jewelry: {
        deployCharacter->SetUInt64(Character::SlotJewelryUInt64, INVALID_EQUIP_ID);
        deployCharacter->SetInt64(Character::JewelryConfigInt64, 0);
        break;
    default:
        asw.set_code(eError::Unkown);
        s_Self->SendProtobuf(deployCharacter->Guid(), eServerID::UnEquipAsw, asw);
        ErrorLog(g_Engine, "error equip type %d", equipType);
        return;
    }
    }
    s_Self->SendProtobuf(deployCharacter->Guid(), eServerID::UnEquipAsw, asw);

    oUnEquipPush push;
    push.set_characterid(deployCharacter->Guid());
    push.set_equptype((int)equipType);
    s_Self->Broadcast(deployCharacter->Guid(), eServerID::UnEquipPush, push, true);

    docker->SaveCharacterData(deployCharacter);
}

void CharacterManager::OnAppear(IScene* const, const UInt64 guid, const std::vector<UInt64>& others) {
    //     IGameObject* go = g_GameWorld->QueryGameObject(guid);
    //     if (go) {
    //         oGameObjectAppearPush pushToSelf, pushToOthers;
    //         oGameUnit* pbToOther = pushToOthers.add_gos();
    //         PackageGameObjectToPB(pbToOther, go, true);
    //         g_BroadcastTemp.clear();
    //         for (int i = 0; i < others.size(); i++) {
    //             IGameObject* other = g_GameWorld->QueryGameObject(others[i]);
    //             if (other) {
    //                 if (go->Type() == eGameObjectType::Character) {
    //                     oGameUnit* pb = pushToSelf.add_gos();
    //                     PackageGameObjectToPB(pb, other, true);
    //                 }
    // 
    //                 if (other->Type() == eGameObjectType::Character) {
    //                     g_BroadcastTemp.push_back(other->GetUInt64(Character::AccountUInt64));
    //                 }
    //             }
    //             else {
    //                 ErrorLog(g_Engine, "Cannot find game object %llu", others[i]);
    //             }
    //         }
    // 
    //         if (go->Type() == eGameObjectType::Character) {
    //             g_PlayerManager->SendProtobufToPlayer(go->GetUInt64(Character::AccountUInt64), eServerID::GameObjectAppearPush, pushToSelf);
    //         }
    // 
    //         g_PlayerManager->Broadcast(g_BroadcastTemp, eServerID::GameObjectAppearPush, pushToOthers);
    //     }
}

void CharacterManager::OnDisappear(IScene* const, const UInt64 guid, const std::vector<UInt64>& others) {
    //     IGameObject* go = g_GameWorld->QueryGameObject(guid);
    //     if (go) {
    //         oGameObjectDisappearPush pushToSelf, pushToOthers;
    //         pushToOthers.add_guids(guid);
    //         g_BroadcastTemp.clear();
    //         for (int i = 0; i < others.size(); i++) {
    //             IGameObject* other = g_GameWorld->QueryGameObject(others[i]);
    //             if (other) {
    //                 if (go->Type() == eGameObjectType::Character) {
    //                     pushToSelf.add_guids(others[i]);
    //                 }
    // 
    //                 if (other->Type() == eGameObjectType::Character) {
    //                     g_BroadcastTemp.push_back(other->GetUInt64(Character::AccountUInt64));
    //                 }
    //             }
    //             else {
    //                 ErrorLog(g_Engine, "Cannot find game object %llu", others[i]);
    //             }
    //         }
    // 
    //         if (go->Type() == eGameObjectType::Character) {
    //             g_PlayerManager->SendProtobufToPlayer(go->GetUInt64(Character::AccountUInt64), eServerID::GameObjectDisappearPush, pushToSelf);
    //         }
    // 
    //         g_PlayerManager->Broadcast(g_BroadcastTemp, eServerID::GameObjectDisappearPush, pushToOthers);
    //     }
}

ICharacterDocker* CharacterManager::CreateCharacterDocker(const UInt64 account, const char* file, const int line) {
    return CharacterDocker::CreateCharacterDocker(account, file, line);
}

ICharacterDocker* CharacterManager::QueryCharacterDocker(const UInt64 account) {
    return CharacterDocker::QueryCharacterDocker(account);
}

void CharacterManager::ReleaseCharacterDocker(const UInt64 account, const char* file, const int line) {
    CharacterDocker::ReleaseCharacterDocker(account, file, line);
}

void CharacterManager::RegistCharacterDockerEvent(const eCharacterDockerEvent ev, const fCharacterDockerEvent callback, const char* file, const int line) {
    g_CharacterDockerEvent.Register(ev, callback, file, line);
}

void CharacterManager::EnterScene(IGameObject* const character, IScene* const scene) {
    IScene* lastScene = g_SceneManager->QuerySceneByEntityGuid(character->Guid());
    if (lastScene == scene)
    {
        return;
    }

	if (lastScene) {
		lastScene->ExitScene(character->Guid());
	}

    scene->EnterScene(character->Guid(), nullptr, true);
    if (scene->Config()->SceneType == AutoConfig::Enum::eSceneType::Public)
    {
        g_Fsm->ForceEnterState(character->Guid(), AutoConfig::Enum::eState::Idle, character->Position(), character->Angle());
        character->SetFloat(Entity::HpFloat, character->GetFloat(Entity::MaxHpFloat));
        character->SetFloat(Entity::MpFloat, character->GetFloat(Entity::MaxMpFloat));
    }
}

void CharacterManager::SendProtobuf(const UInt64 characterId, const eServerID id, const::google::protobuf::Message& pb) {
    IGameObject* character = g_GameWorld->QueryGameObject(characterId);
    if (nullptr == character) {
        ErrorLog(g_Engine, "where is character %llu", characterId);
        return;
    }


    if (character->Type() != eGameObjectType::Character) {
        ErrorLog(g_Engine, "GameObject %llu is not a Character, type %d", character->Guid(), character->Type());
        return;
    }

    g_PlayerManager->SendProtobufToPlayer(character->GetUInt64(Character::AccountUInt64), id, pb);
}

void CharacterManager::Broadcast(const UInt64 characterId, const eServerID id, const ::google::protobuf::Message& pb, const bool excludeSelf) {
    IGameObject* character = g_GameWorld->QueryGameObject(characterId);
    if (character->Type() != eGameObjectType::Character) {
        return;
    }

    g_BroadcastTemp.clear();
    if (!excludeSelf) {
        g_BroadcastTemp.push_back(character->GetUInt64(Character::AccountUInt64));
    }

    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(characterId);
    scene->GetVisual(characterId,
        [&](const std::unordered_set<UInt64>& visual) {
            for (auto it = visual.begin(); it != visual.end(); it++) {
                IGameObject* other = g_GameWorld->QueryGameObject(*it);
                if (other->Type() == eGameObjectType::Character) {
                    g_BroadcastTemp.push_back(other->GetUInt64(Character::AccountUInt64));
                }
            }
        }
    );
    g_PlayerManager->Broadcast(g_BroadcastTemp, id, pb);
    g_BroadcastTemp.clear();
}

void CharacterManager::OnCharacterHpChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue)
{
    if (index == Character::HpFloat)
    {
        if (newValue <= 0)
        {
            if (g_Fsm->EnterState(go->Guid(), AutoConfig::Enum::eState::Death, go->Position(), go->Angle()))
            {
                START_TIMER(g_Engine, s_Self, eTimerID::CharacterBack, 5000, 1, 1000, go->Guid());
            }
        }
    }
}

bool CharacterManager::OnCharacterHPChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue) {
    if (index == Character::HpFloat)
    {
        return g_Fsm->GetState(go->Guid()) != AutoConfig::Enum::eState::Death;
    }

    return true;
}
