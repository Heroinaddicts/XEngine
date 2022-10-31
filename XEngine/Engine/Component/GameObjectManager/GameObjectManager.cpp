#include "GameObjectManager.h"
#include "GameObject.h"

namespace XEngine {
    std::set<GameObject*> g_GameObjectSet;
    std::list<GameObject*> g_GameObjectCreated;
    std::list<GameObject*> g_GameObjectReleased;

    bool GameObjectManager::Initialize(Api::iEngine* const engine) {
        return true;
    }

    bool GameObjectManager::Launch(Api::iEngine* const engine) {
        return true;
    }

    void GameObjectManager::Release(Api::iEngine* const engine) {

    }

    void GameObjectManager::EarlyUpdate(Api::iEngine* const engine) {

    }

    void GameObjectManager::Update(Api::iEngine* const engine) {
        for (auto i = g_GameObjectSet.begin(); i != g_GameObjectSet.end(); i++) {
            (*i)->OnUpdate();
        }

        for (auto i = g_GameObjectCreated.begin(); i != g_GameObjectCreated.end(); i++) {
            g_GameObjectSet.insert(*i);
        }
        g_GameObjectCreated.clear();

        for (auto i = g_GameObjectReleased.begin(); i != g_GameObjectReleased.end(); i++) {
            g_GameObjectSet.erase(*i);
        }
        g_GameObjectReleased.clear();
    }

    void GameObjectManager::FixedUpdate(Api::iEngine* const engine) {
        for (auto i = g_GameObjectSet.begin(); i != g_GameObjectSet.end(); i++) {
            (*i)->OnFixedUpdate();
        }
    }

    void GameObjectManager::LaterUpdate(Api::iEngine* const engine) {
    }

    Api::iGameObject* GameObjectManager::CreateGameObject(const char* file, const int line) {
        GameObject* gameObject = xnew GameObject;
        SafeMemory::Memcpy((void*)&(gameObject->_File), sizeof(gameObject->_File), &file, sizeof(file));
        SafeMemory::Memcpy((void*)&(gameObject->_Line), sizeof(gameObject->_Line), &line, sizeof(line));

        g_GameObjectCreated.push_back(gameObject);
        return gameObject;
    }

    void GameObjectManager::DestroyGameObject(Api::iGameObject* gameObject) {

    }

}