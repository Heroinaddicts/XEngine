#pragma once

#include "iTimerApi.h"
#include "ISQLCache.h"
#include "XPool.h"
#include "SafeString.h"
#include "Game.pb.h"

#include <vector>
#include <list>

UsingXEngine;

extern Api::iEngine* g_Engine;

class SQLCacheColumn;

extern std::string __GeneraterSQL__(const char* format, const std::vector<const SQLCacheColumn*>& list);
