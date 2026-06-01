#include "TestMemory.h"
#include "SafeSystem.h"

#define SHARE_MEMORY_NAME "HelloShareMemory"
#define SHARE_MEMORY_MAP_NAME "HelloMap"

Api::iEngine* g_Engine = nullptr;

bool TestMemory::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    return true;
}

bool is = false;

bool TestMemory::Launch(Api::iEngine* const engine) {
    _Map = engine->GetMemoryApi()->LoadShareMemoryMap<UInt64, UInt64>(SHARE_MEMORY_NAME, SHARE_MEMORY_MAP_NAME);
    if (!_Map) {
        _Map = engine->GetMemoryApi()->OpenOrCreateShareMemoryMap<UInt64, UInt64>(SHARE_MEMORY_NAME, 1024 * 1024, SHARE_MEMORY_MAP_NAME);
        DEBUG(g_Engine, "OpenOrCreate ShareMemoryMap %s/%s", SHARE_MEMORY_NAME, SHARE_MEMORY_MAP_NAME);
        is = true;
    }
    else {
        DEBUG(g_Engine, "Load ShareMemoryMap %s/%s Size %llu", _Map.ShareMemoryName().c_str(), _Map.Name().c_str(), _Map.Size());
    }

    if (!_Map) {
        return false;
    }

    UInt64 value = 0;
    if (_Map.TryGetValue(1, value)) {
        ++value;
    }
    else {
        value = 1;
    }

    _Map.Insert(1, value, true);
    DEBUG(g_Engine, "ShareMemoryMap Counter = %llu", value);
    return true;
}

bool TestMemory::Destroy(Api::iEngine* const engine) {
    engine->GetMemoryApi()->ReleaseShareMemoryMap(_Map);
    return true;
}

static UInt64 s_LastTick = SafeSystem::Time::GetMilliSecond();
void TestMemory::Update(Api::iEngine* const engine) {
    if (!_Map) {
        return;
    }

    if (SafeSystem::Time::GetMilliSecond() - s_LastTick >= 1000) {
        auto it = _Map.Find(1);
        if (it != _Map.End()) {
            if (is) {
                it->second = s_LastTick;
            }
            else {
                DEBUG(g_Engine, "Hello Share Memory Map Value %llu", it->second);
            }
        }


        s_LastTick = SafeSystem::Time::GetMilliSecond();
    }

}
