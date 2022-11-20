#ifndef __Header_h__
#define __Header_h__

#include "iTestRedis.h"
#include "iTimerApi.h"

#include "iRedis.h"

#include "SafeString.h"

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_Engine;
class TestRedis;
extern TestRedis* g_TestRedis;

extern iRedis* g_Redis;
#endif //__Header_h__
