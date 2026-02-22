#include "Scene.h"
#include "SceneManager.h"

static XPool<Scene> s_Pool;

enum eSceneTimerID {
    UpdateAoiPosition = 1
};
static const int s_UpdateAoiPositionInterval = 100;

Scene::Scene(const UInt64 guid, const AutoConfig::Struct::Scene* config, const std::vector<Vector2>& enterPositions, const ITilemap* tilemap, const char* file, const int line)
    : _SceneID(guid), _Config(config), _EnterPositions(enterPositions), _Tilemap(tilemap), _GroundLayer(tilemap->GetLayer("Ground")), _Aoi(_SceneID, AutoConfig::Const::Aoi::AoiGridSize), _EnterPositionIndex(0), _Relasing(false), _File(file), _Line(line) {
    _BroadCastTemp.reserve(2048);
}

void Scene::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eSceneTimerID::UpdateAoiPosition: {
        IGameObject* go = g_GameWorld->QueryGameObject(context);
        _Aoi.UpdatePosition(context, go->Position(),
            [&](const std::vector<UInt64>& appears) {
                if (appears.empty())
                    return;

                if (go) {
                    oGameObjectAppearPush pushToSelf, pushToOthers;
                    oGameUnit* pbToOther = pushToOthers.add_gos();
                    PackageGameObjectToPB(pbToOther, go, go->Is(eGameObjectType::Creature));
                    s_BroadCastTemp.clear();
                    for (int i = 0; i < appears.size(); i++) {
                        IGameObject* other = g_GameWorld->QueryGameObject(appears[i]);
                        if (other) {
                            if (go->Type() == eGameObjectType::Character) {
                                oGameUnit* pb = pushToSelf.add_gos();
                                PackageGameObjectToPB(pb, other, other->Is(eGameObjectType::Creature));
                            }

                            if (other->Type() == eGameObjectType::Character) {
                                s_BroadCastTemp.push_back(other->GetUInt64(Character::AccountUInt64));
                            }
                        }
                        else {
                            ErrorLog(g_Engine, "Cannot find game object %llu", appears[i]);
                        }
                    }

                    if (go->Type() == eGameObjectType::Character) {
                        g_PlayerManager->SendProtobufToPlayer(go->GetUInt64(Character::AccountUInt64), eServerID::GameObjectAppearPush, pushToSelf);
                    }

                    g_PlayerManager->Broadcast(s_BroadCastTemp, eServerID::GameObjectAppearPush, pushToOthers);
                }

                _AoiEvents.Trigger(eAoiEvent::Appear, this, context, appears);
            },
            [&](const std::vector<UInt64>& disappears) {
                if (disappears.empty())
                    return;

                if (go) {
                    oGameObjectDisappearPush pushToSelf, pushToOthers;
                    pushToOthers.add_guids(context);
                    s_BroadCastTemp.clear();
                    for (int i = 0; i < disappears.size(); i++) {
                        IGameObject* other = g_GameWorld->QueryGameObject(disappears[i]);
                        if (other) {
                            if (go->Type() == eGameObjectType::Character) {
                                pushToSelf.add_guids(disappears[i]);
                            }

                            if (other->Type() == eGameObjectType::Character) {
                                s_BroadCastTemp.push_back(other->GetUInt64(Character::AccountUInt64));
                            }
                        }
                        else {
                            ErrorLog(g_Engine, "Cannot find game object %llu", disappears[i]);
                        }
                    }

                    if (go->Type() == eGameObjectType::Character) {
                        g_PlayerManager->SendProtobufToPlayer(go->GetUInt64(Character::AccountUInt64), eServerID::GameObjectDisappearPush, pushToSelf);
                    }

                    g_PlayerManager->Broadcast(s_BroadCastTemp, eServerID::GameObjectDisappearPush, pushToOthers);
                }

                _AoiEvents.Trigger(eAoiEvent::Disappear, this, context, disappears);
            }
        );
        break;
    }
    }
}

Scene* Scene::CreateScene(const UInt64 guid, const AutoConfig::Struct::Scene* config, const ITilemap* tilemapHandle, const char* file, const int line) {
	TraceLog(g_Engine, "CreateScene Scene %s %lld", config->Name, config->ID);
    std::vector<Vector2> enterPositions;
    XASSERT(config->StartPoint.size() % 2 == 0, "wtf");
    for (int i = 0; i < config->StartPoint.size(); i += 2) {
        enterPositions.insert(enterPositions.begin() + SafeTools::Rand() % (enterPositions.size() + 1), Vector2(config->StartPoint[i], config->StartPoint[i + 1]));
    }

    Scene* scene = XPOOL_CREATE(s_Pool, guid, config, enterPositions, tilemapHandle, file, line);
    return scene;
}

void Scene::ReleaseScene(Scene* scene) {
    TraceLog(g_Engine, "Release Scene %llu", scene->SceneID());
    std::vector<UInt64> temp(scene->_Guids.begin(), scene->_Guids.end());
    scene->_Guids.clear();
    for (auto it = temp.begin(); it != temp.end(); it++) {
        IGameObject* go = g_GameWorld->QueryGameObject(*it);
        if (go && go->Type() == eGameObjectType::Character) {
            IScene* publicScene = g_SceneManager->QueryPublicScene(go->GetInt64(Character::LastPublicSceneConfigIDInt64));
            publicScene->EnterScene(*it, nullptr, true);

            g_Fsm->ForceEnterState(go->Guid(), AutoConfig::Enum::eState::Idle, go->Position(), go->Angle());
            go->SetFloat(Entity::HpFloat, go->GetFloat(Entity::MaxHpFloat));
            go->SetFloat(Entity::MpFloat, go->GetFloat(Entity::MaxMpFloat));

            oGameResultInfoPush push;
            push.set_success(false);
            g_PlayerManager->SendProtobufToPlayer(go->GetUInt64(Character::AccountUInt64), eServerID::GameResultInfoPush, push);
        }
        STOP_TIMER(g_Engine, scene, eSceneTimerID::UpdateAoiPosition, *it);
    }

    XPOOL_RELEASE(s_Pool, scene);
}

void Scene::RegistAoiEvent(const eAoiEvent ev, const fAoiEvent callback, const char* file, const int line) {
    _AoiEvents.Register(ev, callback, file, line);
}

void Scene::RegistEvent(const eEvent ev, const fEvent callback, const char* file, const int line) {
    _Events.Register(ev, callback, file, line);
}

void Scene::EntityEnterScene(IGameObject* const entity, const Vector2& position, const fAoiSingle& appearFun, const bool reset) {
	IScene* scene = g_SceneManager->QuerySceneBySceneID(entity->GetUInt64(Entity::SceneGUIDUInt64));
	if (nullptr != scene) {
		scene->ExitScene(entity->Guid());
	}

    entity->SetUInt64(Entity::SceneGUIDUInt64, this->_SceneID);

	if (_Config->SceneType == AutoConfig::Enum::eSceneType::Public
		|| _Config->SceneType == AutoConfig::Enum::eSceneType::PublicBattle) {
        entity->SetInt64(Entity::LastPublicSceneConfigIDInt64, _Config->ID);
	}
	else {
        entity->SetUInt64(Entity::CopySceneGuidUInt64, this->_SceneID);
	}

    entity->SetUInt64(Entity::SceneGUIDUInt64, this->_SceneID);

    if (_Config->SceneType == AutoConfig::Enum::eSceneType::Public
        || _Config->SceneType == AutoConfig::Enum::eSceneType::PublicBattle) {
        entity->SetInt64(Entity::LastPublicSceneConfigIDInt64, _Config->ID);
       
    }
    else {
        entity->SetUInt64(Entity::CopySceneGuidUInt64, this->_SceneID);
        entity->SetUInt64(Entity::BattleStartTimeUInt64, SafeSystem::Time::GetMilliSecond());
    }
    _Guids.insert(entity->Guid());

    if (entity->Type() == eGameObjectType::Character) {
        oSceneInfoPush sceneInfoPush;
        sceneInfoPush.mutable_info()->set_configid(_Config->ID);
        sceneInfoPush.mutable_info()->set_sceneid(SceneID());
        g_PlayerManager->SendProtobufToPlayer(entity->GetUInt64(Character::AccountUInt64), eServerID::SceneInfoPush, sceneInfoPush);

        if (reset) {
            entity->SetPosition(position);
            oCorrectPosition ocp;
            ocp.mutable_position()->set_x(entity->Position().x);
            ocp.mutable_position()->set_y(entity->Position().y);

            g_PlayerManager->SendProtobufToPlayer(entity->GetUInt64(Character::AccountUInt64), eServerID::CorrectPosition, ocp);
        }
        else {
            if (!_GroundLayer->HasGrid(entity->Position())) {
                entity->SetPosition(position);
                oCorrectPosition ocp;
                ocp.mutable_position()->set_x(entity->Position().x);
                ocp.mutable_position()->set_y(entity->Position().y);

                g_PlayerManager->SendProtobufToPlayer(entity->GetUInt64(Character::AccountUInt64), eServerID::CorrectPosition, ocp);
            }
        }
    }


    s_BroadCastTemp.clear();
    oGameObjectAppearPush toSelf, toOther;
    oGameUnit* gu = toOther.add_gos();
    PackageGameObjectToPB(gu, entity, entity->Is(eGameObjectType::Creature));
    _Aoi.Insert(entity->Guid(), entity->Position(), AutoConfig::Const::Aoi::DefaultVisualSize,
        [&](const UInt64 other) {
            if (appearFun)
                appearFun(this, entity->Guid(), other);
            IGameObject* go = g_GameWorld->QueryGameObject(other);
            if (go) {
                if (go->Type() == eGameObjectType::Character) {
                    s_BroadCastTemp.push_back(go->GetUInt64(Character::AccountUInt64));
                }

                if (entity->Type() == eGameObjectType::Character) {
                    oGameUnit* otherGu = toSelf.add_gos();
                    PackageGameObjectToPB(otherGu, go, go->Is(eGameObjectType::Creature));
                }
            }
            else {
                ErrorLog(g_Engine, "Where is other %llu", other);
            }
        }
    );

    g_PlayerManager->Broadcast(s_BroadCastTemp, eServerID::GameObjectAppearPush, toOther);
    if (entity->Type() == eGameObjectType::Character) {
        g_PlayerManager->SendProtobufToPlayer(entity->GetUInt64(Character::AccountUInt64), eServerID::GameObjectAppearPush, toSelf);
    }

    START_TIMER(g_Engine, this, eSceneTimerID::UpdateAoiPosition, 500, Api::Unlimited, 500, entity->Guid());
    TraceLog(g_Engine, "Guid %llu Enter Scene %llu %s", entity->Guid(), _SceneID, _Config->Name);
    _Events.Trigger(eEvent::Enter, this, entity->Guid());
}

void Scene::EnterScene(ITeam* const team) {
	if (_Relasing) {
        return;
    }

    if (_Config->SceneType != AutoConfig::Enum::eSceneType::BattleCopy) {
        ErrorLog(g_Engine, "Scene %llu %s cannot enter by team", _SceneID, _Config->Name);
        return;
    }

    for (auto it = team->GetMembers().begin(); it != team->GetMembers().end(); it++) {
        if (_Guids.find(*it) != _Guids.end()) {
            ErrorLog(g_Engine, "Scene %llu %s already contain guid %llu", _SceneID, _Config->Name, *it);
            XASSERT(false, "wtf");
            return;
        }

        IGameObject* character = g_GameWorld->QueryGameObject(*it);
        if (nullptr == character) {
            ErrorLog(g_Engine, "where is character %llu", *it);
            return;
        }

        IScene* scene = g_SceneManager->QueryCopyScene(character);
        if (nullptr != scene) {
            ErrorLog(g_Engine, "Character %llu already in copy scene %llu", character->Guid(), scene->SceneID());
            return;
        }
    }

    for (auto it = team->GetMembers().begin(); it != team->GetMembers().end(); it++) {
        IGameObject* const character = g_GameWorld->QueryGameObject(*it);
        if (character)
            EntityEnterScene(character, _EnterPositions[_EnterPositionIndex % _EnterPositions.size()], nullptr, true);
    }
    _EnterPositionIndex++;
}

void Scene::EnterScene(const UInt64 guid, const fAoiSingle& appearFun, const bool reset) {
	if (_Relasing) {
		return;
	}

    if (_Guids.find(guid) != _Guids.end()) {
        ErrorLog(g_Engine, "Scene %llu %s already contain guid %llu", _SceneID, _Config->Name, guid);
        XASSERT(false, "wtf");
        return;
	}

	IGameObject* selfGo = g_GameWorld->QueryGameObject(guid);


    EntityEnterScene(selfGo, _EnterPositions[_EnterPositionIndex % _EnterPositions.size()], appearFun, reset);
    _EnterPositionIndex++;
}

void Scene::ExitScene(const UInt64 guid, const fAoiSingle& disappearFun) {
    if (_Relasing) {
        return;
    }
    
    if (_Guids.find(guid) == _Guids.end()) {
        return;
    }
    _Guids.erase(guid);

    IGameObject* selfGo = g_GameWorld->QueryGameObject(guid);
    selfGo->SetUInt64(Entity::SceneGUIDUInt64, INVALID_SCENE_GUID);

    s_BroadCastTemp.clear();
    oGameObjectDisappearPush toSelf, toOther;
    toOther.add_guids(guid);

    if (_Config->SceneType == AutoConfig::Enum::eSceneType::BattleCopy) {
        selfGo->SetUInt64(Entity::CopySceneGuidUInt64, INVALID_SCENE_GUID);
    }

    _Aoi.Remove(guid,
        [&](const UInt64 other) {
            if (disappearFun)
                disappearFun(this, guid, other);

            IGameObject* go = g_GameWorld->QueryGameObject(other);
            if (go) {
                if (go->Type() == eGameObjectType::Character) {
                    s_BroadCastTemp.push_back(go->GetUInt64(Character::AccountUInt64));
                }

                if (selfGo->Type() == eGameObjectType::Character) {
                    toSelf.add_guids(other);
                }
            }
            else {
                ErrorLog(g_Engine, "Where is other %llu", other);
            }
        }
    );

    g_PlayerManager->Broadcast(s_BroadCastTemp, eServerID::GameObjectDisappearPush, toOther);

    if (selfGo->Type() == eGameObjectType::Character) {
        g_PlayerManager->SendProtobufToPlayer(selfGo->GetUInt64(Character::AccountUInt64), eServerID::GameObjectDisappearPush, toSelf);
    }

    STOP_TIMER(g_Engine, this, eSceneTimerID::UpdateAoiPosition, guid);
    TraceLog(g_Engine, "Guid %llu Exit Scene %llu %s", guid, _SceneID, _Config->Name);
    _Events.Trigger(eEvent::Leave, this, guid);

    if (_Config->SceneType == AutoConfig::Enum::eSceneType::BattleCopy) {
		_Relasing = true;
		for (auto it = _Guids.begin(); it != _Guids.end(); it++) {
			IGameObject* const entity = g_GameWorld->QueryGameObject(*it);
			if (entity) {
				if (entity->Type() == eGameObjectType::Character) {
					_Relasing = false;
				}
			}
			else {
				ErrorLog(g_Engine, "where is entity %llu", *it);
			}
		}

		if (_Relasing) {
			g_SceneManager->ReleaseCopyScene(this);
		}
    }
}

void Scene::GetVisual(const UInt64 guid, const fVisualCallback& callback) const {
    _Aoi.QueryVisual(guid, callback);
}

bool Scene::CanMove(const float x, const float y) const {
    return _GroundLayer->HasGrid(x, y);
}

void Scene::Broadcast(const eServerID id, const::google::protobuf::Message& pb) {
    _BroadCastTemp.clear();
    for (auto itor = _Guids.begin(); itor != _Guids.end(); itor++) {
        IGameObject* go = g_GameWorld->QueryGameObject(*itor);
        if (go && go->Type() == eGameObjectType::Character) {
            _BroadCastTemp.push_back(go->Guid());
        }
    }

    g_PlayerManager->Broadcast(_BroadCastTemp, id, pb);
}

void Scene::GetVisual(const Vector2& pos, const float radius, const fGridVisualCallback& callback) {
    _Aoi.GetGridsVisual(pos, radius, callback);
}
