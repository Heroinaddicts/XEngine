#include "HelloWorld.h"

iEngine* g_engine = nullptr;

bool HelloWorld::Initialize(iEngine* const engine) {
    g_engine = engine;
    return true;
}

bool HelloWorld::Launch(iEngine* const engine) {
    return true;
}

bool HelloWorld::Destroy(iEngine* const engine) {
    return true;
}
