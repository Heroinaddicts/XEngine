#include "Location.h"

iEngine * g_Engine = nullptr;
Location* g_Location = nullptr;

bool Location::Initialize(iEngine* const engine) {
    g_Location = this;
    g_Engine = engine;
    return true;
}

bool Location::Launch(iEngine* const engine) {
    return true;
}

bool Location::Destroy(iEngine* const engine) {
    return true;
}
