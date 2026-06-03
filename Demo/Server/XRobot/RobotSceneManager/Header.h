#pragma once

#include "IRobotSceneManager.h"
#include "ITilemapManager.h"
#include "IConfig.h"
#include "IGameRobot.h"
#include "IGameWorld.h"

UsingXEngine;

extern Api::iEngine* g_Engine;
extern ITilemapManager* g_TilemapManager;
extern IConfig* g_Config;
extern IGameRobot* g_GameRobot;
extern IGameWorld* g_GameWorld;

extern std::map<Int64, IIsometricTilemap*> g_Tilemaps;

