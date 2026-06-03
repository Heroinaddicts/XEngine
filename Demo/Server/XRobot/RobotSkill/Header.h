#pragma once

#include "IRobotSkill.h"
#include "IGameRobot.h"
#include "ITilemapManager.h"
#include "IConfig.h"
#include "IGameWorld.h"
#include "IRobotSceneManager.h"

#include "SafeTools.h"

#include "iTimerApi.h"

#include "Vector2.h"

UsingXEngine;
#include "Game.pb.h"

extern Api::iEngine* g_Engine;
extern IGameRobot* g_GameRobot;
extern IConfig* g_Config;
extern IGameWorld* g_GameWorld;
extern IRobotSceneManager* g_RobotSceneManager;
