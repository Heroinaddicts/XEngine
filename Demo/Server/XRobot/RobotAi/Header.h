#pragma once

#include "IRobotAi.h"
#include "IGameRobot.h"
#include "ITilemapManager.h"
#include "IConfig.h"
#include "IGameWorld.h"
#include "IRobotNpcManager.h"
#include "IRobotSceneManager.h"
#include "IRobotMovement.h"

#include "iTimerApi.h"
#include "SafeTools.h"

#include "Vector2.h"

UsingXEngine;
#include "Game.pb.h"

extern Api::iEngine* g_Engine;
extern IGameRobot* g_GameRobot;
extern IConfig* g_Config;
extern IGameWorld* g_GameWorld;
extern IRobotNpcManager* g_RobotNpcManager;
extern IRobotSceneManager* g_RobotSceneManager;
extern IRobotMovement* g_RobotMovement;
