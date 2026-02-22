#pragma once

#include "ISceneManager.h"
#include "ITilemapManager.h"
#include "IConfig.h"
#include "IServerManager.h"
#include "IGameWorld.h"
#include "ICharacterManager.h"
#include "IPlayerManager.h"
#include "ITeamManager.h"
#include "SafeTools.h"

#include "Game.pb.h"
#include "XPool.h"
#include "IFsm.h"

#include "iTimerApi.h"
#include "iTaskApi.h"

#include "XEventPool.h"

#include "XAOI.h"

#include <vector>

UsingXEngine;

extern Api::iEngine* g_Engine;
extern IConfig* g_Config;
extern ITilemapManager* g_TilemapManager;
extern IServerManager* g_ServerManager;
extern IGameWorld* g_GameWorld;
extern ICharacterManager* g_CharacterManager;
extern IFsm* g_Fsm;
extern IPlayerManager* g_PlayerManager;
extern std::vector<UInt64> s_BroadCastTemp;

class SceneManager;
extern SceneManager* g_SceneManager;
