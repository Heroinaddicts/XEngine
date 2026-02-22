#include "GameWorld.h"
#include "GameObject.h"
#include "Attribute.h"

std::unordered_map<eGameObjectType, AttributeCallback> g_AttributeCallback;

static XEventPool<IGameWorld::eGameWorldEvent, IGameObject* const> s_GameObjectEvent[(int)eGameObjectType::__AllType__];
static std::unordered_map<UInt64, IGameObject*> s_GameObjectMap;

Api::iEngine* g_Engine = nullptr;
bool GameWorld::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    _fGuidAlloca = []() -> UInt64 {
        static UInt64 s_Guid = 0;
        return s_Guid++;
        };

    s_GameObjectMap.reserve(10240);
    return true;
}


__forceinline void OnGameObjectCreate(IGameObject* const go) {
    DEBUG(g_Engine, "OnGameObjectCreate %lld Type %d, %llx", go->Guid(), (int)go->Type(), go);
}

__forceinline void OnGameObjectRelease(IGameObject* const go) {
    DEBUG(g_Engine, "OnGameObjectRelease %lld Type %d, %llx", go->Guid(), (int)go->Type(), go);
}

bool GameWorld::Launch(Api::iEngine* const engine) {
    return true;
}

bool GameWorld::Destroy(Api::iEngine* const engine) {
    return true;
}

void GameWorld::SetGuidAlloca(UInt64(*fun)()) {
    _fGuidAlloca = fun;
}

void GameWorld::RegistGameObjectEvent(const eGameObjectType type, const eGameWorldEvent ev, fGameObjectEvent callback) {
    XEventPoolRegist(s_GameObjectEvent[(int)type], ev, callback);
}

IGameObject* GameWorld::CreateGameObject(
    const eGameObjectType type,
    const int config,
    UInt64 guid,
    fInitializeGameObject initer,
    const char* file,
    const int line
) {
    if (guid == AUTO_GUID) {
        guid = _fGuidAlloca();
    }

    XASSERT(s_GameObjectMap.end() == s_GameObjectMap.find(guid), "Guid %lld already exists %s:%d", guid, file, line);
    if (s_GameObjectMap.end() != s_GameObjectMap.find(guid)) {
        ErrorLog(g_Engine, "Guid %lld already exists %s:%d", guid, file, line);
        return nullptr;
    }

    __CreateGameObject__;

    if (initer) {
        initer(go);
    }

    go->_Inited = true;

	s_GameObjectMap.insert({ guid, go });
    for (int i = go->_TypesCount - 1; i >= 0; i--) {
        s_GameObjectEvent[(int)go->_Types[i]].Trigger(eGameWorldEvent::Create, go);
    }

    TraceLog(g_Engine, "GameWorld::CreateGameObject %s %llu", go->_Name, go->_Guid);
    return go;
}

IGameObject* GameWorld::QueryGameObject(const UInt64 guid) const {
    auto itor = s_GameObjectMap.find(guid);
    if (s_GameObjectMap.end() == itor) {
        return nullptr;
    }
    return itor->second;
}

void GameWorld::ReleaseGameObject(IGameObject*& go, const char* file, const int line) {
    XASSERT(s_GameObjectMap.end() != s_GameObjectMap.find(go->Guid()), "Guid %lld is not exists %s:%d", go->Guid(), file, line);
    if (s_GameObjectMap.end() == s_GameObjectMap.find(go->Guid())) {
        ErrorLog(g_Engine, "Guid %lld is not exists %s:%d", go->Guid(), file, line);
        return;
    }

    GameObject* temp = static_cast<GameObject*>(go);
    for (int i = temp->_TypesCount - 1; i >= 0; i--) {
        s_GameObjectEvent[(int)temp->_Types[i]].Trigger(eGameWorldEvent::Release, go);
    }

    TraceLog(g_Engine, "GameWorld::ReleaseGameObject %s %llu", temp->_Name, temp->_Guid);
    s_GameObjectMap.erase(go->Guid());
    GameObject::Release((GameObject*&)go);
}

void GameWorld::RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fUInt32AttributeChangeJudge& judge) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._UInt32Judge.insert(judge);
}

void GameWorld::RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fInt32AttributeChangeJudge& judge) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._Int32Judge.insert(judge);
}

void GameWorld::RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fUInt64AttributeChangeJudge& judge) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._UInt64Judge.insert(judge);
}

void GameWorld::RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fInt64AttributeChangeJudge& judge) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._Int64Judge.insert(judge);
}

void GameWorld::RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fFloatAttributeChangeJudge& judge) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._FloatJudge.insert(judge);
}

void GameWorld::RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fBoolAttributeChangeJudge& judge) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._BoolJudge.insert(judge);
}

void GameWorld::RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fStringAttributeChangeJudge& judge) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._StringJudge.insert(judge);
}

void GameWorld::RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fVector2AttributeChangeJudge& judge) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._Vector2Judge.insert(judge);
}

void GameWorld::RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fVector3AttributeChangeJudge& judge) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._Vector3Judge.insert(judge);

}

void GameWorld::RegistGameObjectAttributeChanged(const eGameObjectType type, const fUInt32AttributeChanged& changed) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._UInt32Changed.insert(changed);
}

void GameWorld::RegistGameObjectAttributeChanged(const eGameObjectType type, const fInt32AttributeChanged& changed) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._Int32Changed.insert(changed);
}

void GameWorld::RegistGameObjectAttributeChanged(const eGameObjectType type, const fUInt64AttributeChanged& changed) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._UInt64Changed.insert(changed);
}

void GameWorld::RegistGameObjectAttributeChanged(const eGameObjectType type, const fInt64AttributeChanged& changed) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._Int64Changed.insert(changed);
}

void GameWorld::RegistGameObjectAttributeChanged(const eGameObjectType type, const fFloatAttributeChanged& changed) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._FloatChanged.insert(changed);
}

void GameWorld::RegistGameObjectAttributeChanged(const eGameObjectType type, const fStringAttributeChanged& changed) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._StringChanged.insert(changed);
}

void GameWorld::RegistGameObjectAttributeChanged(const eGameObjectType type, const fBoolAttributeChanged& changed) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._BoolChanged.insert(changed);
}

void GameWorld::RegistGameObjectAttributeChanged(const eGameObjectType type, const fVector2AttributeChanged& changed) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._Vector2Changed.insert(changed);
}

void GameWorld::RegistGameObjectAttributeChanged(const eGameObjectType type, const fVector3AttributeChanged& changed) {
    auto itor = g_AttributeCallback.find(type);
    if (g_AttributeCallback.end() == itor) {
        itor = g_AttributeCallback.insert({ type, AttributeCallback() }).first;
    }

    itor->second._Vector3Changed.insert(changed);
}
