#pragma once

#include "IPlayerManager.h"
#include "iTimerApi.h"
#include "IBackpack.h"
#include "IServerManager.h"
#include "XPool.h"
#include "SafeString.h"
#include "ICharacterManager.h"
#include "IConfig.h"
#include "ICacheClient.h"
#include "IGameWorld.h"
#include "AutoConfig.h"
#include "XBase64.h"
#include "Game.pb.h"
#include "IFsm.h"
#include "ITaskManager.h"
#include <vector>
UsingXEngine;

extern Api::iEngine* g_Engine;
extern ICharacterManager* g_CharacterManager;
extern IConfig* g_Config;
extern ICacheClient* g_CacheClient;
extern IServerManager* g_ServerManager;
extern IPlayerManager* g_PlayerManager;
extern IGameWorld* g_GameWorld;
extern IFsm* g_Fsm;
