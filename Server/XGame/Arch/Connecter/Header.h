#ifndef __Header_h__
#define __Header_h__

#include "iConnecter.h"

#include "iNetApi.h"
#include "iTimerApi.h"

#include "SafeString.h"
#include "SafeSystem.h"

#include "Arch.pb.h"

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
        CheckHeartBeat,
        HeartBeat,
        Reconnect
    };

    const int HeartBeatInterval = 3000; //单位: 毫秒
    const int CheckHeartBeatInterval = 100; //单位: 毫秒
    const int ReconnectInterval = 500; //单位: 毫秒
}

extern std::map<unsigned_int64, RouteInfo> g_RouteInfos;

extern std::set<iConnecter::fSessionEvent> g_SessionAppearEventPool;
extern std::set<iConnecter::fSessionEvent> g_SessionDisappearEventPool;

extern std::set<iConnecter::fRouteEvent> g_RouteEventPool;

class NodeSession;
extern std::map<int, NodeSession*> g_NodeIdSessionMap;
extern std::map<std::string, std::set<NodeSession*>> g_NodeNameSessionMap;

extern std::string g_NodeName;
extern int g_NodeID;

#pragma pack(push, 1)
struct MessageHeader {
    unsigned_int16 _Len;
    unsigned_int16 _MsgId;
};
#pragma pack(pop)
#endif //__Header_h__
