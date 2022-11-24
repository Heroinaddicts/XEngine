#include "TestProfile.h"

iEngine* g_Engine = nullptr;
TestProfile* g_TestProfile = nullptr;

std::map<std::string, MillionMapTest> g_MillionMapTest;

bool TestProfile::Initialize(iEngine* const engine) {
    g_TestProfile = this;
    g_Engine = engine;
    return true;
}

#define MILLION 1000000

bool TestProfile::Launch(iEngine* const engine) {
    unsigned_int64 tick = SafeSystem::Time::GetMilliSecond();
    for (int i = 0; i < MILLION; i++) {
        MillionMapTest test;
        test._GUID = i;
        test._GateNodeID = 100;

        g_MillionMapTest.insert(std::make_pair(SafeString::Int64ToString(i), test));
    }
    TRACE(g_Engine, "MillionMapTest insert million data use tick %lld", SafeSystem::Time::GetMilliSecond() - tick);

    unsigned_int64 tick2 = SafeSystem::Time::GetMilliSecond();
    for (int i = 0; i < MILLION; i++) {
        auto itor = g_MillionMapTest.find(SafeString::Int64ToString(SafeTools::Rand64(MILLION)));
        if (itor != g_MillionMapTest.end()) {

        }
    }
    TRACE(g_Engine, "MillionMapTest query million data use tick %lld", SafeSystem::Time::GetMilliSecond() - tick2);

    return true;
}

bool TestProfile::Destroy(iEngine* const engine) {
    return true;
}
