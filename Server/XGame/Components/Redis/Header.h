#ifndef __Header_h__
#define __Header_h__

#include "iRedis.h"

#include <set>

#include "SafeSystem.h"
#include "SafeString.h"
#include "SafeQueue.h"
#include "SafeThread.h"

#include "XPool.h"
#include "XLock.h"

#include "hiredis.h"

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_Engine;
class Redis;
extern Redis* g_Redis;

class RedisPipe;
extern std::set<RedisPipe*> g_PipeSet;

#endif //__Header_h__
