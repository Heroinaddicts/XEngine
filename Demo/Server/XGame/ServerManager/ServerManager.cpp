#include "ServerManager.h"

Api::iEngine* g_Engine = nullptr;
static ServerManager* s_Self = nullptr;
static UInt16 s_ServerID;
static const int MAX_COUNTER = 1023; // 10 bits

static UInt64 s_LastTimestamp = SafeSystem::Time::GetMilliSecond();
static int s_Offset = 0;

bool ServerManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool ServerManager::Launch(Api::iEngine* const engine) {
    s_ServerID = engine->GetLaunchParameterUInt16("ServerID");
    return true;
}

bool ServerManager::Destroy(Api::iEngine* const engine) {
    return true;
}

UInt16 ServerManager::ServerID() const {
    return s_ServerID;
}

UInt64 ServerManager::GeneraterGUID() const {
    UInt64 timestamp = SafeSystem::Time::GetMilliSecond();
    if (timestamp == s_LastTimestamp) {
        s_Offset++;
        if (s_Offset > MAX_COUNTER) {
            do {
                timestamp = SafeSystem::Time::GetMilliSecond();
            } while (timestamp <= s_LastTimestamp);
            s_Offset = 0;
            s_LastTimestamp = timestamp;
        }
    }
    else {
        s_Offset = 0;
        s_LastTimestamp = timestamp;
    }

    UInt64 guid = 0;
    guid |= ((UInt64)(s_ServerID & 0x3FF)) << 54;
    guid |= ((UInt64)(timestamp & 0xFFFFFFFFFFF)) << 10;
    guid |= (UInt64)(s_Offset & 0x3FF);
    return guid;
}
