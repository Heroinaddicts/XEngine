#pragma once

#include "iTimerApi.h"
#include "IMonsterManager.h"
#include "ISceneManager.h"
#include "XPool.h"
#include "SafeString.h"
#include "Game.pb.h"
#include "IConfig.h"
#include "IGameWorld.h"
#include "IMovement.h"
#include "IFsm.h"
#include "SafeTools.h"
#include "IPlayerManager.h"
#include "ISkillManager.h"
#include "GeometryUtils.h"

UsingXEngine;

#include <unordered_map>
#include <unordered_set>

extern Api::iEngine* g_Engine;
extern IConfig* g_Config;
extern IGameWorld* g_GameWorld;
extern ISceneManager* g_SceneManager;
extern IMovement* g_Movement;
extern IFsm* g_Fsm;
extern IPlayerManager* g_PlayerManager;
