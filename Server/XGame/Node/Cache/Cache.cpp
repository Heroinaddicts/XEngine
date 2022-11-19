#include "Cache.h"

iEngine * g_Engine = nullptr;
Cache* g_Cache = nullptr;

bool Cache::Initialize(iEngine* const engine) {
    g_Cache = this;
    g_Engine = engine;
    return true;
}

bool Cache::Launch(iEngine* const engine) {
    return true;
}

bool Cache::Destroy(iEngine* const engine) {
    return true;
}
