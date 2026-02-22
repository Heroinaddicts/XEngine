#pragma once

#include "iTimerApi.h"
#include "IGate.h"
#include "XPool.h"
#include "XEventPool.h"

#include "SafeString.h"
#include "SafeSystem.h"
#include "SafeTools.h"

#include "Game.pb.h"
#include "Framework.pb.h"

#include "FrameworkSession.h"
#include "IDb.h"

#include "RedisDef.h"
#include "XBase64.h"

#include <set>
UsingXEngine;

extern Api::iEngine* g_Engine;
extern IDb* g_Db;
extern IDbGroup* g_DbGroup;

extern FrameworkSession* g_GameServerSession;

extern XEventPool<IGate::eGateEvents, IClient* const> g_GateEventsPool;
extern std::map<UInt16, IGate::fClientMessager> g_ClientMessagePool;

__forceinline std::string GenerateToken(const UInt64 account) {
    std::string token = XBase64::Encode(SafeString::Int64ToString(account) + SafeString::Int64ToString(SafeSystem::Time::GetMicroSecond()) + SafeString::Int64ToString(SafeTools::Rand()));
    return token;
}

extern UInt64 g_ClientSessionCount;
