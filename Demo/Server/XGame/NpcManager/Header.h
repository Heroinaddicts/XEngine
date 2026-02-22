#pragma once

#include "iTimerApi.h"
#include "INpcManager.h"
#include "XPool.h"
#include "SafeString.h"
#include "Game.pb.h"

UsingXEngine;

#include "IGameWorld.h"
#include "ISceneManager.h"
#include "IConfig.h"
#include "ICharacterManager.h"
#include "IPlayerManager.h"

extern Api::iEngine* g_Engine;

extern ISceneManager* g_SceneManager;
extern IConfig* g_Config;
extern IGameWorld* g_GameWorld;
extern ICharacterManager* g_CharacterManager;
extern IPlayerManager* g_PlayerManager;

