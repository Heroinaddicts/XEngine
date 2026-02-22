#pragma once

#include "iTimerApi.h"
#include "ITeamManager.h"
#include "XPool.h"
#include "SafeString.h"
#include "Game.pb.h"
#include "XEventPool.h"
#include "IGameWorld.h"
#include "IPlayerManager.h"
#include "ICharacterManager.h"
#include "IConfig.h"
#include "IMatchManager.h"
#include "ISceneManager.h"

UsingXEngine;

extern Api::iEngine* g_Engine;
extern IGameWorld* g_GameWorld;
extern IPlayerManager* g_PlayerManager;
extern ICharacterManager* g_CharacterManager;
extern IConfig* g_Config;
extern IMatchManager* g_MatchManager;

extern XEventPool<ITeamManager::eEvent, ITeam* const, const UInt64, const UInt64> g_TeamEvent;
