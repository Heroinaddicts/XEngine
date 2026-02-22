#pragma once

#include "iTimerApi.h"
#include "IBuffManager.h"
#include "XPool.h"
#include "SafeString.h"
#include "IGameWorld.h"
#include "IConfig.h"
#include "IFsm.h"
#include "Game.pb.h"
#include "IPlayerManager.h"
#include "ICharacterManager.h"
#include "ISceneManager.h"
UsingXEngine;

extern Api::iEngine* g_Engine;
extern IGameWorld* g_GameWorld;
extern IConfig* g_Config;
extern IFsm* g_Fsm;
extern IPlayerManager* g_PlayerManager;
extern ISceneManager* g_SceneManager;

