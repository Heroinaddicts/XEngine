#pragma once

#include "IRobotGameObjectSync.h"
#include "IGameRobot.h"
#include "ITilemapManager.h"
#include "IConfig.h"
#include "IGameWorld.h"

#include "Game.pb.h"

UsingXEngine;

extern Api::iEngine* g_Engine;
extern IGameRobot* g_GameRobot;
extern IConfig* g_Config;
extern IGameWorld* g_GameWorld;
