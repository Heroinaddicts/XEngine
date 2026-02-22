#include "SkillLauncher.h"

static SkillLauncher* s_Self = nullptr;

static std::unordered_map<UInt64, IGameObject*> s_GameObjectLaunchers;

enum eLauncherTimer
{
	LauncherBeat = 0,
	LauncherRelease = 1,
};

bool SkillLauncher::Initialize(Api::iEngine* const engine) {
	s_Self = this;
	return true;
}

bool SkillLauncher::Launch(Api::iEngine* const engine) {
	GetModule(g_Engine, g_SkillBullet, SkillBullet);
	GetModule(g_Engine, g_PlayerManager, PlayerManager);
	GetModule(g_Engine, g_GameWorld, GameWorld);

	g_GameWorld->RegistGameObjectEvent(eGameObjectType::Entity, IGameWorld::eGameWorldEvent::Create, OnGameEntityCreate);
	g_GameWorld->RegistGameObjectEvent(eGameObjectType::Entity, IGameWorld::eGameWorldEvent::Release, OnGameEntityRelease);

	return true;
}

void SkillLauncher::OnLauncherTest(IPlayer* const player, const UInt64 account, const oLauncherTest& req)
{
	/*IGameObject* character = g_GameWorld->QueryGameObject(player->GetDeployCharacterID());
	XASSERT(character != nullptr, "wtf");
	IGameObject* target = g_GameWorld->QueryGameObject(3622700833202911232);
	Vector2 a = target->Position();
	s_Self->AddLauncher(character, req.launcherconfigid(), target, &a);*/
}

UInt64 SkillLauncher::AddLauncher(IGameObject* const cast, const Int64 launcherID, const float angle, const Vector2& targetPos, const Vector2& placePos, const UInt64 targetGuid)
{
	const AutoConfig::Struct::Launcher* launcherConfig = GetConfig(g_Config, Launcher, launcherID);
	if (nullptr == launcherConfig)
	{
		ErrorLog(g_Engine, "Where is launcher config %lld", launcherID);
		return INVALID_GAMEOBJECT_ID;
	}

	IGameObject* launcher = g_GameWorld->CreateGameObject(eGameObjectType::Launcher, launcherID, AUTO_GUID,
		[&](IGameObject* const go) {
			go->SetUInt64(Launcher::HostGuidUInt64, cast->Guid());
			go->SetUInt64(Launcher::TargetGuidUInt64, targetGuid);
			go->SetFloat(Launcher::TargetPosXFloat, targetPos.x);
			go->SetFloat(Launcher::TargetPosYFloat, targetPos.y);
		}
	);
	if (launcherConfig->LauncherPositionType == AutoConfig::Enum::ePosition::WorldPosition)
	{
		launcher->SetPosition(placePos.x, placePos.y);
	}
	launcher->SetAngle(angle);
	
	START_TIMER(g_Engine, this, eLauncherTimer::LauncherBeat, launcherConfig->FireDelay, launcherConfig->FireCount, launcherConfig->FireInterval, launcher->Guid());
	START_TIMER(g_Engine, this, eLauncherTimer::LauncherRelease, launcherConfig->FireDelay + launcherConfig->FireCount * launcherConfig->FireInterval, 1, 0, launcher->Guid());

	s_GameObjectLaunchers.insert({ launcher->Guid(), launcher });

	return launcher->Guid();
}

void SkillLauncher::OnTimer(const int id, const UInt64 context, const Int64 timestamp)
{
	IGameObject* launcher = g_GameWorld->QueryGameObject(context);
	const AutoConfig::Struct::Launcher* launcherConfig = GetConfig(g_Config, Launcher, launcher->ConfigID());
	IGameObject* host = g_GameWorld->QueryGameObject(launcher->GetUInt64(Launcher::HostGuidUInt64));
	switch (id) {
		case eLauncherTimer::LauncherBeat:
		{
			if (launcherConfig->LauncherPositionType == AutoConfig::Enum::ePosition::TraceHost && host != nullptr)
			{
				Vector2 offset(launcherConfig->PositionOffset[0], launcherConfig->PositionOffset[1]);

				Vector2 center(0, launcherConfig->PositionOffset[1]);

				float angleRad = GeometryUtils::ToRadians(launcher->Angle());
				float cosTheta = std::cos(angleRad);
				float sinTheta = std::sin(angleRad);

				// 计算相对于中心点的偏移
				Vector2 relativeOffset;
				relativeOffset.x = offset.x - center.x;
				relativeOffset.y = offset.y - center.y;

				// 绕中心点旋转
				Vector2 result;
				result.x = relativeOffset.x * cosTheta - relativeOffset.y * sinTheta;
				result.y = relativeOffset.x * sinTheta + relativeOffset.y * cosTheta;

				result.x += center.x;
				result.y += center.y;

				// 将结果加回中心点，再加上宿主的位置
				launcher->SetPosition(host->Position().x + result.x, host->Position().y + result.y);
			}
			for (int i = 0; i < launcherConfig->BulletConfigID.size(); i++)
			{
				g_SkillBullet->AddBullet(launcher, launcherConfig->BulletConfigID[i]);
			}
			break;
		}
		case eLauncherTimer::LauncherRelease:
		{
			STOP_TIMER(g_Engine, this, eLauncherTimer::LauncherBeat, context);
			STOP_TIMER(g_Engine, this, eLauncherTimer::LauncherRelease, context);

			auto it = s_GameObjectLaunchers.find(launcher->Guid());
			if (it != s_GameObjectLaunchers.end())
			{
				s_GameObjectLaunchers.erase(it);
			}
			else
			{
				ErrorLog(g_Engine, "GameObject Launchers has not launcher %llu", launcher->Guid());
			}
			
			break;
		}
	}
}

bool SkillLauncher::Destroy(Api::iEngine* const engine) {
	return true;
}

void SkillLauncher::OnGameEntityCreate(IGameObject* const go)
{
	
}

void SkillLauncher::OnGameEntityRelease(IGameObject* const go)
{
	for (auto it = s_GameObjectLaunchers.begin(); it != s_GameObjectLaunchers.end();)
	{
		const AutoConfig::Struct::Launcher* launcherConfig = GetConfig(g_Config, Launcher, it->second->ConfigID());

		if (it->second->GetUInt64(Launcher::HostGuidUInt64) == go->Guid() &&
			launcherConfig->RecycleWithDeath)
		{
			STOP_TIMER(g_Engine, s_Self, eLauncherTimer::LauncherBeat, it->second->Guid());
			STOP_TIMER(g_Engine, s_Self, eLauncherTimer::LauncherRelease, it->second->Guid());

			g_GameWorld->ReleaseGameObject(it->second);
			it = s_GameObjectLaunchers.erase(it);
		}
		else
		{
			++it;
		}
	}
}