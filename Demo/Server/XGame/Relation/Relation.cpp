#include "Relation.h"

Api::iEngine* g_Engine = nullptr;
static Relation* s_Self = nullptr;

bool Relation::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool Relation::Launch(Api::iEngine* const engine) {
    return true;
}

bool Relation::Destroy(Api::iEngine* const engine) {
    return true;
}
