#include "TestMemory.h"
#include "SafeSystem.h"

#define SHARE_MEMORY_NAME "HelloShareMemory"

Api::iEngine* g_Engine = nullptr;

bool TestMemory::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    return true;
}

bool TestMemory::Launch(Api::iEngine* const engine) {
    Api::iShareMemory* sm = engine->GetMemoryApi()->LoadShareMemory(SHARE_MEMORY_NAME);
    if (nullptr == sm) {
        sm = engine->GetMemoryApi()->CreateShareMemory(SHARE_MEMORY_NAME, 1024);
        DEBUG(g_Engine, "Create ShareMemory %s Size 1024", SHARE_MEMORY_NAME);
    }
    else {
        DEBUG(g_Engine, "Load ShareMemory %s Size %llu", sm->Name().c_str(), sm->Size());
    }

    return true;
}

bool TestMemory::Destroy(Api::iEngine* const engine) {
    return true;
}
