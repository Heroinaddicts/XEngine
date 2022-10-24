#include "Hello.h"

iEngine * g_engine = nullptr;

bool Hello::Initialize(iEngine* const engine) {
    g_engine = engine;
    return true;
}

bool Hello::Launch(iEngine* const engine) {
    return true;
}

bool Hello::Destroy(iEngine* const engine) {
    return true;
}
