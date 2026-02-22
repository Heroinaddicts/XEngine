#include "Combat.h"

Api::iEngine* g_Engine = nullptr;
static Combat* s_Self = nullptr;

bool Combat::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool Combat::Launch(Api::iEngine* const engine) {
    return true;
}

bool Combat::Destroy(Api::iEngine* const engine) {
    return true;
}
