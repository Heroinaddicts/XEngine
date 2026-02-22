#pragma once

#include "iTimerApi.h"
#include "IGameWorld.h"
#include "IGame.h"
#include "XPool.h"

#include "SafeString.h"
#include "XEventPool.h"

#include "Framework.pb.h"
#include "FrameworkSession.h"

#include "RedisDef.h"
#include "iRedisApi.h"

#include "IServerManager.h"

UsingXEngine;

extern Api::iEngine* g_Engine;
extern IGameWorld* g_GameWorld;
extern IServerManager* g_ServerManager;

extern Api::iRedisSession* g_RedisSession;

extern FrameworkSession* g_GateServerSession;
extern FrameworkSession* g_CacheServerSession;

extern XEventPool<IGame::eGameEvent, const UInt64> g_GameEventPool;

