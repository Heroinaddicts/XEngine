#pragma once

#include "iTimerApi.h"
#include "ICharacterManager.h"
#include "IGameWorld.h"
#include "XPool.h"
#include "SafeString.h"
#include "Game.pb.h"
#include "XEventPool.h"
#include "UnitCommon.h"
#include "IConfig.h"
#include "ICacheClient.h"
#include "IPlayerManager.h"
#include "ISceneManager.h"
#include "ITilemapManager.h"
#include "IFsm.h"
#include "IBackpack.h"
#include "iTimerApi.h"

UsingXEngine;
UsingUnitCommon;

extern Api::iEngine* g_Engine;
extern IConfig* g_Config;
extern ICacheClient* g_CacheClient;
extern IGameWorld* g_GameWorld;
extern IPlayerManager* g_PlayerManager;
extern ISceneManager* g_SceneManager;
extern IFsm* g_Fsm;
extern IBackpack* g_Backpack;

extern XEventPool<ICharacterManager::eCharacterDockerEvent, ICharacterDocker*, IGameObject*> g_CharacterDockerEvent;

extern std::vector<UInt64> g_BroadcastTemp;
