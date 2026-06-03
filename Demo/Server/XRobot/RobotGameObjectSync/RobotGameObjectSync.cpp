#include "RobotGameObjectSync.h"

Api::iEngine* g_Engine = nullptr;
IGameRobot* g_GameRobot = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;

static RobotGameObjectSync* s_Self = nullptr;

bool RobotGameObjectSync::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool RobotGameObjectSync::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameRobot, GameRobot);
    GetModule(g_Engine, g_GameWorld, GameWorld);

    g_GameRobot->RegistServerProtobuf(eServerID::GameObjectAppearPush, OnGameObjectAppearPush);
    g_GameRobot->RegistServerProtobuf(eServerID::GameObjectDisappearPush, OnGameObjectDisappearPush);
    g_GameRobot->RegistServerProtobuf(eServerID::CharacterInfoPush, OnCharacterInfoPush);
    return true;
}

bool RobotGameObjectSync::Destroy(Api::iEngine* const engine) {
    return true;
}

void RobotGameObjectSync::OnGameObjectAppearPush(IRobotSession* const session, const UInt64 sessionId, const oGameObjectAppearPush& push) {
    for (int i = 0; i < push.gos().size(); i++) {
        IGameObject* go = g_GameWorld->QueryGameObject(push.gos(i).guid());
        if (nullptr == go) {
            TraceLog(g_Engine, "Create GameObject %d, %lld, %llu", push.gos(i).type(), push.gos(i).configid(), push.gos(i).guid());
            go = g_GameWorld->CreateGameObject((eGameObjectType)push.gos(i).type(), push.gos(i).configid(), push.gos(i).guid(),
                [&](IGameObject* const go) {
                    for (int index = 0; index < push.gos(i).attributes().size(); index++) {
                        switch (push.gos(i).attributes(index).value()._type()) {
                        case eAttributeType::__UInt32__: {
                            go->SetUInt32(push.gos(i).attributes(index).index(), push.gos(i).attributes(index).value()._uint());
                            break;
                        }
                        case eAttributeType::__UInt64__: {
                            go->SetUInt64(push.gos(i).attributes(index).index(), push.gos(i).attributes(index).value()._uint());
                            break;
                        }
                        case eAttributeType::__Int32__: {
                            go->SetInt32(push.gos(i).attributes(index).index(), push.gos(i).attributes(index).value()._int());
                            break;
                        }
                        case eAttributeType::__Int64__: {
                            go->SetInt64(push.gos(i).attributes(index).index(), push.gos(i).attributes(index).value()._int());
                            break;
                        }
                        case eAttributeType::__Float__: {
                            go->SetFloat(push.gos(i).attributes(index).index(), push.gos(i).attributes(index).value()._float());
                            break;
                        }
                        case eAttributeType::__String__: {
                            go->SetString(push.gos(i).attributes(index).index(), push.gos(i).attributes(index).value()._string());
                            break;
                        }
                        case eAttributeType::__Bool__: {
                            go->SetBool(push.gos(i).attributes(index).index(), push.gos(i).attributes(index).value()._bool());
                            break;
                        }
                        case eAttributeType::__Vector2__: {
                            go->SetVector2(push.gos(i).attributes(index).index(), Vector2(push.gos(i).attributes(index).value()._vector2().x(), push.gos(i).attributes(index).value()._vector2().y()));
                            break;
                        }
                        case eAttributeType::__Vector3__: {
                            go->SetVector3(push.gos(i).attributes(index).index(),
                                Vector3(
                                    push.gos(i).attributes(index).value()._vector3().x(),
                                    push.gos(i).attributes(index).value()._vector3().y(),
                                    push.gos(i).attributes(index).value()._vector3().z()
                                )
                            );
                            break;
                        }
                        default:
                            XASSERT(false, "wtf");
                            break;
                        }
                    }
                }
            );
        }

        go->SetAngle(push.gos(i).angle());
        go->SetPosition(push.gos(i).position().x(), push.gos(i).position().y());

        if (go->Guid() == session->DeployCharacterID()) {
            //这里要进入状态机
        }
    }
}

void RobotGameObjectSync::OnGameObjectDisappearPush(IRobotSession* const session, const UInt64 sessionId, const oGameObjectDisappearPush& push) {

}

void RobotGameObjectSync::OnCharacterInfoPush(IRobotSession* const, const UInt64 sessionId, const oCharacterInfoPush& push) {
    for (int i = 0; i < push.infos().size(); i++) {
        IGameObject* go = g_GameWorld->QueryGameObject(push.infos(i).guid());
        if (nullptr == go) {
            TraceLog(g_Engine, "Create GameObject %d, %lld, %llu", push.infos(i).type(), push.infos(i).configid(), push.infos(i).guid());
            go = g_GameWorld->CreateGameObject((eGameObjectType)push.infos(i).type(), push.infos(i).configid(), push.infos(i).guid(),
                [&](IGameObject* go) {
                    for (int index = 0; index < push.infos(i).attributes().size(); index++) {
                        switch (push.infos(i).attributes(index).value()._type()) {
                        case eAttributeType::__UInt32__: {
                            go->SetUInt32(push.infos(i).attributes(index).index(), push.infos(i).attributes(index).value()._uint());
                            break;
                        }
                        case eAttributeType::__UInt64__: {
                            go->SetUInt64(push.infos(i).attributes(index).index(), push.infos(i).attributes(index).value()._uint());
                            break;
                        }
                        case eAttributeType::__Int32__: {
                            go->SetInt32(push.infos(i).attributes(index).index(), push.infos(i).attributes(index).value()._int());
                            break;
                        }
                        case eAttributeType::__Int64__: {
                            go->SetInt64(push.infos(i).attributes(index).index(), push.infos(i).attributes(index).value()._int());
                            break;
                        }
                        case eAttributeType::__Float__: {
                            go->SetFloat(push.infos(i).attributes(index).index(), push.infos(i).attributes(index).value()._float());
                            break;
                        }
                        case eAttributeType::__String__: {
                            go->SetString(push.infos(i).attributes(index).index(), push.infos(i).attributes(index).value()._string());
                            break;
                        }
                        case eAttributeType::__Bool__: {
                            go->SetBool(push.infos(i).attributes(index).index(), push.infos(i).attributes(index).value()._bool());
                            break;
                        }
                        case eAttributeType::__Vector2__: {
                            go->SetVector2(push.infos(i).attributes(index).index(), Vector2(push.infos(i).attributes(index).value()._vector2().x(), push.infos(i).attributes(index).value()._vector2().y()));
                            break;
                        }
                        case eAttributeType::__Vector3__: {
                            go->SetVector3(push.infos(i).attributes(index).index(),
                                Vector3(
                                    push.infos(i).attributes(index).value()._vector3().x(),
                                    push.infos(i).attributes(index).value()._vector3().y(),
                                    push.infos(i).attributes(index).value()._vector3().z()
                                )
                            );
                            break;
                        }
                        default:
                            XASSERT(false, "wtf");
                            break;
                        }
                    }
                }
            );
        }



        go->SetAngle(push.infos(i).angle());
        go->SetPosition(push.infos(i).position().x(), push.infos(i).position().y());
    }
}
