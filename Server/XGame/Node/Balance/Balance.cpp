#include "Balance.h"

iEngine * g_Engine = nullptr;
Balance* g_Balance = nullptr;

bool Balance::Initialize(iEngine* const engine) {
    g_Balance = this;
    g_Engine = engine;
    return true;
}

bool Balance::Launch(iEngine* const engine) {
    return true;
}

bool Balance::Destroy(iEngine* const engine) {
    return true;
}
