#include "Monitor.h"

iEngine * g_Engine = nullptr;
Monitor* g_Monitor = nullptr;

bool Monitor::Initialize(iEngine* const engine) {
    g_Monitor = this;
    g_Engine = engine;
    return true;
}

bool Monitor::Launch(iEngine* const engine) {
    return true;
}

bool Monitor::Destroy(iEngine* const engine) {
    return true;
}
