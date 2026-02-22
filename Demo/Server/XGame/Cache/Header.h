#pragma once

#include "iTimerApi.h"
#include "ICache.h"
#include "XPool.h"
#include "ISQLCache.h"
#include "SafeString.h"
#include "FrameworkSession.h"
#include "IDb.h"
#include "Framework.pb.h"
UsingXEngine;

extern Api::iEngine* g_Engine;
extern ISQLCache* g_SQLCache;

extern IDb* g_Db;
extern IDbGroup* g_DbGroup;

extern FrameworkSession* g_CacheClientSession;
