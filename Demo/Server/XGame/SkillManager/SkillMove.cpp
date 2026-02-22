#include "SkillMove.h"

static SkillMove* s_Self = nullptr;

enum eMoveTimer
{
    MoveUpdatePosition = 0,
    MoveUpdateAngle = 1,
    MoveCollision = 2,
    MoveRelease = 3
};

static std::unordered_map<UInt64, MoveData*> s_EntityMoveDataMap;
static std::vector<UInt64> s_BroadcastTemp;

__forceinline bool CalculateMoveEndPoint(IGameObject* cast, const AutoConfig::Struct::Move* moveConfig, Vector2& endPoint, MoveData* moveData)
{
    TraceLog(g_Engine, "StartPosition::X %f", cast->Position().x);
    TraceLog(g_Engine, "StartPosition::Y %f", cast->Position().y);
    TraceLog(g_Engine, "StartAngle::Angle %f", cast->Angle());
    double PI = std::acos(-1.0);
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(cast->Guid());
    if (scene == nullptr) return false;

    float initialSpeed = cast->GetFloat(Entity::MoveSpeedFloat);  // 初始速度
    float acceleration = moveConfig->AddSpeed;                // 加速度
    float totalTimeMs = moveConfig->Time;                    // 总时间(毫秒)
    float totalTime = totalTimeMs / 1000.0f;                 // 转换为秒

    float totalDistance = initialSpeed * totalTime + 0.5f * acceleration * totalTime * totalTime;

    endPoint.x = cast->Position().x + std::cos(cast->Angle() * PI / 180.0f) * totalDistance;
    endPoint.y = cast->Position().y + std::sin(cast->Angle() * PI / 180.0f) * totalDistance;

    TraceLog(g_Engine, "TheoreticalEndPoint: (%.2f, %.2f)", endPoint.x, endPoint.y);

    TraceLog(g_Engine, "HitPoint: (%f, %f), HitDistance: %f",
        initialSpeed, totalTime, acceleration);

    Vector2 hitPoint;
    bool hasHit = scene->GroundTileLayer()->Raycast(cast->Position(), cast->Angle(), totalDistance, hitPoint);

    if (hasHit) {
        endPoint = hitPoint;

        float dx = hitPoint.x - cast->Position().x;
        float dy = hitPoint.y - cast->Position().y;
        float hitDistance = std::sqrt(dx * dx + dy * dy);

        float a = acceleration;
        float v0 = initialSpeed;
        float s = hitDistance;

        float actualTime;

        if (std::abs(a) < 0.001f) {
            if (v0 > 0.001f) {
                actualTime = s / v0;
            }
            else {
                actualTime = 0.0f;
            }
        }
        else {
            float discriminant = v0 * v0 + 2.0f * a * s;

            if (discriminant < 0) {
                actualTime = totalTime * (hitDistance / totalDistance);
                TraceLog(g_Engine, "Warning: No real solution for time, using proportional time: %.3f", actualTime);
            }
            else {
                float sqrtD = std::sqrt(discriminant);
                float t1 = (-v0 + sqrtD) / a;
                float t2 = (-v0 - sqrtD) / a;

                if (t1 > 0 && t1 <= totalTime) {
                    actualTime = t1;
                }
                else if (t2 > 0 && t2 <= totalTime) {
                    actualTime = t2;
                }
                else {
                    actualTime = totalTime * (hitDistance / totalDistance);
                }
            }
        }
        actualTime = std::min(actualTime, totalTime);

        moveData->_ActualMoveTime = static_cast<UInt64>(actualTime * 1000);
        moveData->_EndPoint = endPoint;
    }
    else {

        moveData->_ActualMoveTime = totalTimeMs;
        moveData->_EndPoint = endPoint;
    }

    return hasHit;
}

bool SkillMove::Initialize(Api::iEngine* const engine)
{
    s_Self = this;

    s_BroadcastTemp.reserve(2048);
    return true;
}

bool SkillMove::Launch(Api::iEngine* const engine)
{
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_BuffManager, BuffManager);
    GetModule(g_Engine, g_Fsm, Fsm);
    GetModule(g_Engine, g_SkillDamage, SkillDamage);

    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Entity, IGameWorld::eGameWorldEvent::Create, OnGameEntityCreate);
    g_GameWorld->RegistGameObjectEvent(eGameObjectType::Entity, IGameWorld::eGameWorldEvent::Release, OnGameEntityRelease);

    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, OnCantControllerRefChanged);
    g_Fsm->RegistFsmEvent(OnFsmEvent);
    return true;
}

void SkillMove::OnCantControllerRefChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue) {
    if (index == Entity::CantControllerRefInt32) {
        if (oldValue == 0) {
            if (newValue > 0) {
                if (go) {
                    s_Self->OnMoveRelease(go);
                }
            }
        }
        else if (newValue == 0) {

        }
    }
}

void SkillMove::OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState initState)
{
    switch (ev) {
    case IFsm::eFsmEvent::FsmCreated: {
        g_Fsm->RegistExitJudge(guid, AutoConfig::Enum::eState::Shift, SkillMove::OnExitShiftJudge);
        break;
    }
    case IFsm::eFsmEvent::FsmReleased: {

        break;
    }
    }
}

bool SkillMove::OnExitShiftJudge(const UInt64 guid, const AutoConfig::Enum::eState, const Vector2&, const float)
{
    IGameObject* const entity = g_GameWorld->QueryGameObject(guid);
    if (!entity->Is(eGameObjectType::Entity)) {
        ErrorLog(g_Engine, "GameObject Is Not Entity");
        return false;
    }

    if (entity->GetInt32(Entity::CantControllerRefInt32) > 0)
    {
        return true;
    }

    if (entity->GetInt32(Entity::ShiftRefInt32) > 0)
    {
        return false;
    }
    return true;
}

UInt64 SkillMove::AddMove(IGameObject* const cast, const UInt64 skillGuid, const Int64 skillConfig, const Int64 moveID, const float angle, const UInt64 targetGuid)
{
    const AutoConfig::Struct::Move* moveConfig = GetConfig(g_Config, Move, moveID);
    if (nullptr == moveConfig)
    {
        ErrorLog(g_Engine, "Where is move config %lld", moveID);
        return INVALID_GAMEOBJECT_ID;
    }
    auto itor = s_EntityMoveDataMap.find(cast->Guid());
    if (itor != s_EntityMoveDataMap.end())
    {
        TraceLog(g_Engine, "Entity is moving %llu", cast->Guid());
        return INVALID_GAMEOBJECT_ID;
    }
    if (cast->Guid() == targetGuid)
    {
        TraceLog(g_Engine, "Move target is cast:: %llu", targetGuid);
        return INVALID_GAMEOBJECT_ID;
    }
    XASSERT(skillConfig != 0, "wtf");
    MoveData* moveData = new MoveData();
    moveData->_Cast = cast;
    moveData->_SkillGuid = skillGuid;
    moveData->_SkillConfig = skillConfig;
    moveData->_TargetGuid = targetGuid;
    moveData->_MoveID = moveID;
    moveData->_DamageCount = 0;
    moveData->_CastSpeed = cast->GetFloat(Entity::SpeedFloat);
    moveData->_LastUpdateTimeTick = SafeSystem::Time::GetMilliSecond() + (UInt64)moveConfig->DelayTime;

    moveData->_ActualMoveTime = moveConfig->Time;

    cast->SetAngle(angle);

    g_Fsm->EnterState(cast->Guid(), AutoConfig::Enum::eState::Shift, cast->Position(), cast->Angle());
    cast->SetInt32(Entity::ShiftRefInt32, cast->GetInt32(Entity::ShiftRefInt32) + 1);

    cast->SetFloat(Entity::MoveSpeedFloat, moveConfig->Speed);
    if (moveConfig->MoveType == AutoConfig::Enum::eMoveType::LineMove)
    {
        Vector2 endPoint;
        bool hasHit = CalculateMoveEndPoint(cast, moveConfig, endPoint, moveData);
        moveData->_EndPoint = endPoint;

        if (hasHit) {
            TraceLog(g_Engine, "LineMove will hit at position (%.2f, %.2f), actual time: %llums",
                endPoint.x, endPoint.y, moveData->_ActualMoveTime);
        }
        else {
            TraceLog(g_Engine, "LineMove no collision, end position (%.2f, %.2f), time: %llums",
                endPoint.x, endPoint.y, moveData->_ActualMoveTime);
        }
    }
    else if (moveConfig->MoveType == AutoConfig::Enum::eMoveType::TrackMove)
    {
        IGameObject* target = g_GameWorld->QueryGameObject(targetGuid);
        if (target == nullptr)
        {
            TraceLog(g_Engine, "Target is not exist :: %llu", targetGuid);
            return INVALID_GAMEOBJECT_ID;
        }
        float angle = GeometryUtils::CalculateAngleBetweenPoints(cast->Position(), target->Position());
        cast->SetAngle(angle);
    }

    s_EntityMoveDataMap.insert({ cast->Guid(), moveData });

    START_TIMER(g_Engine, this, eMoveTimer::MoveCollision, moveConfig->DelayTime + moveConfig->DelayEffectTime, Api::Unlimited, moveConfig->DamageInteval, (UInt64)moveData);

    START_TIMER(g_Engine, this, eMoveTimer::MoveUpdatePosition, moveConfig->DelayTime + AutoConfig::Const::PositionSolide::Solide, Api::Unlimited, AutoConfig::Const::PositionSolide::Solide, (UInt64)moveData);

    START_TIMER(g_Engine, this, eMoveTimer::MoveUpdateAngle, moveConfig->DelayTime + moveConfig->UpdateInterval, Api::Unlimited, moveConfig->UpdateInterval, (UInt64)moveData);

    START_TIMER(g_Engine, this, eMoveTimer::MoveRelease, moveConfig->DelayTime + moveData->_ActualMoveTime + 1, 1, 0, (UInt64)moveData);

    return moveID;
}

void SkillMove::OnTimer(const int id, const UInt64 context, const Int64 timestamp)
{
    MoveData* moveData = (MoveData*)context;
    IGameObject* cast = moveData->_Cast;
    if (cast == nullptr)
    {
        return;
    }

    const AutoConfig::Struct::Move* moveConfig = GetConfig(g_Config, Move, moveData->_MoveID);
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(cast->Guid());

    switch (id)
    {
    case eMoveTimer::MoveUpdatePosition:
    {
        switch (moveConfig->MoveType)
        {
        case AutoConfig::Enum::eMoveType::LineMove:
        {
            UInt64 deltaTime = SafeSystem::Time::GetMilliSecond() - moveData->_LastUpdateTimeTick;
            moveData->_LastUpdateTimeTick = SafeSystem::Time::GetMilliSecond();
            float flyDistance = cast->GetFloat(Entity::MoveSpeedFloat) * ((float)deltaTime / 1000) + 0.5 * moveConfig->AddSpeed * ((float)deltaTime / 1000) * ((float)deltaTime / 1000);
            double PI = std::acos(-1.0);
            double radians = cast->Angle() * PI / 180.0f;

            cast->SetPosition(cast->Position().x + std::cos(radians) * flyDistance, cast->Position().y + std::sin(radians) * flyDistance);
            cast->SetFloat(Entity::MoveSpeedFloat, cast->GetFloat(Entity::MoveSpeedFloat) + moveConfig->AddSpeed * ((float)deltaTime / 1000));

            break;
        }
        case AutoConfig::Enum::eMoveType::TrackMove:
        {
            UInt64 deltaTime = SafeSystem::Time::GetMilliSecond() - moveData->_LastUpdateTimeTick;

            moveData->_LastUpdateTimeTick = SafeSystem::Time::GetMilliSecond();
            float flyDistance = cast->GetFloat(Entity::MoveSpeedFloat) * ((float)deltaTime / 1000);
            double PI = std::acos(-1.0);
            double radians = cast->Angle() * PI / 180.0f;

            Vector2 hit;
            if (scene->GroundTileLayer()->HasGrid(cast->Position().x + std::cos(radians) * flyDistance, cast->Position().y + std::sin(radians) * flyDistance))
            {
                cast->SetPosition(cast->Position().x + std::cos(radians) * flyDistance, cast->Position().y + std::sin(radians) * flyDistance);
            }

            break;
        }
        }
        break;
    }
    case eMoveTimer::MoveUpdateAngle:
    {
        switch (moveConfig->MoveType)
        {
        case AutoConfig::Enum::eMoveType::TrackMove:
        {
            IGameObject* target = g_GameWorld->QueryGameObject(moveData->_TargetGuid);
            if (target != nullptr)
            {
                float angle = GeometryUtils::CalculateAngleBetweenPoints(cast->Position(), target->Position());
                float currentAngle = cast->Angle();
                float targetAngle = angle;
                float angleDiff = targetAngle - currentAngle;
                float minAngleDiff = std::fmod(angleDiff + 180.0f, 360.0f) - 180.0f;
                float newAngle;
                if (std::abs(minAngleDiff) <= moveConfig->MaxTurnAngle) {
                    newAngle = currentAngle + minAngleDiff;
                }
                else {
                    if (minAngleDiff > 0) {
                        newAngle = currentAngle + moveConfig->MaxTurnAngle;
                    }
                    else {
                        newAngle = currentAngle - moveConfig->MaxTurnAngle;
                    }
                }
                cast->SetAngle(GeometryUtils::NormalizeAngle(newAngle));
            }

            break;
        }
        }

        break;
    }
    case eMoveTimer::MoveCollision:
    {
        scene->GetVisual(Vector2(cast->Position().x + moveConfig->ReleasePosition[0], cast->Position().y + moveConfig->ReleasePosition[1]), moveConfig->DamageRange,
            [&](const std::vector<UInt64>& visual) {
                bool isMoveRelease = false;
                for (auto visualIt = visual.begin(); visualIt != visual.end(); visualIt++) {
                    IGameObject* go = g_GameWorld->QueryGameObject(*visualIt);
                    if (go == nullptr)
                    {
                        continue;
                    }
                    if (go->Guid() == cast->Guid())
                    {
                        continue;
                    }
                    if (g_Fsm->GetState(go->Guid()) == AutoConfig::Enum::eState::Death)
                    {
                        continue;
                    }

                    float bodyRadius = 0;
                    if (go->Is(eGameObjectType::Character))
                    {
                        const AutoConfig::Struct::Character* characterConfig = GetConfig(g_Config, Character, go->ConfigID());
                        bodyRadius = characterConfig->CharacterBody;
                    }
                    else if (go->Is(eGameObjectType::Monster))
                    {
                        const AutoConfig::Struct::Monster* monsterConfig = GetConfig(g_Config, Monster, go->ConfigID());
                        bodyRadius = monsterConfig->MonsterBody;
                    }

                    if (!isMoveRelease)
                    {
                        if (moveConfig->DamageCount == 0 || moveData->_DamageCount < moveConfig->DamageCount)
                        {
                            if (GeometryUtils::IsCircleIntersectCircle(Vector2(cast->Position().x + moveConfig->ReleasePosition[0], cast->Position().y + moveConfig->ReleasePosition[1]), moveConfig->DamageRange, go->Position(), bodyRadius))
                            {
                                switch (moveConfig->TargetType)
                                {
                                case AutoConfig::Enum::eTargetType::All:
                                {
                                    break;
                                }
                                case AutoConfig::Enum::eTargetType::EnemyCharacter:
                                {
                                    if (go->Type() == eGameObjectType::Character)
                                    {
                                        OnMoveDamageCheck(cast, go, isMoveRelease, moveData);
                                    }
                                    break;
                                }
                                case AutoConfig::Enum::eTargetType::Monster:
                                {
                                    if (go->Type() == eGameObjectType::Monster)
                                    {
                                        OnMoveDamageCheck(cast, go, isMoveRelease, moveData);
                                    }
                                    break;
                                }
                                case AutoConfig::Enum::eTargetType::PlayerCharacter:
                                {
                                    break;
                                }
                                case AutoConfig::Enum::eTargetType::PlayerSelf:
                                {
                                    break;
                                }
                                }
                            }
                        }
                    }
                }
            }
        );
        break;
    }
    case eMoveTimer::MoveRelease:
    {
        if (moveConfig->MoveType == AutoConfig::Enum::eMoveType::LineMove) {
            cast->SetPosition(moveData->_EndPoint.x, moveData->_EndPoint.y);
        }
        OnMoveRelease(cast);


        break;
    }
    }
}

void SkillMove::OnMoveRelease(IGameObject* cast)
{
    auto itor = s_EntityMoveDataMap.find(cast->Guid());
    if (itor != s_EntityMoveDataMap.end())
    {
        IScene* scene = g_SceneManager->QuerySceneByEntityGuid(cast->Guid());

        oSkillEndPush push;
        push.set_castguid(cast->Guid());
        push.set_skillguid(itor->second->_SkillGuid);
        push.set_skillconfigid(itor->second->_SkillConfig);

        s_BroadcastTemp.clear();
        if (cast->Is(eGameObjectType::Character))
        {
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

        g_PlayerManager->Broadcast(s_BroadcastTemp, eServerID::SkillEndPush, push);

        STOP_TIMER(g_Engine, s_Self, eMoveTimer::MoveCollision, (UInt64)itor->second);
        STOP_TIMER(g_Engine, s_Self, eMoveTimer::MoveRelease, (UInt64)itor->second);
        STOP_TIMER(g_Engine, s_Self, eMoveTimer::MoveUpdateAngle, (UInt64)itor->second);
        STOP_TIMER(g_Engine, s_Self, eMoveTimer::MoveUpdatePosition, (UInt64)itor->second);

        cast->SetInt32(Entity::ShiftRefInt32, cast->GetInt32(Entity::ShiftRefInt32) - 1);
        cast->SetFloat(Entity::MoveSpeedFloat, itor->second->_CastSpeed);

        delete itor->second;
        itor->second = nullptr;

        s_EntityMoveDataMap.erase(itor);

        TraceLog(g_Engine, "RayCast Position::%f, %f bool::%d", cast->Position().x, cast->Position().y);
        g_Fsm->EnterState(cast->Guid(), AutoConfig::Enum::eState::Idle, cast->Position(), cast->Angle());
    }
}

void SkillMove::OnMoveDamageCheck(IGameObject* const cast, IGameObject* const target, bool& isMoveRelease, MoveData* const moveData)
{
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(cast->Guid());
    const AutoConfig::Struct::Move* moveConfig = GetConfig(g_Config, Move, moveData->_MoveID);
    if (moveConfig->DamageTarget && target->Guid() != moveData->_TargetGuid)
    {
        return;
    }
    moveData->_DamageCount++;
    OnMoveDamage(cast, target, moveData);
    //发送消息
    if (moveConfig->DamageCount > 0 && moveData->_DamageCount >= moveConfig->DamageCount)
    {
        isMoveRelease = true;
        OnMoveRelease(cast);
    }
}

void SkillMove::OnMoveDamage(IGameObject* const cast, IGameObject* const target, MoveData* const moveData)
{
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(cast->Guid());
    const AutoConfig::Struct::Move* moveConfig = GetConfig(g_Config, Move, moveData->_MoveID);

    g_SkillDamage->AddDamage(cast, target, moveConfig->SkillDamage, moveConfig->DamgaeType);

    //加buff
    for (int i = 0; i < moveConfig->Buff.size(); i++)
    {
        if (moveConfig->Buff[i] != -1)
        {
            g_BuffManager->AddBuff(cast, target, moveConfig->Buff[i]);
        }
    }
}

bool SkillMove::Destroy(Api::iEngine* const engine)
{
    return true;
}

void SkillMove::OnGameEntityCreate(IGameObject* const go)
{

}

void SkillMove::OnGameEntityRelease(IGameObject* const go)
{
    for (auto it = s_EntityMoveDataMap.begin(); it != s_EntityMoveDataMap.end();)
    {
        if (it->second->_Cast->Guid() == go->Guid())
        {
            s_Self->OnMoveRelease(go);
        }
        else
        {
            ++it;
        }
    }
}