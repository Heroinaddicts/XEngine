#pragma once

#include "IGameRobot.h"
#include "iNetApi.h"
#include "iTimerApi.h"
#include "XPool.h"
#include "SafeString.h"
#include "SafeTools.h"
#include "SafeSystem.h"
#include "Game.pb.h"
#include "FrameworkSession.h"
#include "XEventPool.h"

UsingXEngine;

extern Api::iEngine* g_Engine;
extern int g_RobotCount;
extern int g_RobotSessionCount;

extern XEventPool<IGameRobot::eEvent, IRobotSession*> g_RobotEventPool;
extern std::unordered_map<UInt16, IGameRobot::fServerMessage> g_MessagePool;
