#include "GameObjectTest.h"

iEngine* g_engine = nullptr;
iGameObjectApi* g_GameObjectApi = nullptr;
GameObjectTest* g_GameObjectTest = nullptr;

class ComponentTest : public Api::iComponent {
public:
    virtual ~ComponentTest() {}

    // Í¨¹ý iComponent ¼Ì³Ð
    virtual void OnCreate() override {
        //TRACE(g_engine, "ComponentTest::OnCreate");
    }

	virtual void OnStart() override {
		//TRACE(g_engine, "ComponentTest::OnStart");
    }

	virtual void OnUpdate() override {
		//TRACE(g_engine, "ComponentTest::OnUpdate");
		//g_GameObjectApi->DestroyGameObject(this->_GameObject);
    }

    virtual void OnFixedUpdate() override {
		//TRACE(g_engine, "ComponentTest::OnFixedUpdate");
    }

    virtual void OnDestroy() override {
		//TRACE(g_engine, "ComponentTest::OnDestroy");
    }
};

bool GameObjectTest::Initialize(iEngine* const engine) {
    g_GameObjectTest = this;
    g_engine = engine;

    g_GameObjectApi = engine->GetGameObjectApi();
    for (int i = 0; i < 50000; i++) {
		iGameObject* gameObject = CREATE_GAMEOBJECT(g_GameObjectApi);
		ADD_COMPONENT(gameObject, ComponentTest);
    }
    return true;
}

bool GameObjectTest::Launch(iEngine* const engine) {
    return true;
}

bool GameObjectTest::Destroy(iEngine* const engine) {
    return true;
}
