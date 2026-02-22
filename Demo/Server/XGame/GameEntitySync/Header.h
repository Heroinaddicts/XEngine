#pragma once

#include "iTimerApi.h"
#include "IGameEntitySync.h"
#include "IGameWorld.h"
#include "XPool.h"
#include "SafeString.h"
#include "Game.pb.h"
#include "ICharacterManager.h"
#include "ISceneManager.h"
#include "IPlayerManager.h"
#include "IFsm.h"

UsingXEngine;

extern Api::iEngine* g_Engine;
extern IGameWorld* g_GameWorld;
extern ICharacterManager* g_CharacterManager;
extern ISceneManager* g_SceneManager;
extern IPlayerManager* g_PlayerManager;
extern IFsm* g_Fsm;

