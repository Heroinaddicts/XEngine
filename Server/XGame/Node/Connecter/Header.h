#ifndef __Header_h__
#define __Header_h__

#include "iConnecter.h"

#include "iNetApi.h"
#include "iTimerApi.h"

#include "NodeProto.h"

#include "SafeString.h"

#include <set>
#include <map>

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_Engine;
class Connecter;
extern Connecter* g_Connecter;

#define INVALID_NODE_ID -1
#define NODE_SESSION_PIPE_SIZE (1*1024*1024)

namespace TimerConfig {
    enum eID {
        NodeHeartBeat,
        Reconnect
    };

    const int NodeHeartBeatInterval = 5000; //单位: 毫秒
    const int ReconnectInterval = 500; //单位: 毫秒
}

extern std::set<iConnecter::fSessionEvent> g_SessionAppearEventPool;
extern std::set<iConnecter::fSessionEvent> g_SessionDisappearEventPool;

class NodeSession;
extern std::map<int, NodeSession*> g_NodeSessionMap;

#endif //__Header_h__
