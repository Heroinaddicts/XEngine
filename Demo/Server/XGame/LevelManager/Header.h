#pragma once

#include "iTimerApi.h"
#include "ILevelManager.h"
#include "IGameWorld.h"
#include "IFsm.h"
#include "IConfig.h"

#include "XPool.h"
#include "SafeString.h"
#include "XEventPool.h"
#include "Game.pb.h"

UsingXEngine;

extern Api::iEngine* g_Engine;
extern IGameWorld* g_GameWorld;
extern IFsm* g_Fsm;
extern IConfig* g_Config;

extern XEventPool<ILevelManager::eEvent, IGameObject* const, const UInt64, const UInt64> g_EventPool;
