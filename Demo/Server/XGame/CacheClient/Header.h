#pragma once

#include "iTimerApi.h"
#include "ICacheClient.h"
#include "XPool.h"

#include "SafeTools.h"
#include "SafeString.h"
#include "FrameworkSession.h"
#include "Framework.pb.h"

UsingXEngine;
UsingSQLCacheTemplate;

extern Api::iEngine* g_Engine;
extern FrameworkSession* g_CacheServerSession;

extern ISQLCache* g_SQLCache;

