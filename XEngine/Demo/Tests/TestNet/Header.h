#ifndef __Header_h__
#define __Header_h__

#include "iTestNet.h"

#include "iNetApi.h"
#include "iTimerApi.h"

#include "SafeSystem.h"
#include "SafeString.h"
#include "SafeTools.h"

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_Engine;
class TestNet;
extern TestNet* g_TestNet;

namespace TimeConfig {
    enum eTimeID {
        TraceBandwidth,
        RandomSendData,
        RandomClose,

        NewClient
    };
}

extern int g_ClientCount;

#endif //__Header_h__
