#include "Master.h"

iEngine * g_engine = nullptr;
Master* g_Master = nullptr;

bool Master::Initialize(iEngine* const engine) {
    g_Master = this;
    g_engine = engine;
    return true;
}

bool Master::Launch(iEngine* const engine) {
    return true;
}

bool Master::Destroy(iEngine* const engine) {
    return true;
}
