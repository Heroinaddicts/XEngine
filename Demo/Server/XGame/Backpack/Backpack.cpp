#include "Backpack.h"
#include "BackpackMatrix.h"

Api::iEngine* g_Engine = nullptr;
ICharacterManager* g_CharacterManager = nullptr;
IConfig* g_Config = nullptr;
ICacheClient* g_CacheClient = nullptr;
IServerManager* g_ServerManager = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
IGameWorld* g_GameWorld = nullptr;
IFsm* g_Fsm = nullptr;
ITaskManager* g_TaskManager = nullptr;

static Backpack* s_Self = nullptr;

bool Backpack::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool Backpack::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_CacheClient, CacheClient);
    GetModule(g_Engine, g_CharacterManager, CharacterManager);
    GetModule(g_Engine, g_ServerManager, ServerManager);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_Fsm, Fsm);
    GetModule(g_Engine, g_TaskManager, TaskManager);

    g_CharacterManager->RegistCharacterDockerEvent(ICharacterManager::eCharacterDockerEvent::CharacterDockerCreated, Backpack::OnCharacterDataCreated);
    return true;
}

bool Backpack::QueryResourceByID(const UInt64 characterGuid, const UInt64 resourceId)
{
    BackpackMatrix2D* backpack = BackpackMatrix2D::QueryBackpackMatrix2D(characterGuid);
    XASSERT(backpack != nullptr, "wtf");

    return backpack->QueryResourceByID(resourceId);
}

bool Backpack::Equip(const UInt64 characterGuid, const UInt64 equipGuid)
{
    BackpackMatrix2D* backpack = BackpackMatrix2D::QueryBackpackMatrix2D(characterGuid);
    XASSERT(backpack != nullptr, "wtf");

    bool unEuip = true;
    //判断人物是否已经装备
    IGameObject* character = g_GameWorld->QueryGameObject(characterGuid);
    IGameObject* equip = g_GameWorld->QueryGameObject(equipGuid);
    AutoConfig::Enum::eEquipmentType equipType = (AutoConfig::Enum::eEquipmentType)equip->GetInt32(Equip::EquipTypeInt32);
    switch (equipType) {
    case AutoConfig::Enum::eEquipmentType::Shose: {
        if (character->GetUInt64(Character::SlotShoesUInt64) != INVALID_EQUIP_ID)
        {
            unEuip = backpack->UnEquip(character->GetUInt64(Character::SlotShoesUInt64));
        }
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Weapon: {
        if (character->GetUInt64(Character::SlotWeaponUInt64) != INVALID_EQUIP_ID)
        {
            unEuip = backpack->UnEquip(character->GetUInt64(Character::SlotWeaponUInt64));
        }
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Pants: {
        if (character->GetUInt64(Character::SlotPantsUInt64) != INVALID_EQUIP_ID)
        {
            unEuip = backpack->UnEquip(character->GetUInt64(Character::SlotPantsUInt64));
        }
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Cloths: {
        if (character->GetUInt64(Character::SlotClothsUInt64) != INVALID_EQUIP_ID)
        {
            unEuip = backpack->UnEquip(character->GetUInt64(Character::SlotClothsUInt64));
        }
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Belt: {
        if (character->GetUInt64(Character::SlotBeltUInt64) != INVALID_EQUIP_ID)
        {
            unEuip = backpack->UnEquip(character->GetUInt64(Character::SlotBeltUInt64));
        }
        break;
    }
    case AutoConfig::Enum::eEquipmentType::Jewelry: {
        if (character->GetUInt64(Character::SlotJewelryUInt64) != INVALID_EQUIP_ID)
        {
            unEuip = backpack->UnEquip(character->GetUInt64(Character::SlotJewelryUInt64));
        }
        break;
    }
    }
    bool succeed = unEuip && backpack->Equip(equipGuid);
    SaveBackpackData(character);
    if (succeed)
    {
        g_TaskManager->OnEquipWear(character->GetUInt64(Character::AccountUInt64), equipGuid);
    }
    return succeed;
}

bool Backpack::UnEquip(const UInt64 characterGuid, const UInt64 equipGuid)
{
    BackpackMatrix2D* backpack = BackpackMatrix2D::QueryBackpackMatrix2D(characterGuid);
    XASSERT(backpack != nullptr, "wtf");
    bool succeed = backpack->UnEquip(equipGuid);
    SaveBackpackData(g_GameWorld->QueryGameObject(characterGuid));
    return succeed;
}

void Backpack::SaveBackpackData(IGameObject* const gameObject)
{
    g_CacheClient->Update(gameObject->GetUInt64(Character::AccountUInt64), eTable::t_backpack, t_backpack::CharacterId__UInt64__PrimaryUnique, gameObject->Guid(),
        [&](ISQLCacheRow* const row) {
            BackpackMatrix2D* backpack = BackpackMatrix2D::QueryBackpackMatrix2D(gameObject->Guid());
            oBackpackItemPush backpackData = backpack->PackBackpackData();

            std::string serializedStr;
            bool serializeSuccess = backpackData.SerializeToString(&serializedStr);
            std::string encodedStr = XEngine::XBase64::Encode(serializedStr);
            row->GetColumn(t_backpack::BackPackData__String__)->SetString(encodedStr);
        },
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (!succeed)
            {
                ErrorLog(g_Engine, "UpdateBackpackData Error");
            }
        }
    );
}

void Backpack::OnCharacterDataCreated(ICharacterDocker* const docker, IGameObject* const gameObject) {
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterCreated, Backpack::OnCharacterCreated);
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterLoaded, Backpack::OnCharacterLoaded);
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterRelease, Backpack::OnCharacterRelease);
}

void Backpack::OnCharacterRelease(ICharacterDocker* const docker, IGameObject* const gameObject)
{
    if (BackpackMatrix2D::QueryBackpackMatrix2D(gameObject->Guid()) == nullptr)
    {
        return;
    }
    g_CacheClient->Update(docker->_Account, eTable::t_backpack, t_backpack::CharacterId__UInt64__PrimaryUnique, gameObject->Guid(),
        [&](ISQLCacheRow* const row) {
            BackpackMatrix2D* backpack = BackpackMatrix2D::QueryBackpackMatrix2D(gameObject->Guid());
            oBackpackItemPush backpackData = backpack->PackBackpackData();

            std::string serializedStr;
            bool serializeSuccess = backpackData.SerializeToString(&serializedStr);
            std::string encodedStr = XEngine::XBase64::Encode(serializedStr);
            row->GetColumn(t_backpack::BackPackData__String__)->SetString(encodedStr);

            BackpackMatrix2D::ReleaseBackpackMatrix2D(gameObject->Guid());
        },
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (!succeed)
            {
                ErrorLog(g_Engine, "UpdateBackpackData Error");
            }
        }
    );
}

void Backpack::OnCharacterLoaded(ICharacterDocker* const docker, IGameObject* const gameObject)
{
    if (BackpackMatrix2D::QueryBackpackMatrix2D(gameObject->Guid()) != nullptr)
    {
        BackpackMatrix2D* backpack = BackpackMatrix2D::QueryBackpackMatrix2D(gameObject->Guid());
        g_PlayerManager->SendProtobufToPlayer(docker->_Account, eServerID::BackpackItemPush, backpack->PackBackpackData());
        return;
    }
    g_CacheClient->Query(docker->_Account, eTable::t_backpack, t_backpack::CharacterId__UInt64__PrimaryUnique, gameObject->Guid(),
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (!succeed) {
                ErrorLog(g_Engine, "Query BackPack %11u CharacterId Error", gameObject->Guid());
                return;
            }

            if (tab != nullptr) {

                tab->Query(t_backpack::CharacterId__UInt64__PrimaryUnique, gameObject->Guid(),
                    [&](ISQLCacheRow* const row) {
                        oBackpackItemPush backpackData;
                        std::string decodedStr = XEngine::XBase64::Decode(row->GetColumn(t_backpack::BackPackData__String__)->GetString());
                        backpackData.ParseFromString(decodedStr);

                        BackpackMatrix2D* backpack = BackpackMatrix2D::CreateBackpackMatrix2D(gameObject->Guid(), backpackData.width(), backpackData.height(), backpackData.totalweight());
                        backpack->LoadBackpackData(backpackData);

                        g_PlayerManager->SendProtobufToPlayer(docker->_Account, eServerID::BackpackItemPush, backpackData);
                    });
            }
        });
}

void Backpack::GetRewardItems(const UInt64 characterGuid, const std::vector<Int64>& itemResourceIds)
{
    BackpackMatrix2D* backpack = BackpackMatrix2D::QueryBackpackMatrix2D(characterGuid);
    XASSERT(backpack != nullptr, "wtf");
    backpack->StartCalculateChange();
    for (auto const& id : itemResourceIds)
    {
        const AutoConfig::Struct::Resource* resourceConfig = g_Config->GetAutoConfig()->QueryResourceConfig(id);
        eGameObjectType type = resourceConfig->ResourceType == AutoConfig::Enum::eResources::Equipment ? eGameObjectType::Equip : eGameObjectType::Item;
        IGameObject* item = g_GameWorld->CreateGameObject(type, resourceConfig->RelevantID, AUTO_GUID,
            [&](IGameObject* const go) {
                go->SetInt64(Equip::ResourceIdInt64, id);
                go->SetInt32(Equip::ItemTypeInt32, static_cast<int>(resourceConfig->ResourceType));
                go->SetInt32(Equip::CountInt32, resourceConfig->Count);
                if (type == eGameObjectType::Equip)
                {
                    const AutoConfig::Struct::Equipment* equipmentConfig = g_Config->GetAutoConfig()->QueryEquipmentConfig(resourceConfig->RelevantID);
                    go->SetInt32(Equip::EquipTypeInt32, static_cast<int>(equipmentConfig->EquipmentType));
                    go->SetBool(Equip::EquipedBool, false);
                    go->SetInt32(Equip::PatienceInt32, equipmentConfig->Patience);
                    go->SetInt32(Equip::StrengthLvInt32, 0);
                }
            });
        backpack->Push(item, true);
    }
    oBackpackAddItemChange change = backpack->GetAddItemChangeData();
    //TODO 多余的后续通过邮件发送
    change.clear_remainingitem();
    change.set_code(eError::Succeed);
    g_PlayerManager->SendProtobufToPlayer(g_GameWorld->QueryGameObject(characterGuid)->GetUInt64(Character::AccountUInt64), eServerID::BackpackAddItemPush, change);
}

void Backpack::OnCharacterCreated(ICharacterDocker* const docker, IGameObject* const gameObject)
{
    if (BackpackMatrix2D::QueryBackpackMatrix2D(gameObject->Guid()) != nullptr)
    {
        return;
    }
    int width = AutoConfig::Const::PackRules::PackSlot[0];
    int height = AutoConfig::Const::PackRules::PackSlot[1];
    BackpackMatrix2D* backpack = BackpackMatrix2D::CreateBackpackMatrix2D(gameObject->Guid(), width, height);
    //默认穿戴东西
    const AutoConfig::Struct::Resource* resourceConfig = g_Config->GetAutoConfig()->QueryResourceConfig(AutoConfig::Const::InitialResource::InitialWeapon);
    const AutoConfig::Struct::Equipment* equipmentConfig = g_Config->GetAutoConfig()->QueryEquipmentConfig(resourceConfig->RelevantID);
    IGameObject* equip = g_GameWorld->CreateGameObject(eGameObjectType::Equip, equipmentConfig->ID, AUTO_GUID,
        [&](IGameObject* const go) {
            go->SetInt64(Equip::ResourceIdInt64, AutoConfig::Const::InitialResource::InitialWeapon);
            go->SetInt32(Equip::ItemTypeInt32, static_cast<int>(resourceConfig->ResourceType));
            go->SetInt32(Equip::CountInt32, resourceConfig->Count);
            go->SetInt32(Equip::EquipTypeInt32, static_cast<int>(equipmentConfig->EquipmentType));
            go->SetBool(Equip::EquipedBool, false);
            go->SetInt32(Equip::PatienceInt32, equipmentConfig->Patience);
            go->SetInt32(Equip::StrengthLvInt32, 0);
        });
    backpack->Push(equip, true);

    IGameObject* equip1 = g_GameWorld->CreateGameObject(eGameObjectType::Equip, 2006, AUTO_GUID,
        [&](IGameObject* const go) {
            go->SetInt32(Equip::ItemTypeInt32, static_cast<int>(AutoConfig::Enum::eResources::Equipment));
            go->SetInt32(Equip::CountInt32, 1);
            go->SetInt32(Equip::EquipTypeInt32, static_cast<int>(AutoConfig::Enum::eEquipmentType::Weapon));
            go->SetBool(Equip::EquipedBool, false);
            go->SetInt32(Equip::PatienceInt32, 30);
            go->SetInt32(Equip::StrengthLvInt32, 0);
        });
    backpack->Push(equip1, true);

    g_CacheClient->Create(docker->_Account, eTable::t_backpack,
        [&](ISQLCacheRow* const row) {
            oBackpackItemPush backpackData = backpack->PackBackpackData();
            row->GetColumn(t_backpack::Account__UInt64__)->SetUInt64(docker->_Account);
            row->GetColumn(t_backpack::CharacterId__UInt64__PrimaryUnique)->SetUInt64(gameObject->Guid());

            std::string serializedStr;
            bool serializeSuccess = backpackData.SerializeToString(&serializedStr);
            std::string encodedStr = XEngine::XBase64::Encode(serializedStr);

            row->GetColumn(t_backpack::BackPackData__String__)->SetString(encodedStr);
        },
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (succeed) {
                g_PlayerManager->SendProtobufToPlayer(docker->_Account, eServerID::BackpackItemPush, backpack->PackBackpackData());
            }
            else {
                ErrorLog(g_Engine, "Create BackPack %11u Account %11u CharacterId Error", docker->_Account, gameObject->Guid());
            }
        });
}

bool Backpack::Destroy(Api::iEngine* const engine) {
    return true;
}
