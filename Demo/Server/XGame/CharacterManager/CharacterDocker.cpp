#include "CharacterDocker.h"
#include "CharacterManager.h"

static XPool<CharacterDocker> s_Pool;
static std::unordered_map<UInt64, CharacterDocker*> s_CharacterDockerMap;

CharacterDocker* CharacterDocker::CreateCharacterDocker(const UInt64 account, const char* file, const int line) {
    if (s_CharacterDockerMap.find(account) != s_CharacterDockerMap.end()) {
        ErrorLog(g_Engine, "CharacterDocker account %llu already exists %s:%d", account, file, line);
        XASSERT(false, "wtf");
        return nullptr;
    }

    CharacterDocker* docker = XPOOL_CREATE(s_Pool, account, file, line);
    s_CharacterDockerMap.insert({ account, docker });
    g_CharacterDockerEvent.Trigger(ICharacterManager::eCharacterDockerEvent::CharacterDockerCreated, docker, nullptr);
    return docker;
}

CharacterDocker* CharacterDocker::QueryCharacterDocker(const UInt64 account) {
    auto itor = s_CharacterDockerMap.find(account);
    return s_CharacterDockerMap.end() != itor ? itor->second : nullptr;
}

void CharacterDocker::ReleaseCharacterDocker(const UInt64 account, const char* file, const int line) {
    auto itor = s_CharacterDockerMap.find(account);
    if (s_CharacterDockerMap.end() == itor) {
        ErrorLog(g_Engine, "CharacterDocker %llu not exists %s:%d", account, file, line);
        XASSERT(false, "wtf");
        return;
    }

    g_Fsm->ReleaseFsm(itor->second->_DeployCharacterID);

    for (auto i = itor->second->_Characters.begin(); i != itor->second->_Characters.end(); i++) {
        itor->second->_Events.Trigger(ICharacterDocker::eCharacterEvent::CharacterRelease, itor->second, i->second);
        g_GameWorld->ReleaseGameObject(i->second);
    }

    g_CharacterDockerEvent.Trigger(ICharacterManager::eCharacterDockerEvent::ChracterDockerRelease, itor->second, nullptr);
    XPOOL_RELEASE(s_Pool, itor->second);
    s_CharacterDockerMap.erase(itor);
}

IGameObject* CharacterDocker::__CreateCharacters(const Int64 configId) {
    const AutoConfig::Struct::Character* characterConfig = g_Config->GetAutoConfig()->QueryCharacterConfig(configId);
    XASSERT(characterConfig, "wtf");
    IGameObject* character = g_GameWorld->CreateGameObject(eGameObjectType::Character, configId, AUTO_GUID,
        [&](IGameObject* const go) {
            go->SetUInt64(Character::AccountUInt64, _Account);
            go->SetString(Character::NicknameString, g_PlayerManager->QueryPlayer(_Account)->Nickname());
			go->SetInt64(Character::LevelInt64, 1);
			go->SetInt64(Character::ExpInt64, 0);
            go->SetUInt64(Character::SlotClothsUInt64, INVALID_EQUIP_ID);
            go->SetUInt64(Character::SlotPantsUInt64, INVALID_EQUIP_ID);
            go->SetUInt64(Character::SlotBeltUInt64, INVALID_EQUIP_ID);
            go->SetUInt64(Character::SlotShoesUInt64, INVALID_EQUIP_ID);
            go->SetUInt64(Character::SlotJewelryUInt64, INVALID_EQUIP_ID);
            go->SetUInt64(Character::SlotWeaponUInt64, INVALID_EQUIP_ID);


            go->SetFloat(Character::AtkFloat, characterConfig->InitialAtk);
            go->SetFloat(Character::SpeedFloat, characterConfig->InitialSpeed);
            go->SetFloat(Character::HpFloat, characterConfig->InitialHp);
            go->SetFloat(Character::MaxHpFloat, characterConfig->InitialHp);
            go->SetFloat(Character::CriticalFloat, characterConfig->InitialCritical);
            go->SetFloat(Character::CriticalDamageFloat, characterConfig->InitialCriticalDamage);
            go->SetFloat(Character::DefendFloat, characterConfig->InitialDefend);
            go->SetFloat(Character::MagicDefendFloat, characterConfig->InitialMagicDefend);
            go->SetFloat(Character::BloodFloat, characterConfig->InitialBlood);
            go->SetFloat(Character::MagicAttackFloat, characterConfig->InitialMagicAttack);
            go->SetFloat(Character::PhysicalPierceFloat, characterConfig->InitialPhysicalPierce);
            go->SetFloat(Character::PhysicalPierceRateFloat, characterConfig->PhysicalPierceRate);
            go->SetFloat(Character::MagicPierceFloat, characterConfig->InitialMagicPierce);
            go->SetFloat(Character::MagicPierceRateFloat, characterConfig->MagicPierceRate);
            go->SetFloat(Character::AffordFloat, characterConfig->InitialAfford);
            go->SetFloat(Character::DodgeFloat, characterConfig->InitialDodge);
            go->SetFloat(Character::ResilienceFloat, characterConfig->InitialResilience);
            go->SetFloat(Character::VisualFieldFloat, characterConfig->VisualField);
            go->SetFloat(Character::AtkSpeedFloat, characterConfig->InitialAtkSpeed3);
            go->SetFloat(Character::MpFloat, characterConfig->InitialMp);
            go->SetFloat(Character::MaxMpFloat, characterConfig->InitialMp);
            go->SetInt32(Character::DamagePriorityInt32, (Int32)characterConfig->DamagePriority);
            go->SetFloat(Character::HpRecoverFloat, characterConfig->InitialHpReserve);
            go->SetFloat(Character::MpRecoverFloat, characterConfig->InitialMpReserve);

            go->SetInt64(Character::LastPublicSceneConfigIDInt64, AutoConfig::Const::World::DefaultWorld);
            const AutoConfig::Struct::Scene* const config = g_Config->GetAutoConfig()->QuerySceneConfig(AutoConfig::Const::World::DefaultWorld);
            go->SetPosition(config->StartPoint[0], config->StartPoint[1]);
        }
    );
    _Characters.insert({ character->Guid(), character });

    g_CacheClient->Create(_Account, eTable::t_character,
        [&](ISQLCacheRow* const row) {
            row->GetColumn(t_character::Account__UInt64__)->SetUInt64(_Account);
            row->GetColumn(t_character::Guid__UInt64__PrimaryUnique)->SetUInt64(character->Guid());
            row->GetColumn(t_character::ConfigID__Int64__)->SetInt64(configId);
            row->GetColumn(t_character::Level__Int64__)->SetInt64(1);
            row->GetColumn(t_character::Exp__Int64__)->SetInt64(0);
            row->GetColumn(t_character::LastPublicSceneConfigID__Int64__)->SetInt64(character->GetInt64(Character::LastPublicSceneConfigIDInt64));
            row->GetColumn(t_character::SceneGUID__UInt64__)->SetUInt64(character->GetUInt64(Character::SceneGUIDUInt64));
            row->GetColumn(t_character::X__Float__)->SetFloat(character->Position().x);
            row->GetColumn(t_character::Y__Float__)->SetFloat(character->Position().y);
            row->GetColumn(t_character::Angle__Float__)->SetFloat(character->Angle());

            row->GetColumn(t_character::SlotCloths__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotClothsUInt64));
            row->GetColumn(t_character::ClothsConfigID__Int64__)->SetInt64(character->GetInt64(Character::ClothsConfigInt64));

            row->GetColumn(t_character::SlotPants__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotPantsUInt64));
            row->GetColumn(t_character::PantsConfigID__Int64__)->SetInt64(character->GetInt64(Character::PantsConfigInt64));

            row->GetColumn(t_character::SlotBelt__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotBeltUInt64));
            row->GetColumn(t_character::BeltConfigID__Int64__)->SetInt64(character->GetInt64(Character::BeltConfigInt64));

            row->GetColumn(t_character::SlotShoes__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotShoesUInt64));
            row->GetColumn(t_character::ShoesConfigID__Int64__)->SetInt64(character->GetInt64(Character::ShoesConfigInt64));

            row->GetColumn(t_character::SlotJewelry__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotJewelryUInt64));
            row->GetColumn(t_character::JewelryConfigID__Int64__)->SetInt64(character->GetInt64(Character::JewelryConfigInt64));

            row->GetColumn(t_character::SlotWeapon__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotWeaponUInt64));
            row->GetColumn(t_character::WeaponConfigID__Int64__)->SetInt64(character->GetInt64(Character::WeaponConfigInt64));
        },
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (succeed) {
                TraceLog(g_Engine, "Insert Character %llu Account %llu Into Database succeed", character->Guid(), _Account);
            }
            else {
                ErrorLog(g_Engine, "Insert Character %llu Account %llu Into Database error", character->Guid(), _Account);
            }
        }
    );

    return character;
}

IGameObject* CharacterDocker::DeployCharacter(const UInt64 characterId) {
    XASSERT(characterId != 0, "wtf");
    if (_DeployCharacterID == characterId)
        return nullptr;

    IGameObject* const lastDeployer = g_GameWorld->QueryGameObject(_DeployCharacterID);
    XASSERT(_DeployCharacterID == INVALID_GAMEOBJECT_ID || lastDeployer, "wtf");

    IGameObject* const willDeployer = g_GameWorld->QueryGameObject(characterId);
    XASSERT(willDeployer, "wtf");

    if (nullptr == willDeployer) {
        ErrorLog(g_Engine, "where is character %llu", characterId);
        XASSERT(false, "wtf");
        return nullptr;
    }

    IScene* scene = nullptr;
    if (lastDeployer) {
        willDeployer->SetAngle(lastDeployer->Angle());
        willDeployer->SetPosition(lastDeployer->Position());
        willDeployer->SetInt64(Character::LastPublicSceneConfigIDInt64, lastDeployer->GetInt64(Character::LastPublicSceneConfigIDInt64));
        willDeployer->SetUInt64(Character::SceneGUIDUInt64, lastDeployer->GetUInt64(Character::SceneGUIDUInt64));

        //这里断言一下，如果是在副本场景内，是不允许切换角色的，说明上层逻辑有问题
        XASSERT(g_SceneManager->QueryCopyScene(lastDeployer) == nullptr, "wtf");

        //这里通知当前场景中其他玩家，该角色退出场景
        scene = g_SceneManager->QueryPublicScene(lastDeployer->GetInt64(Character::LastPublicSceneConfigIDInt64));
        XASSERT(scene, "wtf");


        scene->ExitScene(_DeployCharacterID,
            [&](IScene* const, const UInt64 guid, const UInt64 other) {

            }
        );

        _Events.Trigger(eCharacterEvent::CharacterUnDeployed, this, lastDeployer);
        g_Fsm->ReleaseFsm(lastDeployer->Guid());
    }
    else {
        scene = g_SceneManager->QueryPublicScene(willDeployer->GetInt64(Character::LastPublicSceneConfigIDInt64));
    }

    if (!scene) {
        scene = g_SceneManager->QueryPublicScene(AutoConfig::Const::World::DefaultWorld);
        willDeployer->SetInt64(Character::LastPublicSceneConfigIDInt64, AutoConfig::Const::World::DefaultWorld);
        willDeployer->SetUInt64(Character::SceneGUIDUInt64, INVALID_SCENE_GUID);
        const auto config = g_Config->GetAutoConfig()->QuerySceneConfig(AutoConfig::Const::World::DefaultWorld);
        XASSERT(config, "wtf");
        willDeployer->SetPosition(config->StartPoint[0], config->StartPoint[1]);
    }

    g_Fsm->CreateFsm(willDeployer->Guid());
    g_BroadcastTemp.clear();

    if (!lastDeployer) {
        oSceneInfoPush sceneInfoPush;
        sceneInfoPush.mutable_info()->set_configid(scene->Config()->ID);
        sceneInfoPush.mutable_info()->set_sceneid(scene->SceneID());
        g_PlayerManager->SendProtobufToPlayer(willDeployer->GetUInt64(Character::AccountUInt64), eServerID::SceneInfoPush, sceneInfoPush);
    }
    _DeployCharacterID = characterId;
    oSwitchCharacterPush switchCharacterPush;
    switchCharacterPush.set_characterid(willDeployer->Guid());
    switchCharacterPush.set_x(willDeployer->Position().x);
    switchCharacterPush.set_y(willDeployer->Position().y);
    g_PlayerManager->SendProtobufToPlayer(willDeployer->GetUInt64(Character::AccountUInt64), eServerID::SwitchCharacterPush, switchCharacterPush);

    _DeployCharacterID = willDeployer->Guid();
    scene->EnterScene(characterId, nullptr, false);
    _Events.Trigger(eCharacterEvent::CharacterDeployed, this, willDeployer);
    return willDeployer;
}

void CharacterDocker::__LoadCharacterDockerData__(CharacterDocker* docker, const UInt64 account, const fCharacterDockerLoaded& callback) {
    g_CacheClient->Query(account, eTable::t_character, t_character::Account__UInt64__, account,
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (!succeed) {
                ErrorLog(g_Engine, "Load Character Docker Data error, %llu", account);
                XASSERT(false, "wtf");
                callback(false);
                return;
            }

            if (QueryCharacterDocker(account) != docker) {
                ErrorLog(g_Engine, "Character Docker %llu has changed", account);
                XASSERT(false, "wtf");
                callback(false);
                return;
            }

            tab->Query(t_character::Account__UInt64__, account,
                [&](const ISQLCacheRow* const row) {

                    const UInt64 characterId = row->GetColumn(t_character::Guid__UInt64__PrimaryUnique)->GetUInt64();
                    const Int64 configId = row->GetColumn(t_character::ConfigID__Int64__)->GetInt64();
                    const AutoConfig::Struct::Character* characterConfig = g_Config->GetAutoConfig()->QueryCharacterConfig(configId);

                    const Int64 level = row->GetColumn(t_character::Level__Int64__)->GetInt64();
                    const Int64 exp = row->GetColumn(t_character::Exp__Int64__)->GetInt64();
                    const Int64 lastPublicSceneConfigId = row->GetColumn(t_character::LastPublicSceneConfigID__Int64__)->GetInt64();
                    const UInt64 sceneGuid = row->GetColumn(t_character::SceneGUID__UInt64__)->GetUInt64();

                    const float x = row->GetColumn(t_character::X__Float__)->GetFloat();
                    const float y = row->GetColumn(t_character::Y__Float__)->GetFloat();
                    const float angle = row->GetColumn(t_character::Angle__Float__)->GetFloat();

                    const UInt64 slotCloths = row->GetColumn(t_character::SlotCloths__UInt64__)->GetUInt64();
                    const Int64 clothsConfigID = row->GetColumn(t_character::ClothsConfigID__Int64__)->GetInt64();

                    const UInt64 slotPants = row->GetColumn(t_character::SlotPants__UInt64__)->GetUInt64();
                    const Int64 pantsConfigID = row->GetColumn(t_character::PantsConfigID__Int64__)->GetInt64();

                    const UInt64 slotBelt = row->GetColumn(t_character::SlotBelt__UInt64__)->GetUInt64();
                    const Int64 beltConfigID = row->GetColumn(t_character::BeltConfigID__Int64__)->GetInt64();

                    const UInt64 slotShoes = row->GetColumn(t_character::SlotShoes__UInt64__)->GetUInt64();
                    const Int64 shoesConfigID = row->GetColumn(t_character::ShoesConfigID__Int64__)->GetInt64();

                    const UInt64 slotJewelry = row->GetColumn(t_character::SlotJewelry__UInt64__)->GetUInt64();
                    const Int64 jewelryConfigID = row->GetColumn(t_character::JewelryConfigID__Int64__)->GetInt64();

                    const UInt64 slotWeapon = row->GetColumn(t_character::SlotWeapon__UInt64__)->GetUInt64();
                    const Int64 weaponConfigID = row->GetColumn(t_character::WeaponConfigID__Int64__)->GetInt64();

                    IGameObject* goCharacter = g_GameWorld->CreateGameObject(eGameObjectType::Character, configId, characterId,
                        [&](IGameObject* const go) {
                            go->SetUInt64(Character::SceneGUIDUInt64, sceneGuid);
                            go->SetInt64(Character::LastPublicSceneConfigIDInt64, lastPublicSceneConfigId);
                            go->SetString(Character::NicknameString, g_PlayerManager->QueryPlayer(account)->Nickname());
                            go->SetUInt64(Character::AccountUInt64, account);
							go->SetInt64(Character::LevelInt64, level);
							go->SetInt64(Character::ExpInt64, exp);

                            go->SetUInt64(Character::SlotClothsUInt64, slotCloths);
                            go->SetInt64(Character::ClothsConfigInt64, clothsConfigID);

                            go->SetUInt64(Character::SlotPantsUInt64, slotPants);
                            go->SetInt64(Character::PantsConfigInt64, pantsConfigID);

                            go->SetUInt64(Character::SlotBeltUInt64, slotBelt);
                            go->SetInt64(Character::BeltConfigInt64, beltConfigID);

                            go->SetUInt64(Character::SlotShoesUInt64, slotShoes);
                            go->SetInt64(Character::ShoesConfigInt64, shoesConfigID);

                            go->SetUInt64(Character::SlotJewelryUInt64, slotJewelry);
                            go->SetInt64(Character::JewelryConfigInt64, jewelryConfigID);

                            go->SetUInt64(Character::SlotWeaponUInt64, slotWeapon);
                            go->SetInt64(Character::WeaponConfigInt64, weaponConfigID);

                            go->SetPosition(x, y);
                            go->SetAngle(angle);

                            go->SetFloat(Character::AtkFloat, characterConfig->InitialAtk);
                            go->SetFloat(Character::SpeedFloat, characterConfig->InitialSpeed);
                            go->SetFloat(Character::HpFloat, characterConfig->InitialHp);
                            go->SetFloat(Character::MaxHpFloat, characterConfig->InitialHp);
                            go->SetFloat(Character::CriticalFloat, characterConfig->InitialCritical);
                            go->SetFloat(Character::CriticalDamageFloat, characterConfig->InitialCriticalDamage);
                            go->SetFloat(Character::DefendFloat, characterConfig->InitialDefend);
                            go->SetFloat(Character::MagicDefendFloat, characterConfig->InitialMagicDefend);
                            go->SetFloat(Character::BloodFloat, characterConfig->InitialBlood);
                            go->SetFloat(Character::MagicAttackFloat, characterConfig->InitialMagicAttack);
                            go->SetFloat(Character::PhysicalPierceFloat, characterConfig->InitialPhysicalPierce);
                            go->SetFloat(Character::PhysicalPierceRateFloat, characterConfig->PhysicalPierceRate);
                            go->SetFloat(Character::MagicPierceFloat, characterConfig->InitialMagicPierce);
                            go->SetFloat(Character::MagicPierceRateFloat, characterConfig->MagicPierceRate);
                            go->SetFloat(Character::AffordFloat, characterConfig->InitialAfford);
                            go->SetFloat(Character::DodgeFloat, characterConfig->InitialDodge);
                            go->SetFloat(Character::ResilienceFloat, characterConfig->InitialResilience);
                            go->SetFloat(Character::VisualFieldFloat, characterConfig->VisualField);
                            go->SetFloat(Character::AtkSpeedFloat, characterConfig->InitialAtkSpeed3);
                            go->SetFloat(Character::MpFloat, characterConfig->InitialMp);
                            go->SetFloat(Character::MaxMpFloat, characterConfig->InitialMp);
                            go->SetInt32(Character::DamagePriorityInt32, (Int32)characterConfig->DamagePriority);
                            go->SetFloat(Character::HpRecoverFloat, characterConfig->InitialHpReserve);
                            go->SetFloat(Character::MpRecoverFloat, characterConfig->InitialMpReserve);
                        }
                    );

                    docker->_Characters.insert({ goCharacter->Guid(), goCharacter });
                }
            );

            callback(true);
            docker->SendCharacterInfosToClient();
            docker->TriggerLoaded();
            g_CharacterDockerEvent.Trigger(ICharacterManager::eCharacterDockerEvent::CharacterDockerLoaded, docker, nullptr);
        }
    );
}

void CharacterDocker::Load(const fCharacterDockerLoaded& callback) {
    __LoadCharacterDockerData__(this, _Account, callback);
}

void CharacterDocker::RegistCharacterEvent(const eCharacterEvent ev, const fCharacterEvent& callback, const char* file, const int line) {
    _Events.Register(ev, callback, file, line);
}

IGameObject* CharacterDocker::GetDeployCharacter() const {
    if (INVALID_GAMEOBJECT_ID == _DeployCharacterID)
        return nullptr;

    IGameObject* character = g_GameWorld->QueryGameObject(_DeployCharacterID);
    XASSERT(character, "wtf");
    return character;
}

void CharacterDocker::TriggerLoaded() {
    for (auto itor = _Characters.begin(); itor != _Characters.end(); itor++) {
        _Events.Trigger(eCharacterEvent::CharacterLoaded, this, itor->second);
    }
}

void CharacterDocker::SendCharacterInfosToClient() {
    if (_Characters.empty())
        return;

    oCharacterInfoPush characterInfoPush;
    for (auto itor = _Characters.begin(); itor != _Characters.end(); itor++) {
        oGameUnit* info = characterInfoPush.add_infos();
        PackageGameObjectToPB(info, itor->second, false);
    }

    g_PlayerManager->SendProtobufToPlayer(_Account, eServerID::CharacterInfoPush, characterInfoPush);
}

void CharacterDocker::SaveCharacterData(IGameObject* const character) {
    g_CacheClient->Update(_Account, eTable::t_character, t_character::Guid__UInt64__PrimaryUnique, character->Guid(),
        [&](ISQLCacheRow* const row) {
            row->GetColumn(t_character::Level__Int64__)->SetInt64(character->GetInt64(Character::LevelInt64));
            row->GetColumn(t_character::Exp__Int64__)->SetInt64(character->GetInt64(Character::ExpInt64));
            row->GetColumn(t_character::LastPublicSceneConfigID__Int64__)->SetInt64(character->GetInt64(Character::LastPublicSceneConfigIDInt64));
            row->GetColumn(t_character::SceneGUID__UInt64__)->SetUInt64(character->GetUInt64(Character::SceneGUIDUInt64));
            row->GetColumn(t_character::X__Float__)->SetFloat(character->Position().x);
            row->GetColumn(t_character::Y__Float__)->SetFloat(character->Position().y);
            row->GetColumn(t_character::Angle__Float__)->SetFloat(character->Angle());

            row->GetColumn(t_character::SlotCloths__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotClothsUInt64));
            row->GetColumn(t_character::ClothsConfigID__Int64__)->SetInt64(character->GetInt64(Character::ClothsConfigInt64));

            row->GetColumn(t_character::SlotPants__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotPantsUInt64));
            row->GetColumn(t_character::PantsConfigID__Int64__)->SetInt64(character->GetInt64(Character::PantsConfigInt64));

            row->GetColumn(t_character::SlotBelt__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotBeltUInt64));
            row->GetColumn(t_character::BeltConfigID__Int64__)->SetInt64(character->GetInt64(Character::BeltConfigInt64));

            row->GetColumn(t_character::SlotShoes__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotShoesUInt64));
            row->GetColumn(t_character::ShoesConfigID__Int64__)->SetInt64(character->GetInt64(Character::ShoesConfigInt64));

            row->GetColumn(t_character::SlotJewelry__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotJewelryUInt64));
            row->GetColumn(t_character::JewelryConfigID__Int64__)->SetInt64(character->GetInt64(Character::JewelryConfigInt64));

            row->GetColumn(t_character::SlotWeapon__UInt64__)->SetUInt64(character->GetUInt64(Character::SlotWeaponUInt64));
            row->GetColumn(t_character::WeaponConfigID__Int64__)->SetInt64(character->GetInt64(Character::WeaponConfigInt64));
        },
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (!succeed) {
                ErrorLog(g_Engine, "SaveCharacterData error");
            }
        }
    );
}

void CharacterDocker::SaveDocker() {
    for (auto itor = _Characters.begin(); itor != _Characters.end(); itor++) {
        SaveCharacterData(itor->second);
    }
}


