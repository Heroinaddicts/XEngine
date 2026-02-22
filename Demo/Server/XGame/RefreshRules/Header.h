#pragma once

#include "iTimerApi.h"
#include "IRefreshRules.h"
#include "ISceneManager.h"
#include "XPool.h"
#include "SafeString.h"
#include "Game.pb.h"
#include "IConfig.h"
#include "IGameWorld.h"
#include "IMonsterManager.h"
#include "IBossManager.h"

#include "SafeTools.h"

UsingXEngine;

#include <unordered_map>
#include <unordered_set>

extern Api::iEngine* g_Engine;
extern IConfig* g_Config;
extern IGameWorld* g_GameWorld;
extern ISceneManager* g_SceneManager;

extern IMonsterManager* g_MonsterManager;
extern IBossManager* g_BossManager;
