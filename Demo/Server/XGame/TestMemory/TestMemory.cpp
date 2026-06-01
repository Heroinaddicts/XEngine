#include "TestMemory.h"

Api::iEngine* g_Engine = nullptr;
static TestMemory* s_Self = nullptr;

bool TestMemory::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    return true;
}

bool TestMemory::Launch(Api::iEngine* const engine) {
    Api::iMemoryApi* mapi = engine->GetMemoryApi();

    _MMap = mapi->CreateMMap("E:/Game/World of Warcraft/Data/data/data.003", Api::eAccess::ReadOnly);

    return _MMap != nullptr;
}

bool TestMemory::Destroy(Api::iEngine* const engine) {
    if (_MMap) {
        engine->GetMemoryApi()->Close(_MMap);
        _MMap = nullptr;
    }

    return true;
}
