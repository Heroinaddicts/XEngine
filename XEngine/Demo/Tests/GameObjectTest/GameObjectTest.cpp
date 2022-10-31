#include "GameObjectTest.h"

iEngine* g_engine = nullptr;
GameObjectTest* g_GameObjectTest = nullptr;

class ComponentTest : public Api::iComponent {
public:
    virtual ~ComponentTest() {}

    // Í¨¹ý iComponent ¼Ì³Ð
    virtual void OnCreate() override {

    }

    virtual void OnStart() override {

    }

    virtual void OnUpdate() override {

    }

    virtual void OnFixedUpdate() override {

    }

    virtual void OnDestroy() override {

    }
};

bool GameObjectTest::Initialize(iEngine* const engine) {
    g_GameObjectTest = this;
    g_engine = engine;

    iGameObjectApi* gameObjectApi = engine->GetGameObjectApi();

    iGameObject* gameObject = CREATE_GAMEOBJECT(gameObjectApi);
    ADD_COMPONENT(gameObject, ComponentTest);
    return true;
}

bool GameObjectTest::Launch(iEngine* const engine) {
    return true;
}

bool GameObjectTest::Destroy(iEngine* const engine) {
    return true;
}
