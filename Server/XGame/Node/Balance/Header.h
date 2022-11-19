#ifndef __Header_h__
#define __Header_h__

#include "iTimerApi.h"

#include "iBalance.h"
#include "iConnecter.h"
#include "NodeDefine.h"

#include "SafeSystem.h"
#include "SafeMemory.h"

#include <vector>

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_Engine;
class Balance;
extern Balance* g_Balance;
extern iConnecter* g_Connecter;

struct GateInfo {
    iNodeSession* const _Session;
    bool _IsFull;

    GateInfo(iNodeSession* session) : _Session(session), _IsFull(false) {}
    GateInfo(const GateInfo& info) : _Session(info._Session), _IsFull(info._IsFull) {}

    GateInfo& operator=(const GateInfo& info) {
        SafeMemory::Memcpy(this, sizeof(GateInfo), &info, sizeof(info));
        return *this;
    }
};

extern int g_SelectIndexGateList;
extern std::vector<GateInfo> g_GateList;

namespace TimerConfig {
    enum eID {
        DelayClose  //超时 未收到客户端确认消息 主动close
    };

    const int DelayCloseTimeLen = SafeSystem::Time::Second * 2;
}

#endif //__Header_h__
