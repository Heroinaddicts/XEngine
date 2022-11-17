#include "GameObjectTest.h"
#include "SafeTools.h"

iEngine* g_engine = nullptr;
iGameObjectApi* g_GameObjectApi = nullptr;
GameObjectTest* g_GameObjectTest = nullptr;

int g_CompoentTestCount = 0;

class ComponentTest : public Api::iComponent {
public:
    virtual ~ComponentTest() {}

    // Í¨¹ý iComponent ¼Ì³Ð
    virtual void OnCreate() override {
        TRACE(g_engine, "ComponentTest::OnCreate %d", ++g_CompoentTestCount);
    }

    virtual void OnUpdate() override {
        if (SafeTools::Rand(10000) >= 9950) {
            std::vector<ComponentTest*> tests;
            _GameObject->GetComponents<ComponentTest>(tests);

            for (int i = 0; i < tests.size(); i++) {
                _GameObject->RemoveComponent<ComponentTest>(tests[i]);
            }
        }
    }

    virtual void OnFixedUpdate() override {
        //TRACE(g_engine, "ComponentTest::OnFixedUpdate");
    }

    virtual void OnDestroy() override {
    }
};

class ComponentTest2 : public Api::iComponent {
public:
    virtual ~ComponentTest2() {}

    virtual void OnFixedUpdate() {
        if (SafeTools::Rand(10000) >= 9950) {
            g_GameObjectApi->DestroyGameObject(_GameObject);
        }
    }

    virtual void OnDestroy() override {
        TRACE(g_engine, "ComponentTest2::OnDestroy %d", --g_CompoentTestCount);
    }
};

bool GameObjectTest::Initialize(iEngine* const engine) {
    g_GameObjectTest = this;
    g_engine = engine;

    g_GameObjectApi = engine->GetGameObjectApi();
    for (int i = 0; i < 1000; i++) {
        iGameObject* gameObject = CREATE_GAMEOBJECT(g_GameObjectApi);
        gameObject->AddComponent<ComponentTest>();
        gameObject->AddComponent<ComponentTest2>();
    }
    return true;
}

bool GameObjectTest::Launch(iEngine* const engine) {
    return true;
}

bool GameObjectTest::Destroy(iEngine* const engine) {
    return true;
}
