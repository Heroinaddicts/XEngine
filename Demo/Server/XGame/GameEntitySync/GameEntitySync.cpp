#include "GameEntitySync.h"

Api::iEngine* g_Engine = nullptr;
IGameWorld* g_GameWorld = nullptr;
ICharacterManager* g_CharacterManager = nullptr;
ISceneManager* g_SceneManager = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
IFsm* g_Fsm = nullptr;

static GameEntitySync* s_Self = nullptr;

static std::vector<UInt64> s_BroadcastTmpe;

bool GameEntitySync::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;

    s_BroadcastTmpe.reserve(2048);
    return true;
}

bool GameEntitySync::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_CharacterManager, CharacterManager);
    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_Fsm, Fsm);

    g_Fsm->RegistFsmEvent(
        [](const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState) {
            switch (ev) {
            case IFsm::eFsmEvent::FsmCreated: {
                g_Fsm->RegistStateChanged(guid, OnEntityStateChanged);
                break;
            }
            }
        }
    );

    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, GameEntitySync::OnUInt32AttributeChanged);
    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, GameEntitySync::OnInt32AttributeChanged);
    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, GameEntitySync::OnUInt64AttributeChanged);
    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, GameEntitySync::OnInt64AttributeChanged);
    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, GameEntitySync::OnFloatAttributeChanged);
    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, GameEntitySync::OnBoolAttributeChanged);
    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, GameEntitySync::OnStringAttributeChanged);
    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, GameEntitySync::OnVector2AttributeChanged);
    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, GameEntitySync::OnVector3AttributeChanged);
    return true;
}

bool GameEntitySync::Destroy(Api::iEngine* const engine) {
    return true;
}

void GameEntitySync::OnUInt32AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const UInt32 oldValue, const UInt32 newValue) {
    if ((sync && go->Is(eGameObjectType::Character)) || share) {
        oGameObjectAttributeSync push;
        push.set_guid(go->Guid());
        oAttribute* attr = push.mutable_attribute();
        attr->set_index(index);
        attr->mutable_value()->set__uint(newValue);

        if (sync && go->Is(eGameObjectType::Character)) {
            g_CharacterManager->SendProtobuf(go->Guid(), eServerID::GameObjectAttributeSync, push);
        }

        if (share) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (nullptr == scene) {
                TraceLog(g_Engine, "can not find any scene for game object %llu", go->Guid());
                return;
            }

            s_BroadcastTmpe.clear();
            scene->GetVisual(go->Guid(),
                [&](const std::unordered_set<UInt64>& visual) {
                    for (auto it = visual.begin(); it != visual.end(); it++) {
                        IGameObject* g = g_GameWorld->QueryGameObject(*it);
                        if (g && g->Is(eGameObjectType::Character)) {
                            s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }
            );

            g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::GameObjectAttributeSync, push);
        }
    }
}

void GameEntitySync::OnInt32AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue) {
    if ((sync && go->Is(eGameObjectType::Character)) || share) {
        oGameObjectAttributeSync push;
        push.set_guid(go->Guid());
        oAttribute* attr = push.mutable_attribute();
        attr->set_index(index);
        attr->mutable_value()->set__int(newValue);

        if (sync && go->Is(eGameObjectType::Character)) {
            g_CharacterManager->SendProtobuf(go->Guid(), eServerID::GameObjectAttributeSync, push);
        }

        if (share) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (nullptr == scene) {
                TraceLog(g_Engine, "can not find any scene for game object %llu", go->Guid());
                return;
            }

            s_BroadcastTmpe.clear();
            scene->GetVisual(go->Guid(),
                [&](const std::unordered_set<UInt64>& visual) {
                    for (auto it = visual.begin(); it != visual.end(); it++) {
                        IGameObject* g = g_GameWorld->QueryGameObject(*it);
                        if (g && g->Is(eGameObjectType::Character)) {
                            s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }
            );

            g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::GameObjectAttributeSync, push);
        }
    }
}

void GameEntitySync::OnUInt64AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const UInt64 oldValue, const UInt64 newValue) {
    if ((sync && go->Is(eGameObjectType::Character)) || share) {
        oGameObjectAttributeSync push;
        push.set_guid(go->Guid());
        oAttribute* attr = push.mutable_attribute();
        attr->set_index(index);
        attr->mutable_value()->set__uint(newValue);

        if (sync && go->Is(eGameObjectType::Character)) {
            g_CharacterManager->SendProtobuf(go->Guid(), eServerID::GameObjectAttributeSync, push);
        }

        if (share) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (nullptr == scene) {
                TraceLog(g_Engine, "can not find any scene for game object %llu", go->Guid());
                return;
            }

            s_BroadcastTmpe.clear();
            scene->GetVisual(go->Guid(),
                [&](const std::unordered_set<UInt64>& visual) {
                    for (auto it = visual.begin(); it != visual.end(); it++) {
                        IGameObject* g = g_GameWorld->QueryGameObject(*it);
                        if (g && g->Is(eGameObjectType::Character)) {
                            s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }
            );

            g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::GameObjectAttributeSync, push);
        }
    }
}

void GameEntitySync::OnInt64AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Int64 oldValue, const Int64 newValue) {
    if ((sync && go->Is(eGameObjectType::Character)) || share) {
        oGameObjectAttributeSync push;
        push.set_guid(go->Guid());
        oAttribute* attr = push.mutable_attribute();
        attr->set_index(index);
        attr->mutable_value()->set__int(newValue);

        if (sync && go->Is(eGameObjectType::Character)) {
            g_CharacterManager->SendProtobuf(go->Guid(), eServerID::GameObjectAttributeSync, push);
        }

        if (share) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (nullptr == scene) {
                TraceLog(g_Engine, "can not find any scene for game object %llu", go->Guid());
                return;
            }

            s_BroadcastTmpe.clear();
            scene->GetVisual(go->Guid(),
                [&](const std::unordered_set<UInt64>& visual) {
                    for (auto it = visual.begin(); it != visual.end(); it++) {
                        IGameObject* g = g_GameWorld->QueryGameObject(*it);
                        if (g && g->Is(eGameObjectType::Character)) {
                            s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }
            );

            g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::GameObjectAttributeSync, push);
        }
    }
}

void GameEntitySync::OnFloatAttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue) {
    if ((sync && go->Is(eGameObjectType::Character)) || share) {
        oGameObjectAttributeSync push;
        push.set_guid(go->Guid());
        oAttribute* attr = push.mutable_attribute();
        attr->set_index(index);
        attr->mutable_value()->set__float(newValue);

        if (sync && go->Is(eGameObjectType::Character)) {
            g_CharacterManager->SendProtobuf(go->Guid(), eServerID::GameObjectAttributeSync, push);
        }

        if (share) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (nullptr == scene) {
                TraceLog(g_Engine, "can not find any scene for game object %llu", go->Guid());
                return;
            }

            s_BroadcastTmpe.clear();
            scene->GetVisual(go->Guid(),
                [&](const std::unordered_set<UInt64>& visual) {
                    for (auto it = visual.begin(); it != visual.end(); it++) {
                        IGameObject* g = g_GameWorld->QueryGameObject(*it);
                        if (g && g->Is(eGameObjectType::Character)) {
                            s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }
            );

            g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::GameObjectAttributeSync, push);
        }
    }
}

void GameEntitySync::OnBoolAttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const bool oldValue, const bool newValue) {
    if ((sync && go->Is(eGameObjectType::Character)) || share) {
        oGameObjectAttributeSync push;
        push.set_guid(go->Guid());
        oAttribute* attr = push.mutable_attribute();
        attr->set_index(index);
        attr->mutable_value()->set__bool(newValue);

        if (sync && go->Is(eGameObjectType::Character)) {
            g_CharacterManager->SendProtobuf(go->Guid(), eServerID::GameObjectAttributeSync, push);
        }

        if (share) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (nullptr == scene) {
                TraceLog(g_Engine, "can not find any scene for game object %llu", go->Guid());
                return;
            }

            s_BroadcastTmpe.clear();
            scene->GetVisual(go->Guid(),
                [&](const std::unordered_set<UInt64>& visual) {
                    for (auto it = visual.begin(); it != visual.end(); it++) {
                        IGameObject* g = g_GameWorld->QueryGameObject(*it);
                        if (g && g->Is(eGameObjectType::Character)) {
                            s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }
            );

            g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::GameObjectAttributeSync, push);
        }
    }
}

void GameEntitySync::OnStringAttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const std::string& oldValue, const std::string& newValue) {
    if ((sync && go->Is(eGameObjectType::Character)) || share) {
        oGameObjectAttributeSync push;
        push.set_guid(go->Guid());
        oAttribute* attr = push.mutable_attribute();
        attr->set_index(index);
        attr->mutable_value()->set__string(newValue);

        if (sync && go->Is(eGameObjectType::Character)) {
            g_CharacterManager->SendProtobuf(go->Guid(), eServerID::GameObjectAttributeSync, push);
        }

        if (share) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (nullptr == scene) {
                TraceLog(g_Engine, "can not find any scene for game object %llu", go->Guid());
                return;
            }

            s_BroadcastTmpe.clear();
            scene->GetVisual(go->Guid(),
                [&](const std::unordered_set<UInt64>& visual) {
                    for (auto it = visual.begin(); it != visual.end(); it++) {
                        IGameObject* g = g_GameWorld->QueryGameObject(*it);
                        if (g && g->Is(eGameObjectType::Character)) {
                            s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }
            );

            g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::GameObjectAttributeSync, push);
        }
    }
}

void GameEntitySync::OnVector2AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Vector2& oldValue, const Vector2& newValue) {
    if ((sync && go->Is(eGameObjectType::Character)) || share) {
        oGameObjectAttributeSync push;
        push.set_guid(go->Guid());
        oAttribute* attr = push.mutable_attribute();
        attr->set_index(index);

        oVector2* vector2 = new oVector2();
        vector2->set_x(newValue.x);
        vector2->set_y(newValue.y);
        attr->mutable_value()->set_allocated__vector2(vector2);

        if (sync && go->Is(eGameObjectType::Character)) {
            g_CharacterManager->SendProtobuf(go->Guid(), eServerID::GameObjectAttributeSync, push);
        }

        if (share) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (nullptr == scene) {
                TraceLog(g_Engine, "can not find any scene for game object %llu", go->Guid());
                return;
            }

            s_BroadcastTmpe.clear();
            scene->GetVisual(go->Guid(),
                [&](const std::unordered_set<UInt64>& visual) {
                    for (auto it = visual.begin(); it != visual.end(); it++) {
                        IGameObject* g = g_GameWorld->QueryGameObject(*it);
                        if (g && g->Is(eGameObjectType::Character)) {
                            s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }
            );

            g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::GameObjectAttributeSync, push);
        }
    }
}

void GameEntitySync::OnVector3AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Vector3& oldValue, const Vector3& newValue) {
    if ((sync && go->Is(eGameObjectType::Character)) || share) {
        oGameObjectAttributeSync push;
        push.set_guid(go->Guid());
        oAttribute* attr = push.mutable_attribute();
        attr->set_index(index);

        oVector2* vector2 = new oVector2();
        vector2->set_x(newValue.x);
        vector2->set_y(newValue.y);
        attr->mutable_value()->set_allocated__vector2(vector2);

        if (sync && go->Is(eGameObjectType::Character)) {
            g_CharacterManager->SendProtobuf(go->Guid(), eServerID::GameObjectAttributeSync, push);
        }

        if (share) {
            IScene* scene = g_SceneManager->QuerySceneByEntityGuid(go->Guid());
            if (nullptr == scene) {
                TraceLog(g_Engine, "can not find any scene for game object %llu", go->Guid());
                return;
            }

            s_BroadcastTmpe.clear();
            scene->GetVisual(go->Guid(),
                [&](const std::unordered_set<UInt64>& visual) {
                    for (auto it = visual.begin(); it != visual.end(); it++) {
                        IGameObject* g = g_GameWorld->QueryGameObject(*it);
                        if (g && g->Is(eGameObjectType::Character)) {
                            s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                        }
                    }
                }
            );

            g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::GameObjectAttributeSync, push);
        }
    }
}

void GameEntitySync::OnEntityStateChanged(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& pos, const float angle) {
    IGameObject* const entity = g_GameWorld->QueryGameObject(guid);
    oStateChangePush push;
    push.set_guid(guid);
    oStateChange* change = push.mutable_change();
    change->set_angle(angle);
    change->set_state((int)state);
    change->mutable_pos()->set_x(pos.x);
    change->mutable_pos()->set_y(pos.y);

    if (entity->Is(eGameObjectType::Character)) {
        g_CharacterManager->SendProtobuf(entity->Guid(), eServerID::StateChangePush, push);
    }


    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(entity->Guid());
    if (nullptr == scene) {
        TraceLog(g_Engine, "can not find any scene for game object %llu", entity->Guid());
        return;
    }

    s_BroadcastTmpe.clear();
    scene->GetVisual(entity->Guid(),
        [&](const std::unordered_set<UInt64>& visual) {
            for (auto it = visual.begin(); it != visual.end(); it++) {
                IGameObject* g = g_GameWorld->QueryGameObject(*it);
                if (g && g->Is(eGameObjectType::Character)) {
                    s_BroadcastTmpe.push_back(g->GetUInt64(Character::AccountUInt64));
                }
            }
        }
    );

    g_PlayerManager->Broadcast(s_BroadcastTmpe, eServerID::StateChangePush, push);
}
