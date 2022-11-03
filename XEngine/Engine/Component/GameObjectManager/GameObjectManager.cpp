#include "GameObjectManager.h"
#include "GameObject.h"
#include "Engine.h"

namespace XEngine {
    std::set<GameObject*> g_GameObjectSet;
    std::list<GameObject*> g_GameObjectCreated;
    std::set<GameObject*> g_GameObjectReleased;

    GameObjectManager* GameObjectManager::GetInstance() {
        static GameObjectManager s_Instance;
        return &s_Instance;
    }

    bool GameObjectManager::Initialize(Api::iEngine* const engine) {
        return true;
    }

    bool GameObjectManager::Launch(Api::iEngine* const engine) {
        return true;
    }

    void GameObjectManager::Release(Api::iEngine* const engine) {

    }

    static bool s_IsRecovering = false;
    __forceinline void Recover() {
        s_IsRecovering = true;
        for (auto i = g_GameObjectReleased.begin(); i != g_GameObjectReleased.end(); i++) {
            (*i)->OnDestroy();
            g_GameObjectSet.erase(*i);
            xdel(*i);
        }
        g_GameObjectReleased.clear();
        s_IsRecovering = false;
    }

    void GameObjectManager::EarlyUpdate(Api::iEngine* const engine) {
        for (auto i = g_GameObjectCreated.begin(); i != g_GameObjectCreated.end(); i++) {
            g_GameObjectSet.insert(*i);
            (*i)->OnCreate();
        }
        g_GameObjectCreated.clear();
        Recover();
    }

    void GameObjectManager::Update(Api::iEngine* const engine) {
        for (auto i = g_GameObjectSet.begin(); i != g_GameObjectSet.end(); i++) {
            (*i)->OnUpdate();
        }
        Recover();
    }

    void GameObjectManager::FixedUpdate(Api::iEngine* const engine) {
        for (auto i = g_GameObjectSet.begin(); i != g_GameObjectSet.end(); i++) {
            (*i)->OnFixedUpdate();
        }
        Recover();
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
        if (!s_IsRecovering) {
            g_GameObjectReleased.insert(dynamic_cast<GameObject*>(gameObject));
        }
        else {
            XERROR(g_Engine, "do not destroy an object in OnDestroy");
        }
    }
}
