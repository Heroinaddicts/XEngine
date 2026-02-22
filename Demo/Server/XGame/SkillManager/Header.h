#pragma once

#include "iTimerApi.h"
#include "ISkillManager.h"
#include "ISkillEffect.h"
#include "ISkillEvent.h"
#include "XPool.h"
#include "SafeString.h"
#include "Game.pb.h"
#include "IConfig.h"
#include "IGameWorld.h"
#include "ICharacterManager.h"
#include "ICacheClient.h"
#include "IPlayerManager.h"
#include "ISkillLauncher.h"
#include "ISkillBullet.h"
#include "ISkillMove.h"
#include "ISkillDamage.h"
#include "ISceneManager.h"
#include "IFsm.h"
#include "IBuffManager.h"
#include "ITilemapManager.h"
#include "SafeTools.h"
#include <cmath>
#include <random>
#include <Geometry/GeometryUtils.h>

UsingXEngine;

extern Api::iEngine* g_Engine;
extern IConfig* g_Config;
extern IGameWorld* g_GameWorld;
extern ICharacterManager* g_CharacterManager;
extern ICacheClient* g_CacheClient;
extern IPlayerManager* g_PlayerManager;
extern ISceneManager* g_SceneManager;
extern IFsm* g_Fsm;
extern IBuffManager* g_BuffManager;
extern ISkillBullet* g_SkillBullet;
extern IBuffManager* g_BuffManager;
extern IGameWorld* g_GameWorld;
extern ISkillMove* g_SkillMove;
extern ISkillDamage* g_SkillDamage;

struct SkillInfo {
    const UInt64 _CharacterID;
    bool _IsLoaded;
    std::map<Int64, IGameObject*> _SkillMap;

    SkillInfo(const UInt64 characterId) : _CharacterID(characterId), _IsLoaded(false) {}
};

struct CollisionInfo {
    UInt64 _TargetGuid;
    UInt64 _CollisionTick;
};

struct MoveData
{
	IGameObject* _Cast;
	UInt64 _SkillGuid;
	Int64 _SkillConfig;
	UInt64 _TargetGuid;
	Int64 _MoveID;
	Int32 _DamageCount;
	float _CastSpeed;
	UInt64 _LastUpdateTimeTick;
	UInt64 _ActualMoveTime;
	Vector2 _EndPoint;
};
