#include "TestPhysics.h"
#include "tinyxml2.h"
#include "SafeSystem.h"
#include "X3DObj.h"
#include "SafeSystem.h"

iEngine* g_engine = nullptr;
iObjLoader* g_objloader = nullptr;

class PhysicsTest : public Api::iPhysxContext {
public:
    PhysicsTest() : Api::iPhysxContext(nullptr) {}

    // 通过 iPhysxContext 继承
    virtual void OnPhysxCreated(bool success) override {
        if (success) {
            SetLayer(0);
            SafeTools::Rand(100) > 50 ? SetTrigger(true) : SetTrigger(false);
            SetCCD(true);
        }
        else {
            xdel this;
        }
    }

    virtual void OnPhysxAwake() override {
    }

    virtual void OnPhysxRelease() override {
        xdel this;
    }

    virtual void OnTriggerEnter(iCollider* const other) override {
        //TRACE(g_engine, "PhysicsTest::OnTriggerEnter %lld", this);
    }

    virtual void OnTriggerExit(iCollider* const other) override {
        //TRACE(g_engine, "PhysicsTest::OnTriggerExit %lld", this);
    }

    virtual void OnCollisionEnter(iCollision* const other) override {
        //TRACE(g_engine, "PhysicsTest::OnCollisionEnter %lld", this);
    }

    virtual void OnCollisionExit(iCollision* const other) override {
        //TRACE(g_engine, "PhysicsTest::OnCollisionExit %lld", this);
    }
};

class PhysicsMeshStatic : public Api::iPhysxContext {
public:
    PhysicsMeshStatic() : Api::iPhysxContext(nullptr) {}

    // 通过 iPhysxContext 继承
    virtual void OnPhysxCreated(bool success) override {
        if (success) {
            SetLayer(1);
            SetTrigger(false);
            SetCCD(true);
        }
        else {
            xdel this;
        }
    }

    virtual void OnPhysxAwake() override {
    }

    virtual void OnPhysxRelease() override {
        xdel this;
    }

    virtual void OnTriggerEnter(iCollider* const other) override {
        //TRACE(g_engine, "PhysicsMeshStatic::OnTriggerEnter %lld", this);
    }

    virtual void OnTriggerExit(iCollider* const other) override {
        //TRACE(g_engine, "PhysicsMeshStatic::OnTriggerExit %lld", this);
    }

    virtual void OnCollisionEnter(iCollision* const other) override {
        //TRACE(g_engine, "PhysicsMeshStatic::OnCollisionEnter %lld", this);
    }

    virtual void OnCollisionExit(iCollision* const other) override {
        //TRACE(g_engine, "PhysicsMeshStatic::OnCollisionExit %lld", this);
    }
};

bool TestPhysics::Initialize(iEngine* const engine) {
    g_engine = engine;
    return true;
}

bool TestPhysics::Launch(iEngine* const engine) {
    TRACE(engine, "Main thread id %lld", SafeSystem::Process::GetCurrentThreadID());

    g_objloader = engine->GetComponent<iObjLoader>("ObjLoader");
    XASSERT(g_objloader, "GetComponent Error");

    iPhysxScene* scene = engine->GetPhysicsApi()->CreateScene(0.5f, 0.5f, 0.3f);
    //scene->CreatePlane(0, 1, 0, 0, nullptr);
    int64 tick = SafeSystem::Time::GetMilliSecond();

    START_TIMER(engine, this, 1, SafeTools::Rand(500), Api::Unlimited, 17, scene);

    tinyxml2::XMLDocument doc;
    if (tinyxml2::XMLError::XML_SUCCESS != doc.LoadFile((SafeSystem::File::GetApplicationPath() + "/Env/Config/Objs/TestPhysx/SceneObjs.xml").c_str())) {
        XASSERT(false, "load xml error");
        return false;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    tinyxml2::XMLElement* sceneObj = root->FirstChildElement("SceneObj");
    while (nullptr != sceneObj) {
        float x = SafeString::StringToFloat(sceneObj->Attribute("x"));
        float y = SafeString::StringToFloat(sceneObj->Attribute("y"));
        float z = SafeString::StringToFloat(sceneObj->Attribute("z"));

        float scalex = SafeString::StringToFloat(sceneObj->Attribute("ScaleX"));
        float scaley = SafeString::StringToFloat(sceneObj->Attribute("ScaleY"));
        float scalez = SafeString::StringToFloat(sceneObj->Attribute("ScaleZ"));

        float qx = SafeString::StringToFloat(sceneObj->Attribute("QuaternionX"));
        float qy = SafeString::StringToFloat(sceneObj->Attribute("QuaternionY"));
        float qz = SafeString::StringToFloat(sceneObj->Attribute("QuaternionZ"));
        float qw = SafeString::StringToFloat(sceneObj->Attribute("QuaternionW"));

        std::string file = SafeSystem::File::GetApplicationPath() + "/Env/Config/Objs/TestPhysx/" + sceneObj->Attribute("SceneObj");
        const X3DObj* obj = g_objloader->Get3DObj(file);
        scene->CreateTriangleMesh(XEngine::eRigType::Static, XEngine::Vector3(x, y, z), Quaternion(qx, qy, qz, qw), Vector3(scalex, scaley, scalez), obj, xnew PhysicsMeshStatic());
        sceneObj = sceneObj->NextSiblingElement("SceneObj");
    }

    START_TIMER(engine, this, 4, 5000, 10000, 50, scene);
    START_TIMER(engine, this, 5, 5000, 10000, 50, scene);
    START_TIMER(engine, this, 6, 5000, 10000, 50, scene);
    scene->Simulate(1 / 60.0f);
    scene->RelationPhysicsLayer(0, 1);
    scene->RelationPhysicsLayer(0, 0);
    return true;
}

bool TestPhysics::Destroy(iEngine* const engine) {
    return true;
}

void TestPhysics::OnStart(const int id, void* const context, const int64 timestamp) {

}

void TestPhysics::OnTimer(const int id, void* const context, const int64 timestamp) {
    if (id >= 4) {
        iPhysxScene* scene = static_cast<iPhysxScene*>(context);

        int index = SafeTools::Rand(100000000) % 2;
        Quaternion qt;
        PhysicsTest* test = xnew PhysicsTest();
        if (index == 0) {
            scene->CreateBox(eRigType::Dynamic, Vector3(SafeTools::Rand(50) - 25, 30, SafeTools::Rand(50) - 25), qt, Vector3(2, 2, 1), test);
        }
        else {
            scene->CreateCapsule(eRigType::Dynamic, Vector3(SafeTools::Rand(50) - 25, 30, SafeTools::Rand(50) - 25), qt, 1, 2, test);
        }

        START_TIMER(g_engine, this, 2, (SafeTools::Rand(5) + 5) * 1000, 1, 1000, test);
    }
    else if (2 == id) {
        PhysicsTest* test = static_cast<PhysicsTest*>(context);
        test->ReleasePhysics();
    }
    else if (1 == id) {
        iPhysxScene* scene = static_cast<iPhysxScene*>(context);
        scene->FetchResults(true);
        scene->Simulate(1 / 60.0f);
    }
}

void TestPhysics::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {

}

void TestPhysics::OnPause(const int id, void* const context, const int64 timestamp) {

}

void TestPhysics::OnResume(const int id, void* const context, const int64 timestamp) {

}

