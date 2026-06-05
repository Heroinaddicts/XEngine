#include "RobotMovement.h"

#include <list>

Api::iEngine* g_Engine = nullptr;
IGameRobot* g_GameRobot = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
IRobotSceneManager* g_RobotSceneManager = nullptr;

static RobotMovement* s_Self = nullptr;

struct MoveInfo {
    std::list<Vector2> _Path;
    IRobotMovement::fArrive _Callback;

    MoveInfo() : _Callback(nullptr) {}
};
static std::unordered_map<UInt64, MoveInfo> s_MoveInfoMap;

enum eMoveTimer {
    Arrived = 0,
    UpdatePosition = 1
};

bool RobotMovement::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool RobotMovement::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameRobot, GameRobot);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_RobotSceneManager, RobotSceneManager);

    g_GameRobot->RegistEvent(IGameRobot::eEvent::RobotCreated, OnRobotCreated);
    g_GameRobot->RegistEvent(IGameRobot::eEvent::RobotRelease, OnRobotRelease);
    return true;
}

bool RobotMovement::Destroy(Api::iEngine* const engine) {
    return true;
}

void RobotMovement::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    IGameObject* character = g_GameWorld->QueryGameObject(context);
    XASSERT(character, "wtf");
    switch (id) {
    case eMoveTimer::UpdatePosition: {
        float distance = (timestamp - character->GetUInt64(Character::LastUpdatePositionTickUInt64)) / 1000.0f * character->GetFloat(Character::SpeedFloat);
        character->SetPosition(Vector2::CalcPosition(character->Position(), character->Angle(), distance));
        //TraceLog(g_Engine, "Character UpdatePosition %f, %f", character->Position().x, character->Position().y);
        break;
    }
    case eMoveTimer::Arrived: {
        auto itor = s_MoveInfoMap.find(context);
        XASSERT(s_MoveInfoMap.end() != itor, "wtf");

        character->SetPosition(*itor->second._Path.begin());

        itor->second._Path.erase(itor->second._Path.begin());
        if (itor->second._Path.empty()) {
            STOP_TIMER(g_Engine, s_Self, eMoveTimer::Arrived, context);
            STOP_TIMER(g_Engine, s_Self, eMoveTimer::UpdatePosition, context);
            itor->second._Callback(context, character->Position(), true);

            IRobotSession* session = g_GameRobot->QuerySession(character->GetUInt64(Character::AccountUInt64));
            XASSERT(session, "wtf");
            if (session) {
                oStateChange change;
                change.set_state((int)AutoConfig::Enum::eState::Idle);
                change.set_angle(character->Angle());
                change.mutable_pos()->set_x(character->Position().x);
                change.mutable_pos()->set_y(character->Position().y);

                session->SendProtobuf(eClientID::StateChange, change);
            }
            return;
        }

        Vector2 next = *itor->second._Path.begin();
        float distance = (next - character->Position()).Length();
        float speed = character->GetFloat(Character::SpeedFloat);
        int delay = (distance / speed) * 1000;
        START_TIMER(g_Engine, s_Self, eMoveTimer::Arrived, delay, 1, delay, character->Guid());
        character->SetAngle((next - character->Position()).Angle());
        character->SetUInt64(Character::LastUpdatePositionTickUInt64, timestamp);

        IRobotSession* session = g_GameRobot->QuerySession(character->GetUInt64(Character::AccountUInt64));
        XASSERT(session, "wtf");
        if (session) {
            oStateChange change;
            change.set_state((int)AutoConfig::Enum::eState::Moving);
            change.set_angle(character->Angle());
            change.mutable_pos()->set_x(character->Position().x);
            change.mutable_pos()->set_y(character->Position().y);

            session->SendProtobuf(eClientID::StateChange, change);
        }
        break;
    }
    }
}

void RobotMovement::MoveTo(const UInt64 characterId, const Vector2& pos, const fArrive& arrive) {

    auto itor = s_MoveInfoMap.find(characterId);
    if (s_MoveInfoMap.end() == itor) {
        itor = s_MoveInfoMap.insert({ characterId, MoveInfo() }).first;
    }

    IGameObject* character = g_GameWorld->QueryGameObject(characterId);
    if (nullptr != itor->second._Callback) {
        if (g_Engine->GetTimerApi()->IsExistsTimer(s_Self, eMoveTimer::Arrived, characterId)) {
            itor->second._Callback(characterId, character->Position(), itor->second._Path.empty());
        }
    }
    STOP_TIMER(g_Engine, s_Self, eMoveTimer::Arrived, characterId);
    STOP_TIMER(g_Engine, s_Self, eMoveTimer::UpdatePosition, characterId);

    if (character) {
        IRobotScene* scene = g_RobotSceneManager->QueryCharacterScene(character->Guid());
        XASSERT(scene, "wtf");

        scene->_GroundTileLayer->FindPath(character->Position(), pos,
            [&](const bool succeed, const std::vector<Vector2>& path) {
                if (path.empty()) {
                    arrive(characterId, character->Position(), false);
                    return;
                }

                itor->second._Path.clear();
                for (int i = 0; i < path.size(); i++) {
                    itor->second._Path.push_back(path[i]);
                }
                character->SetAngle((path[0] - character->Position()).Angle());
                character->SetUInt64(Character::LastUpdatePositionTickUInt64, SafeSystem::Time::GetMilliSecond());
                float distance = (path[0] - character->Position()).Length();
                float speed = character->GetFloat(Character::SpeedFloat);
                int delay = (distance / speed) * 1000;
                START_TIMER(g_Engine, s_Self, eMoveTimer::UpdatePosition, 20, Api::Unlimited, 20, character->Guid());
                START_TIMER(g_Engine, s_Self, eMoveTimer::Arrived, delay, 1, delay, character->Guid());
                itor->second._Callback = arrive;

                IRobotSession* session = g_GameRobot->QuerySession(character->GetUInt64(Character::AccountUInt64));
                XASSERT(session, "wtf");
                if (session) {
                    oStateChange change;
                    change.set_state((int)AutoConfig::Enum::eState::Moving);
                    change.set_angle(character->Angle());
                    change.mutable_pos()->set_x(character->Position().x);
                    change.mutable_pos()->set_y(character->Position().y);

                    session->SendProtobuf(eClientID::StateChange, change);
                }
            }
        );
    }
}

void RobotMovement::OnRobotCreated(IRobotSession* const session) {
    //s_MoveInfoMap.insert({ session->DeployCharacterID(), MoveInfo() });
}

void RobotMovement::OnRobotRelease(IRobotSession* const session) {
    STOP_TIMER(g_Engine, s_Self, eMoveTimer::Arrived, session->DeployCharacterID());
    STOP_TIMER(g_Engine, s_Self, eMoveTimer::UpdatePosition, session->DeployCharacterID());
    s_MoveInfoMap.erase(session->DeployCharacterID());
}
