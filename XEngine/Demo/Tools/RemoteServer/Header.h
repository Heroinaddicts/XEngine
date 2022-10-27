#ifndef __Header_h__
#define __Header_h__

#include "iComponent.h"
#include "iNetApi.h"
#include "iTimerApi.h"
#include "SafeString.h"
#include "SafeSystem.h"
using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_engine;

#include "RemoteProto.h"

class AgentConnection;
class UserConnection;

extern AgentConnection* g_agentConnection;
extern UserConnection* g_userConnection;

#endif //__Header_h__
