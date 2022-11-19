#include "Lobby.h"

iEngine * g_Engine = nullptr;
Lobby* g_Lobby = nullptr;

bool Lobby::Initialize(iEngine* const engine) {
    g_Lobby = this;
    g_Engine = engine;
    return true;
}

bool Lobby::Launch(iEngine* const engine) {
    return true;
}

bool Lobby::Destroy(iEngine* const engine) {
    return true;
}
