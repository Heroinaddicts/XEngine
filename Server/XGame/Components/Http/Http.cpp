#include "Http.h"

iEngine * g_Engine = nullptr;
Http* g_Http = nullptr;

bool Http::Initialize(iEngine* const engine) {
    g_Http = this;
    g_Engine = engine;
    return true;
}

bool Http::Launch(iEngine* const engine) {
    return true;
}

bool Http::Destroy(iEngine* const engine) {
    return true;
}
