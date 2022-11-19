#include "GameServer.h"

iEngine * g_Engine = nullptr;
GameServer* g_GameServer = nullptr;

bool GameServer::Initialize(iEngine* const engine) {
    g_GameServer = this;
    g_Engine = engine;
    return true;
}

bool GameServer::Launch(iEngine* const engine) {
    return true;
}

bool GameServer::Destroy(iEngine* const engine) {
    return true;
}
