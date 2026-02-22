#pragma once

#include "iTimerApi.h"
#include "IMovement.h"
#include "XPool.h"
#include "SafeString.h"

#include "IGameWorld.h"
#include "ISceneManager.h"
#include "ITilemapManager.h"
#include "IPlayerManager.h"

#include "Vector2.h"
#include "Game.pb.h"
#include "IFsm.h"

#include "IConfig.h"

UsingXEngine;

#include <list>
#include <vector>
#include <unordered_map>

extern Api::iEngine* g_Engine;

extern IFsm* g_Fsm;
extern IGameWorld* g_GameWorld;
extern ISceneManager* g_SceneManager;
extern IPlayerManager* g_PlayerManager;
