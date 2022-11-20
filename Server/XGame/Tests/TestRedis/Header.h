#ifndef __Header_h__
#define __Header_h__

#include "iTestRedis.h"
#include "iTimerApi.h"

#include "iRedis.h"

#include "SafeString.h"
#include "SafeTools.h"
#include "SafeSystem.h"

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_Engine;
class TestRedis;
extern TestRedis* g_TestRedis;

extern iRedis* g_Redis;

extern int g_RedisSessionID;
#endif //__Header_h__
