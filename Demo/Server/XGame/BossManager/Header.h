#pragma once

#include "iTimerApi.h"
#include "IBossManager.h"
#include "XPool.h"
#include "IGameWorld.h"
#include "SafeString.h"
#include "Game.pb.h"
#include "IConfig.h"
#include "HydraCypress.h"
#include "ISceneManager.h"
#include "ISkillManager.h"
#include "IPlayerManager.h"
#include "Vector2.h"
#include "IFsm.h"
#include <unordered_map>
#include <atomic>

UsingXEngine;

extern IConfig* g_Config;
extern IGameWorld* g_GameWorld;
extern ISceneManager* g_SceneManager;
extern ISkillManager* g_SkillManager;
extern IPlayerManager* g_PlayerManager;
extern IFsm* g_Fsm;

extern Api::iEngine* g_Engine;

struct BossTimerData {
    UInt64 guid;
    Int64 skillConfig;
    int timerType;
    UInt64 timerId;
    Vector2 previousPosition;
    bool isPreviousSuccess;
};

