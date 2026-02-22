#include "Movement.h"

Api::iEngine* g_Engine = nullptr;
IFsm* g_Fsm = nullptr;
IGameWorld* g_GameWorld = nullptr;
ISceneManager* g_SceneManager = nullptr;
IPlayerManager* g_PlayerManager = nullptr;

static Movement* s_Self = nullptr;

enum eMovementTimer {
    Moving = 0,
    Arrived = 1
};
#define CalcPositionInterval 100

struct MoveToInfo {
    std::list<Vector2> _Path;
    IMovement::fArrived _Arrived;
};

static std::unordered_map<UInt64, MoveToInfo> s_MoveInfos;

struct FaildInfo {
    const UInt64 _Guid;
    const IMovement::fArrived _Arrived;

    FaildInfo(const UInt64 guid, const IMovement::fArrived& fun) : _Guid(guid), _Arrived(fun) {}
};

bool Movement::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool Movement::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Fsm, Fsm);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);

    //this->_TimerLog = true;

    g_Fsm->RegistFsmEvent(OnFsmEvent);
    return true;
}

bool Movement::Destroy(Api::iEngine* const engine) {
    return true;
}

void Movement::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eMovementTimer::Moving: {
        UpdatePosition(context, timestamp);
        break;
    }
    }
}

void Movement::OnEnd(const int id, const UInt64 context, bool nonviolent, const Int64 timestamp) {
    switch (id) {
    case eMovementTimer::Moving: {
        if (!nonviolent) {
            CalcPosition(context, timestamp);
        }
        break;
    }
    case eMovementTimer::Arrived: {
        TraceLog(g_Engine, "Movement::OnEnd Arrived nonviolent %s", nonviolent ? "true" : "false");
        if (!nonviolent) {
            IGameObject* go = g_GameWorld->QueryGameObject(context);
            auto it = s_MoveInfos.find(context);
            XASSERT(s_MoveInfos.end() != it, "wtf");
            if (s_MoveInfos.end() != it) {
                if (nullptr != it->second._Arrived) {
                    it->second._Arrived(go, false);
                }

                s_MoveInfos.erase(it);
            }
        }
        else {
            IGameObject* go = g_GameWorld->QueryGameObject(context);
            auto it = s_MoveInfos.find(context);
            if (s_MoveInfos.end() != it) {
                go->SetPosition(*it->second._Path.begin());
                it->second._Path.erase(it->second._Path.begin());
                if (it->second._Path.empty()) {
                    if (nullptr == it->second._Arrived) {
                        s_MoveInfos.erase(it);
                        //TraceLog(g_Engine, "s_MoveInfos erase %llu", context);
                    }
                    else {
                        IMovement::fArrived fun = it->second._Arrived;
                        s_MoveInfos.erase(it);
                        //TraceLog(g_Engine, "s_MoveInfos erase %llu", context);
                        fun(go, true);
                    }
                    g_Fsm->EnterState(go->Guid(), AutoConfig::Enum::eState::Idle, go->Position(), go->Angle());
                }
                else {
                    const float angle = ((*it->second._Path.begin()) - go->Position()).Angle();
                    if (!g_Fsm->EnterState(go->Guid(), AutoConfig::Enum::eState::Moving, go->Position(), angle)) {
                        if (nullptr != it->second._Arrived)
                            it->second._Arrived(go, false);
                        return;
                    }
                    float distance = ((*it->second._Path.begin()) - go->Position()).Length();
                    float speed = go->GetFloat(Entity::SpeedFloat);
                    int delay = (distance / speed) * 1000;
                    START_TIMER(g_Engine, s_Self, eMovementTimer::Arrived, delay, 1, delay, context);
                }
            }
        }
        break;
    }
    }
}

void Movement::MoveTo(IGameObject* const entity, const Vector2& position, const fArrived& callback) {
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(entity->Guid());
    if (nullptr == scene) {
        ErrorLog(g_Engine, "GameObject %llu not in any scene", entity->Guid());
        callback(entity, false);
        return;
    }

    STOP_TIMER(g_Engine, s_Self, eMovementTimer::Arrived, entity->Guid());

    scene->GroundTileLayer()->FindPath(entity->Position(), position,
        [&](const bool succeed, const std::vector<Vector2>& path) {
            if (succeed) {
                auto it = s_MoveInfos.insert({ entity->Guid(), MoveToInfo() }).first;
                for (int i = 0; i < path.size(); i++) {
                    it->second._Path.push_back(path[i]);
                }

                if (it->second._Path.empty()) {
                    if (callback)
                        callback(entity, false);
                    return;
                }

                const float angle = (path[0] - entity->Position()).Angle();
                if (!g_Fsm->EnterState(entity->Guid(), AutoConfig::Enum::eState::Moving, entity->Position(), angle)) {
                    if (callback)
                        callback(entity, false);
                    return;
                }
                float distance = (path[0] - entity->Position()).Length();
                float speed = entity->GetFloat(Entity::SpeedFloat);
                int delay = (distance / speed) * 1000;
                it->second._Arrived = callback;
                START_TIMER(g_Engine, s_Self, eMovementTimer::Arrived, delay, 1, delay, entity->Guid());
            }
            else {
                if (callback)
                    callback(entity, false);
            }
        }
    );
}

void Movement::StopMoveTo(IGameObject* const entity) {
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(entity->Guid());
    if (nullptr == scene) {
        ErrorLog(g_Engine, "GameObject %llu not in any scene", entity->Guid());
        return;
    }

    STOP_TIMER(g_Engine, s_Self, eMovementTimer::Arrived, entity->Guid());
    g_Fsm->EnterState(entity->Guid(), AutoConfig::Enum::eState::Idle, entity->Position(), entity->Angle());
}

void Movement::OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState initState) {
    switch (ev) {
    case IFsm::eFsmEvent::FsmCreated: {
        g_Fsm->RegistStateEnter(guid, AutoConfig::Enum::eState::Moving, Movement::OnEnterMoving);
        g_Fsm->RegistStateExit(guid, AutoConfig::Enum::eState::Moving, Movement::OnExitMoving);
        g_Fsm->RegistEnterJudge(guid, AutoConfig::Enum::eState::Moving, Movement::OnEnterMovingJugde);
        break;
    }
    case IFsm::eFsmEvent::FsmReleased: {
        STOP_TIMER(g_Engine, s_Self, eMovementTimer::Moving, guid);
        STOP_TIMER(g_Engine, s_Self, eMovementTimer::Arrived, guid);
        break;
    }
    }
}

void Movement::GetBestSlideEdges(const Vector2& delta, int& first, int& second)
{
    Vector2 nd = delta.Normalized();

    float best = -1.f;
    float secondBest = -1.f;
    first = -1;
    second = -1;

    for (int i = 0; i < AutoConfig::Const::RhombusMap::RhombusAngles.size(); i++)
    {
        float rad = AutoConfig::Const::RhombusMap::RhombusAngles[i] * XDeg2Rad; //AutoConfig::Const.RhombusMap.RhombusAngles[i] * XDeg2Rad;
        Vector2 edgeDir(std::cos(rad), std::sin(rad));
        float dot = nd.Dot(edgeDir);
        if (dot > best)
        {
            secondBest = best;
            second = first;
            best = dot;
            first = i;
        }
        else if (dot > secondBest)
        {
            secondBest = dot;
            second = i;
        }
    }
}

bool Movement::CanSlide(IGameObject* const unit, const ITileLayer* const tilemap, const Vector2& delta, Vector2& pos) {
    int edgeA, edgeB;
    GetBestSlideEdges(delta, edgeA, edgeB);

    // 第一个贴边方向
    {
        float rad = AutoConfig::Const::RhombusMap::RhombusAngles[edgeA] * XDeg2Rad;
        Vector2 slideDir(std::cos(rad), std::sin(rad));
        Vector2 slidePos = unit->Position() + slideDir * unit->GetFloat(Entity::SpeedFloat) * 0.1;

        if (tilemap->HasGrid(slidePos))
        {
            pos = slidePos;
            return true;
        }
    }

    {
        float rad = AutoConfig::Const::RhombusMap::RhombusAngles[edgeB] * XDeg2Rad;
        Vector2 slideDir(std::cos(rad), std::sin(rad));
        Vector2 slidePos = unit->Position() + slideDir * unit->GetFloat(Entity::SpeedFloat) * 0.1;

        if (tilemap->HasGrid(slidePos))
        {
            pos = slidePos;
            return true;
        }
    }

    return false;
}

bool Movement::CalcPosition(const UInt64 guid, const UInt64 timestamp) {
    IGameObject* go = g_GameWorld->QueryGameObject(guid);
    if (nullptr == go) {
        ErrorLog(g_Engine, "Can not find game object %llu", guid);
        STOP_TIMER(g_Engine, s_Self, eMovementTimer::Moving, guid);
        return false;
    }

    if (!go->Is(eGameObjectType::Entity)) {
        ErrorLog(g_Engine, "GameObject not a Engity %llu", guid);
        STOP_TIMER(g_Engine, s_Self, eMovementTimer::Moving, guid);
        return false;
    }

    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(guid);
    if (nullptr == scene) {
        //ErrorLog(g_Engine, "Can not find scene info on game object %llu", guid);
        STOP_TIMER(g_Engine, s_Self, eMovementTimer::Moving, guid);
        return false;
    }


    Vector2 pos = Vector2::CalcPosition(go->Position(), go->Angle(), go->GetFloat(Entity::SpeedFloat) * ((timestamp - go->GetUInt64(Entity::LastUpdatePositionTickUInt64)) / 1000.0f));
    if (scene->GroundTileLayer()->HasGrid(pos.x, pos.y)) {
        //DEBUG(g_Engine, "Entity %s update position %f, %f angle %f", go->GetString(Entity::NicknameString).c_str(), pos.x, pos.y, go->Angle());
        go->SetPosition(pos);
        go->SetUInt64(Entity::LastUpdatePositionTickUInt64, timestamp);
    }
    else {
        float angleInRadians = go->Angle() * XDeg2Rad;
        Vector2 direction(std::cos(angleInRadians), std::sin(angleInRadians));
        Vector2 delta = direction * go->GetFloat(Entity::SpeedFloat) * ((timestamp - go->GetUInt64(Entity::LastUpdatePositionTickUInt64)) / 1000.0f);
        Vector2 slidPos;
        if (CanSlide(go, scene->GroundTileLayer(), delta, slidPos)) {
            go->SetPosition(slidPos);
            go->SetUInt64(Entity::LastUpdatePositionTickUInt64, timestamp);
            return true;
        }

        TraceLog(g_Engine, "Entity %s can not move to %f, %f", go->GetString(Entity::NicknameString).c_str(), pos.x, pos.y);
        return false;
    }

    return true;
}

void Movement::UpdatePosition(const UInt64 guid, const UInt64 timestamp) {
    if (!CalcPosition(guid, timestamp)) {
        IGameObject* go = g_GameWorld->QueryGameObject(guid);
        if (go) {
            g_Fsm->ForceEnterState(guid, AutoConfig::Enum::eState::Idle, go->Position(), go->Angle());
        }
    }
}

bool Movement::OnEnterMovingJugde(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& position, const float angle) {
    IGameObject* go = g_GameWorld->QueryGameObject(guid);
    if (nullptr == go) {
        ErrorLog(g_Engine, "Can not find game object %llu", guid);
        return false;
    }

    if (!go->Is(eGameObjectType::Entity)) {
        ErrorLog(g_Engine, "GameObject not a Engity %llu", guid);
        return false;
    }

    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(guid);
    if (nullptr == scene) {
        ErrorLog(g_Engine, "Can not find scene info on game object %llu", guid);
        return false;
    }

    Vector2 pos = Vector2::CalcPosition(position, angle, 0.05);
    if (!scene->GroundTileLayer()->HasGrid(pos.x, pos.y)) {
        float angleInRadians = go->Angle() * XDeg2Rad;
        Vector2 direction(std::cos(angleInRadians), std::sin(angleInRadians));
        Vector2 delta = direction * go->GetFloat(Entity::SpeedFloat) * ((SafeSystem::Time::GetMilliSecond() - go->GetUInt64(Entity::LastUpdatePositionTickUInt64)) / 1000.0f);
        Vector2 slidPos;
        if (!CanSlide(go, scene->GroundTileLayer(), delta, slidPos)) {
            return false;
        }
    }

    return true;
}

void Movement::OnEnterMoving(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& pos, const float angle) {
    IGameObject* go = g_GameWorld->QueryGameObject(guid);
    XASSERT(go, "wtf");
    if (go) {
        go->SetPosition(pos);
        go->SetAngle(angle);
        STOP_TIMER(g_Engine, s_Self, eMovementTimer::Moving, guid);
        go->SetUInt64(Entity::LastUpdatePositionTickUInt64, SafeSystem::Time::GetMilliSecond());
        START_TIMER(g_Engine, s_Self, eMovementTimer::Moving, CalcPositionInterval, Api::Unlimited, CalcPositionInterval, guid);
    }
}

void Movement::OnExitMoving(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& pos, const float angle) {
    STOP_TIMER(g_Engine, s_Self, eMovementTimer::Moving, guid);
    STOP_TIMER(g_Engine, s_Self, eMovementTimer::Arrived, guid);
    TraceLog(g_Engine, "Entity %llu OnExitMoving", guid);
}
