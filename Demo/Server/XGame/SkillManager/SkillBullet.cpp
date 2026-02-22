#include "SkillBullet.h"

static SkillBullet* s_Self = nullptr;
IFsm* g_Fsm = nullptr;
IBuffManager* g_BuffManager = nullptr;

enum eBulletTimer
{
    BulletUpdatePosition = 0,
    BulletUpdateAngle = 1,
    BulletCollision = 2,
    BulletRelease = 3,
    BulletAddHurtLife = 4,
};

static std::vector<UInt64> s_BroadcastTemp;

static std::unordered_map<UInt64, std::vector<CollisionInfo>> s_BulletCollisionInfo;

static std::unordered_map<UInt64, std::unordered_set<UInt64>> s_SceneBulletMap;

bool SkillBullet::Initialize(Api::iEngine* engine) {
    s_Self = this;

    s_BroadcastTemp.reserve(2048);
    return true;
}

bool SkillBullet::Launch(Api::iEngine* engine) {

    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_Fsm, Fsm);
    GetModule(g_Engine, g_BuffManager, BuffManager);
    GetModule(g_Engine, g_SkillDamage, SkillDamage);

    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Create, SkillBullet::OnSceneCreate);
    g_SceneManager->RegistSceneEvent(ISceneManager::eSceneEvent::Release, SkillBullet::OnSceneRelease);

    return true;
}

UInt64 SkillBullet::AddBullet(IGameObject* const launcher, const Int64 bulletID)
{
    const AutoConfig::Struct::Bullet* bulletConfig = GetConfig(g_Config, Bullet, bulletID);
    if (nullptr == bulletConfig)
    {
        ErrorLog(g_Engine, "Where is bullet config %lld", bulletID);
        return INVALID_GAMEOBJECT_ID;
    }
    IGameObject* bullet = g_GameWorld->CreateGameObject(eGameObjectType::Bullet, bulletID, AUTO_GUID,
        [&](IGameObject* const go) {
            IGameObject* host = g_GameWorld->QueryGameObject(launcher->GetUInt64(Launcher::HostGuidUInt64));

            go->SetFloat(Entity::SpeedFloat, bulletConfig->Speed);
            go->SetFloat(Entity::CriticalFloat, host->GetFloat(Entity::CriticalFloat));
            go->SetFloat(Entity::CriticalDamageFloat, host->GetFloat(Entity::CriticalDamageFloat));
            go->SetFloat(Entity::PhysicalPierceFloat, host->GetFloat(Entity::PhysicalPierceFloat));
            go->SetFloat(Entity::PhysicalPierceRateFloat, host->GetFloat(Entity::PhysicalPierceRateFloat));
            go->SetFloat(Entity::MagicPierceFloat, host->GetFloat(Entity::MagicPierceFloat));
            go->SetFloat(Entity::MagicPierceRateFloat, host->GetFloat(Entity::MagicPierceRateFloat));
            go->SetFloat(Entity::AtkFloat, host->GetFloat(Entity::AtkFloat));

            go->SetUInt64(Bullet::HostGuidUInt64, launcher->GetUInt64(Launcher::HostGuidUInt64));
            go->SetUInt64(Bullet::TargetGuidUInt64, launcher->GetUInt64(Launcher::TargetGuidUInt64));
            go->SetFloat(Bullet::TargetPosXFloat, launcher->GetFloat(Launcher::TargetPosXFloat));
            go->SetFloat(Bullet::TargetPosYFloat, launcher->GetFloat(Launcher::TargetPosYFloat));
            go->SetFloat(Bullet::DeltaTimeFloat, 0);
            go->SetVector2(Bullet::StartPosVector2, launcher->Position());
            go->SetInt64(Bullet::DamageCountInt64, 0);
            go->SetUInt64(Bullet::LastUpdateTimeUInt64, SafeSystem::Time::GetMilliSecond());
        }
    );

    bullet->SetPosition(launcher->Position());
    bullet->SetAngle(launcher->Angle() + bulletConfig->Angle);
    TraceLog(g_Engine, "Create Bullet %llu", bullet->Guid());
    if (bulletConfig->Ballistics == AutoConfig::Enum::eBallistics::Parabolic)
    {
        Vector2 centerPos = Vector2(launcher->GetFloat(Launcher::TargetPosXFloat) / 2 + launcher->Position().x / 2, launcher->GetFloat(Launcher::TargetPosYFloat) / 2 + launcher->Position().y / 2);
        Vector2 dir = Vector2(launcher->Position().y - launcher->GetFloat(Launcher::TargetPosYFloat), launcher->GetFloat(Launcher::TargetPosXFloat) - launcher->Position().x).Normalized();
        float height = Vector2(launcher->GetFloat(Launcher::TargetPosXFloat) - launcher->Position().x, launcher->GetFloat(Launcher::TargetPosYFloat) - launcher->Position().y).Length() * bulletConfig->HeightParameter;
        Vector2 controlPos = centerPos + Vector2(dir.x * height, dir.y * height);
    }
    else if (bulletConfig->Ballistics == AutoConfig::Enum::eBallistics::Track)
    {
        IGameObject* target = g_GameWorld->QueryGameObject(bullet->GetUInt64(Launcher::TargetGuidUInt64));
        float angle = GeometryUtils::CalculateAngleBetweenPoints(bullet->Position(), target->Position());
        bullet->SetAngle(angle);
    }

    IGameObject* host = g_GameWorld->QueryGameObject(launcher->GetUInt64(Launcher::HostGuidUInt64));

    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(host->Guid());
    if (nullptr == scene) {
        ErrorLog(g_Engine, "can not find any scene for game object %llu", host->Guid());
        return INVALID_GAMEOBJECT_ID;
    }

    scene->EnterScene(bullet->Guid(),
        [&](IScene* const, const UInt64 guid, UInt64 other) {
            TraceLog(g_Engine, "bullet enter scene %llu", guid);
        }, false
    );

    if (s_SceneBulletMap.find(scene->SceneID()) == s_SceneBulletMap.end())
    {
        s_SceneBulletMap.insert({ scene->SceneID(), {} });
    }
    s_SceneBulletMap.find(scene->SceneID())->second.insert(bullet->Guid());

    if (bulletConfig->IsCollision)
    {
        START_TIMER(g_Engine, this, eBulletTimer::BulletCollision, bulletConfig->CollisionInterval, Api::Unlimited, bulletConfig->CollisionInterval, bullet->Guid());
    }

    START_TIMER(g_Engine, this, eBulletTimer::BulletUpdatePosition, AutoConfig::Const::PositionSolide::Solide, Api::Unlimited, AutoConfig::Const::PositionSolide::Solide, bullet->Guid());

    START_TIMER(g_Engine, this, eBulletTimer::BulletUpdateAngle, bulletConfig->UpdateInterval, Api::Unlimited, bulletConfig->UpdateInterval, bullet->Guid());
    if (bulletConfig->LifeCycle > 0)
    {
        START_TIMER(g_Engine, this, eBulletTimer::BulletRelease, bulletConfig->LifeCycle + 1, 1, 0, bullet->Guid());
    }

    s_BulletCollisionInfo.insert({ bullet->Guid(), {} });

    return bullet->Guid();
}

bool SkillBullet::BulletCollisionCheck(IGameObject* const bullet, IGameObject* const go)
{
    float bodyRadius = 0;
    if (go->Is(eGameObjectType::Character))
    {
        const AutoConfig::Struct::Character* characterConfig = GetConfig(g_Config, Character, go->ConfigID());
        bodyRadius = characterConfig->CharacterBody;
    }
    else if (go->Is(eGameObjectType::Boss))
    {
        const AutoConfig::Struct::MonsterBoss* monsterBossConfig = GetConfig(g_Config, MonsterBoss, go->ConfigID());
        bodyRadius = monsterBossConfig->MonsterBody;
    }
    else if (go->Is(eGameObjectType::Monster))
    {
        const AutoConfig::Struct::Monster* monsterConfig = GetConfig(g_Config, Monster, go->ConfigID());
        bodyRadius = monsterConfig->MonsterBody;
    }
    const AutoConfig::Struct::Bullet* bulletConfig = GetConfig(g_Config, Bullet, bullet->ConfigID());

    switch (bulletConfig->RangeType)
    {
    case AutoConfig::Enum::eRangeType::Circle:
    {
        return GeometryUtils::IsCircleIntersectCircle(go->Position(), bodyRadius, bullet->Position(), bulletConfig->Radius);
    }
    case AutoConfig::Enum::eRangeType::Sector:
    {
        float startAngle = GeometryUtils::NormalizeAngle(bullet->Angle() - bulletConfig->SectorAngle / 2.0f);
        float endAngle = GeometryUtils::NormalizeAngle(bullet->Angle() + bulletConfig->SectorAngle / 2.0f);
        return GeometryUtils::IsCircleIntersectSetor(go->Position(), bodyRadius, bullet->Position(), bulletConfig->Radius, startAngle, endAngle);
    }
    case AutoConfig::Enum::eRangeType::Ellipse:
    {
        return GeometryUtils::IsCircleIntersectEllipse(go->Position(), bodyRadius, bullet->Position(), bulletConfig->Radius * 2, bulletConfig->ShortRadius * 2, bullet->Angle());
    }
    case AutoConfig::Enum::eRangeType::Cube:
    {
        return GeometryUtils::IsCircleIntersectRectangle(go->Position(), bodyRadius, bullet->Position(), bulletConfig->RectangleSize[0], bulletConfig->RectangleSize[1], bullet->Angle());
    }
    default:
        return false;
    }
}

void SkillBullet::OnTimer(const int id, const UInt64 context, const Int64 timestamp)
{
    IGameObject* bullet = g_GameWorld->QueryGameObject(context);
    //TraceLog(g_Engine, "Bullet PositionXXXX::%f YYYY::%f", bullet->Position().x, bullet->Position().y);
    if (bullet == nullptr)
    {
        ErrorLog(g_Engine, "Bullet Is Not Exist: %llu", context);
        return;
    }
    const AutoConfig::Struct::Bullet* bulletConfig = GetConfig(g_Config, Bullet, bullet->ConfigID());
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(bullet->Guid());
    const AutoConfig::Struct::Scene* sceneConfig = GetConfig(g_Config, Scene, scene->Config()->ID);
    switch (id)
    {
    case eBulletTimer::BulletUpdatePosition:
    {
        float intervalTime = SafeSystem::Time::GetMilliSecond() - bullet->GetUInt64(Bullet::LastUpdateTimeUInt64);
        bullet->SetUInt64(Bullet::LastUpdateTimeUInt64, SafeSystem::Time::GetMilliSecond());
        IGameObject* host = g_GameWorld->QueryGameObject(bullet->GetUInt64(Bullet::HostGuidUInt64));
        if (g_Fsm->GetState(host->Guid()) == AutoConfig::Enum::eState::Death && bulletConfig->IsFollowHost)
        {
            OnBulletRelease(bullet);
            return;
        }
        IGameObject* hitEntity = g_GameWorld->QueryGameObject(bullet->GetUInt64(Bullet::HitUnitGuidUInt64));
        if (bullet->GetUInt64(Bullet::HitUnitGuidUInt64) != 0 && (hitEntity == nullptr || g_Fsm->GetState(hitEntity->Guid()) == AutoConfig::Enum::eState::Death))
        {
            OnBulletRelease(bullet);
            return;
        }
        switch (bulletConfig->Ballistics)
        {
        case AutoConfig::Enum::eBallistics::Line:
        {
            float flyDistance = bullet->GetFloat(Entity::SpeedFloat) * (intervalTime / 1000) + 0.5 * bulletConfig->AddSpeed * (intervalTime / 1000) * (intervalTime / 1000);
            double PI = std::acos(-1.0);
            double radians = bullet->Angle() * PI / 180.0f;

            const AutoConfig::Struct::Bullet* config = g_Config->GetAutoConfig()->QueryBulletConfig(bullet->ConfigID());
			IScene* scene = g_SceneManager->QuerySceneByEntityGuid(bullet->Guid());
            XASSERT(scene && config, "wtf");
            if (scene && config) {
                ITileLayer* layer = scene->FindTileLayer(config->CanMoveLayer);
				if (layer) {
					const Vector2 pos(bullet->Position().x + std::cos(radians) * flyDistance, bullet->Position().y + std::sin(radians) * flyDistance);
                    if (!layer->HasGrid(pos)) {
                        OnBulletRelease(bullet);
                    }
                    else {
						bullet->SetPosition(pos);
                    }

                    break;
                }
            }

            OnBulletRelease(bullet);
            break;
        }
        case AutoConfig::Enum::eBallistics::Parabolic:
        {
            float deltaTime = bullet->GetFloat(Bullet::DeltaTimeFloat) + AutoConfig::Const::PositionSolide::Solide;
            float t = deltaTime / bulletConfig->LifeCycle;
            float x = (1 - t) * (1 - t) * bullet->GetVector2(Bullet::StartPosVector2).x + 2 * (1 - t) * t * bullet->GetVector2(Bullet::ControlPosVector2).x + t * t * bullet->GetFloat(Bullet::TargetPosXFloat);
            float y = (1 - t) * (1 - t) * bullet->GetVector2(Bullet::StartPosVector2).y + 2 * (1 - t) * t * bullet->GetVector2(Bullet::ControlPosVector2).y + t * t * bullet->GetFloat(Bullet::TargetPosYFloat);

            bullet->SetFloat(Bullet::DeltaTimeFloat, deltaTime);
            bullet->SetPosition(x, y);

            break;
        }
        case AutoConfig::Enum::eBallistics::Track:
        {
            float flyDistance = bullet->GetFloat(Entity::SpeedFloat) * (intervalTime / 1000);
            double PI = std::acos(-1.0);
            double radians = bullet->Angle() * PI / 180.0f;
            bullet->SetPosition(bullet->Position().x + std::cos(radians) * flyDistance, bullet->Position().y + std::sin(radians) * flyDistance);
            break;
        }
        }
        break;
    }
    case eBulletTimer::BulletUpdateAngle:
    {
        switch (bulletConfig->Ballistics)
        {
        case AutoConfig::Enum::eBallistics::Track:
        {
            IGameObject* target = g_GameWorld->QueryGameObject(bullet->GetUInt64(Launcher::TargetGuidUInt64));
            if (target != nullptr)
            {
                float angle = GeometryUtils::CalculateAngleBetweenPoints(bullet->Position(), target->Position());
                float currentAngle = bullet->Angle();
                float targetAngle = angle;
                float angleDiff = targetAngle - currentAngle;
                float minAngleDiff = std::fmod(angleDiff + 180.0f, 360.0f) - 180.0f;
                float newAngle;
                if (std::abs(minAngleDiff) <= bulletConfig->MaxTurnAngle) {
                    newAngle = currentAngle + minAngleDiff;
                }
                else {
                    if (minAngleDiff > 0) {
                        newAngle = currentAngle + bulletConfig->MaxTurnAngle;
                    }
                    else {
                        newAngle = currentAngle - bulletConfig->MaxTurnAngle;
                    }
                }
                bullet->SetAngle(GeometryUtils::NormalizeAngle(newAngle));
            }

            break;
        }
        }

        break;
    }
    case eBulletTimer::BulletCollision:
    {
        scene->GetVisual(bullet->Position(), bulletConfig->Radius + 5,
            [&](const std::vector<UInt64>& visual) {
                bool isBulletRelease = false;
                for (auto visualIt = visual.begin(); visualIt != visual.end(); visualIt++) {
                    IGameObject* go = g_GameWorld->QueryGameObject(*visualIt);
                    IGameObject* host = g_GameWorld->QueryGameObject(bullet->GetUInt64(Launcher::HostGuidUInt64));
                    if (go == nullptr || go->GetBool(Entity::IsCantDamagedBool))
                    {
                        continue;
                    }
                    if (g_Fsm->GetState(go->Guid()) == AutoConfig::Enum::eState::Death)
                    {
                        continue;
                    }
                    if (sceneConfig->IsPvP == false && host->Is(eGameObjectType::Character) && go->Is(eGameObjectType::Character))
                    {
                        UInt64 teamIdHost = host->GetUInt64(Entity::TeamGuidUInt64);
                        UInt64 teamIdGo = go->GetUInt64(Entity::TeamGuidUInt64);
                        if (teamIdGo != teamIdHost || (teamIdGo == INVALID_GAMEOBJECT_ID && teamIdHost == INVALID_GAMEOBJECT_ID))
                        {
                            if (go->Guid() != host->Guid())
                            {
                                continue;
                            }
                        }
                    }
                    if (!isBulletRelease)
                    {
                        if (bulletConfig->CollisionCount == 0 || bullet->GetInt64(Bullet::DamageCountInt64) < bulletConfig->CollisionCount)
                        {
                            if (BulletCollisionCheck(bullet, go))
                            {

                                for (int i = 0; i < bulletConfig->TargetType.size(); i++)
                                {
                                    switch (bulletConfig->TargetType[i])
                                    {
                                    case AutoConfig::Enum::eTargetType::All:
                                    {
                                        break;
                                    }
                                    case AutoConfig::Enum::eTargetType::EnemyCharacter:
                                    {
                                        if (host != nullptr && go->Is(eGameObjectType::Character) && host->Is(eGameObjectType::Character))
                                        {
                                            UInt64 teamIdHost = host->GetUInt64(Entity::TeamGuidUInt64);
                                            UInt64 teamIdGo = go->GetUInt64(Entity::TeamGuidUInt64);
                                            if (go->Guid() != host->Guid() && (teamIdGo != teamIdHost || (teamIdGo == INVALID_GAMEOBJECT_ID && teamIdHost == INVALID_GAMEOBJECT_ID)))
                                            {
                                                OnBulletDamageCheck(bullet, go, isBulletRelease);
                                            }
                                        }
                                        break;
                                    }
                                    case AutoConfig::Enum::eTargetType::Monster:
                                    {
                                        if (go->Is(eGameObjectType::Monster))
                                        {
                                            OnBulletDamageCheck(bullet, go, isBulletRelease);
                                        }
                                        break;
                                    }
                                    case AutoConfig::Enum::eTargetType::PlayerCharacter:
                                    {
                                        if (host != nullptr && go->Is(eGameObjectType::Character) && host->Is(eGameObjectType::Character))
                                        {
                                            UInt64 teamIdHost = host->GetUInt64(Entity::TeamGuidUInt64);
                                            UInt64 teamIdGo = go->GetUInt64(Entity::TeamGuidUInt64);
                                            if (go->Guid() != host->Guid() && teamIdHost == teamIdGo && teamIdHost != INVALID_GAMEOBJECT_ID)
                                            {
                                                OnBulletDamageCheck(bullet, go, isBulletRelease);
                                            }
                                        }
                                        break;
                                    }
                                    case AutoConfig::Enum::eTargetType::PlayerSelf:
                                    {
                                        if (host != nullptr && go->Guid() == host->Guid())
                                        {
                                            OnBulletDamageCheck(bullet, go, isBulletRelease);
                                        }
                                        break;
                                    }
                                    case AutoConfig::Enum::eTargetType::Character:
                                    {
                                        if (go->Type() == eGameObjectType::Character)
                                        {
                                            OnBulletDamageCheck(bullet, go, isBulletRelease);
                                        }
                                        break;
                                    }
                                    }
                                }

                                if (host && host->Is(eGameObjectType::Monster))
                                {
                                    if (host && host->GetBool(Entity::CastIsTrackOnCollisionBool))
                                    {
                                        host->SetPosition(host->GetVector2(Monster::PreviousPositionVector2));
                                        g_Fsm->EnterState(host->Guid(), AutoConfig::Enum::eState::Idle, host->GetVector2(Monster::PreviousPositionVector2), host->Angle());
                                    }
                                }

                                if (isBulletRelease)
                                {
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        );
        break;
    }
    case eBulletTimer::BulletRelease:
    {

        if (bulletConfig->LifePanType == AutoConfig::Enum::eLifePanType::HurtLife && bullet->GetUInt64(Bullet::HitUnitGuidUInt64) != 0)
        {
            IGameObject* host = g_GameWorld->QueryGameObject(bullet->GetUInt64(Launcher::HostGuidUInt64));
            START_TIMER(g_Engine, s_Self, eBulletTimer::BulletAddHurtLife, bulletConfig->AddHurtLife, 1, 0, bullet->Guid());
            //host->SetBool(Entity::IsCantDamagedBool, false);
            return;
        }
        OnBulletRelease(bullet);

        break;
    }
    case eBulletTimer::BulletAddHurtLife:
    {
        OnBulletRelease(bullet);
        break;
    }
    }
}

void SkillBullet::OnSceneCreate(IScene* const scene) {

}

void SkillBullet::OnSceneRelease(IScene* const scene) {
    auto itor = s_SceneBulletMap.find(scene->SceneID());
    if (itor != s_SceneBulletMap.end()) 
    {
        std::vector<UInt64> bulletGuids(itor->second.begin(), itor->second.end());

        for (auto guid : bulletGuids)
        {
            IGameObject* bullet = g_GameWorld->QueryGameObject(guid);
            if (bullet != nullptr)
            {
                STOP_TIMER(g_Engine, s_Self, eBulletTimer::BulletUpdatePosition, bullet->Guid());
                STOP_TIMER(g_Engine, s_Self, eBulletTimer::BulletUpdateAngle, bullet->Guid());
                STOP_TIMER(g_Engine, s_Self, eBulletTimer::BulletCollision, bullet->Guid());
                STOP_TIMER(g_Engine, s_Self, eBulletTimer::BulletRelease, bullet->Guid());
                STOP_TIMER(g_Engine, s_Self, eBulletTimer::BulletAddHurtLife, bullet->Guid());

                s_Self->OnBulletRelease(bullet);
            }
        }

        s_SceneBulletMap.erase(scene->SceneID());
    }
}

void SkillBullet::OnBulletDamage(IGameObject* const bullet, IGameObject* const target)
{
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(bullet->Guid());
    const AutoConfig::Struct::Bullet* bulletConfig = GetConfig(g_Config, Bullet, bullet->ConfigID());

    g_SkillDamage->AddDamage(bullet, target, bulletConfig->SkillDamage, bulletConfig->DamageType);
    IGameObject* host = g_GameWorld->QueryGameObject(bullet->GetUInt64(Bullet::HostGuidUInt64));

    //加buff
    for (int i = 0; i < bulletConfig->BuffConfigIDs.size(); i++)
    {
        if (bulletConfig->BuffConfigIDs[i] != -1)
        {
            g_BuffManager->AddBuff(host, target, bulletConfig->BuffConfigIDs[i]);
        }
    }
}

void SkillBullet::OnBulletRelease(IGameObject* bullet)
{
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(bullet->Guid());
    if (scene == nullptr)
    {
        ErrorLog(g_Engine, "bullet %llu is not in a scene", bullet->Guid());
    }

    const AutoConfig::Struct::Bullet* bulletConfig = GetConfig(g_Config, Bullet, bullet->ConfigID());
    IGameObject* host = g_GameWorld->QueryGameObject(bullet->GetUInt64(Launcher::HostGuidUInt64));

    if (bulletConfig->LifePanType == AutoConfig::Enum::eLifePanType::HurtLife && host != nullptr)
    {
        host->SetBool(Entity::IsCantDamagedBool, true);
        host->SetFloat(Entity::HpFloat, host->GetFloat(Entity::MaxHpFloat));
    }

    oBulletReleasePush push;
    push.set_bulletguid(bullet->Guid());

    s_BroadcastTemp.clear();
    scene->GetVisual(bullet->Guid(),
        [&](const std::unordered_set<UInt64>& visual) {
            for (auto visualIt = visual.begin(); visualIt != visual.end(); visualIt++) {
                IGameObject* g = g_GameWorld->QueryGameObject(*visualIt);
                if (g && g->Is(eGameObjectType::Character)) {
                    s_BroadcastTemp.push_back(g->GetUInt64(Character::AccountUInt64));
                }
            }
        }
    );

    g_PlayerManager->Broadcast(s_BroadcastTemp, eServerID::BulletReleasePush, push);

    STOP_TIMER(g_Engine, this, eBulletTimer::BulletUpdatePosition, bullet->Guid());
    STOP_TIMER(g_Engine, this, eBulletTimer::BulletUpdateAngle, bullet->Guid());
    STOP_TIMER(g_Engine, this, eBulletTimer::BulletCollision, bullet->Guid());
    STOP_TIMER(g_Engine, this, eBulletTimer::BulletRelease, bullet->Guid());
    STOP_TIMER(g_Engine, this, eBulletTimer::BulletAddHurtLife, bullet->Guid());

    /*scene->ExitScene(bullet->Guid(),
        [&](IScene* const, const UInt64 guid, const UInt64 other) {

        }
    );*/
    auto it = s_BulletCollisionInfo.find(bullet->Guid());
    if (it != s_BulletCollisionInfo.end())
    {
        s_BulletCollisionInfo.erase(it);
    }
    else
    {
        ErrorLog(g_Engine, "BulletCollision Info has not bullet %llu", bullet->Guid());
    }

    auto it2 = s_SceneBulletMap.find(scene->SceneID());
    if (it2 != s_SceneBulletMap.end())
    {
        it2->second.erase(bullet->Guid());
    }
    else
    {
        ErrorLog(g_Engine, "SceneBulletMap not exit sceneID %llu", scene->SceneID());
    }

    g_GameWorld->ReleaseGameObject(bullet);
}

void SkillBullet::OnBulletDamageCheck(IGameObject* const bullet, IGameObject* const target, bool& isBulletRelease)
{
    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(bullet->Guid());
    const AutoConfig::Struct::Bullet* bulletConfig = GetConfig(g_Config, Bullet, bullet->ConfigID());
    if (bulletConfig->EffectTarget && target->Guid() != bullet->GetUInt64(Bullet::TargetGuidUInt64))
    {
        return;
    }
    //矫正碰撞时间
    auto itor = s_BulletCollisionInfo.find(bullet->Guid());
    if (itor == s_BulletCollisionInfo.end())
    {
        TraceLog(g_Engine, "Bullet Collision Info Is Not Exist::%llu", bullet->Guid());
        return;
    }
    auto itor2 = itor->second.begin();
    UInt64 currentTime = SafeSystem::Time::GetMilliSecond();
    for (; itor2 != itor->second.end(); ++itor2)
    {
        if (itor2->_TargetGuid == target->Guid())
        {
            if ((currentTime - itor2->_CollisionTick) < bulletConfig->IntervalTime)
            {
                return;
            }
            else
            {
                itor2->_CollisionTick = currentTime;
            }
            break;
        }
    }

    if (itor2 == itor->second.end())
    {
        itor->second.push_back({ target->Guid(), currentTime });
    }

    IGameObject* host = g_GameWorld->QueryGameObject(bullet->GetUInt64(Bullet::HostGuidUInt64));
    if (host->GetBool(Entity::CastIsTrackOnCollisionBool))
    {
        bullet->SetUInt64(Bullet::HitUnitGuidUInt64, target->Guid());
        IGameObject* hitUnit = g_GameWorld->QueryGameObject(bullet->GetUInt64(Bullet::HitUnitGuidUInt64));
        hitUnit->SetPosition(bullet->Position());
        g_Fsm->EnterState(hitUnit->Guid(), AutoConfig::Enum::eState::Idle, hitUnit->Position(), hitUnit->Angle());
    }

    host->SetBool(Entity::IsCantDamagedBool, false);

    bullet->SetInt64(Bullet::DamageCountInt64, bullet->GetInt64(Bullet::DamageCountInt64) + 1);
    OnBulletDamage(bullet, target);

    //发送消息
    if (bulletConfig->CollisionCount != 0 && bullet->GetInt64(Bullet::DamageCountInt64) >= bulletConfig->CollisionCount && bulletConfig->IsDisappera)
    {
        isBulletRelease = true;
        OnBulletRelease(bullet);
    }
}

bool SkillBullet::Destroy(Api::iEngine* engine) {
    return true;
}