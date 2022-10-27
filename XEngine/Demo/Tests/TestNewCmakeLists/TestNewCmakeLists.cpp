#include "TestNewCmakeLists.h"

iEngine * g_engine = nullptr;

bool TestNewCmakeLists::Initialize(iEngine* const engine) {
    g_engine = engine;
    return true;
}

bool TestNewCmakeLists::Launch(iEngine* const engine) {
    return true;
}

bool TestNewCmakeLists::Destroy(iEngine* const engine) {
    return true;
}
