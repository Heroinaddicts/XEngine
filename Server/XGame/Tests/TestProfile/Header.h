#ifndef __Header_h__
#define __Header_h__

#include "iTestProfile.h"

#include "SafeString.h"
#include "SafeSystem.h"
#include "SafeTools.h"
#include <map>
#include <string>

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_Engine;
class TestProfile;
extern TestProfile* g_TestProfile;

struct MillionMapTest {
    unsigned_int64 _GUID;
    int _GateNodeID;
};

extern std::map<std::string, MillionMapTest> g_MillionMapTest;

#endif //__Header_h__
