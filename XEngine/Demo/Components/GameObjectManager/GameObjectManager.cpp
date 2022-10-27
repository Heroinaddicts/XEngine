#include "GameObjectManager.h"
#include "GameObject.h"

iEngine* g_engine = nullptr;

bool GameObjectManager::Initialize(iEngine* const engine) {
    g_engine = engine;

    return true;
}

bool GameObjectManager::Launch(iEngine* const engine) {
    return true;
}

bool GameObjectManager::Destroy(iEngine* const engine) {
    return true;
}
