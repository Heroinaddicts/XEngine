#pragma once

#include "iTimerApi.h"
#include "IPlayerManager.h"
#include "XPool.h"
#include "SafeString.h"
#include "Game.pb.h"
#include "XEventPool.h"
#include "ICacheClient.h"
#include "ISceneManager.h"
#include "IConfig.h"
#include "IGameWorld.h"
#include "ICharacterManager.h"

#include "IGame.h"
UsingXEngine;

extern Api::iEngine* g_Engine;
extern ICacheClient* g_CacheClient;
extern ISceneManager* g_SceneManager;
extern IConfig* g_Config;

extern IGame* g_Game;
extern IGameWorld* g_GameWorld;
extern ICharacterManager* g_CharacterManager;

class PlayerManager;
extern PlayerManager* g_PlayerManager;

enum class ePlayerStatus {
    Loading,
    NeedCreate,
    Creating,
    Loaded
};
