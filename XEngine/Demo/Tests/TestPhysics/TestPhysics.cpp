#include "TestPhysics.h"
#include "tinyxml2.h"
#include "SafeSystem.h"
#include "X3DObj.h"
#include "SafeSystem.h"

iEngine* g_engine = nullptr;
iObjLoader* g_objloader = nullptr;

TestPhysics* s_Self = nullptr;

static int s_PhysxObjectCount = 0;

bool TestPhysics::Initialize(iEngine* const engine) {
    g_engine = engine;
    s_Self = this;
    return true;
}

bool TestPhysics::Launch(iEngine* const engine) {
    TRACE(engine, "Main thread id %lld", SafeSystem::Process::GetCurrentThreadID());

    g_objloader = engine->GetModule<iObjLoader>("ObjLoader");
    XASSERT(g_objloader, "GetModule Error");

    iPhysxScene* scene = engine->GetPhysicsApi()->CreateScene(0.5f, 0.5f, 0.3f);
    //scene->CreatePlane(0, 1, 0, 0, nullptr);
    int64 tick = SafeSystem::Time::GetMilliSecond();


    tinyxml2::XMLDocument doc;
    if (tinyxml2::XMLError::XML_SUCCESS != doc.LoadFile((SafeSystem::File::GetApplicationPath() + "/Env/Config/Objs/LostCity/SceneObjs.xml").c_str())) {
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

        std::string file = SafeSystem::File::GetApplicationPath() + "/Env/Config/Objs/LostCity/" + sceneObj->Attribute("SceneObj");
        const X3DObj* obj = g_objloader->Get3DObj(file);
        //scene->CreateTriangleMesh(XEngine::eRigType::Static, XEngine::Vector3(x, y, z), Quaternion(qx, qy, qz, qw), Vector3(scalex, scaley, scalez), obj, xnew PhysicsMeshStatic());
        sceneObj = sceneObj->NextSiblingElement("SceneObj");
    }

    START_TIMER(engine, this, 4, 5000, Api::Unlimited, 10, scene);
    START_TIMER(engine, this, 5, 5000, Api::Unlimited, 10, scene);
    START_TIMER(engine, this, 6, 5000, Api::Unlimited, 10, scene);
    START_TIMER(engine, this, 7, 5000, Api::Unlimited, 5000, scene);
    scene->Simulate(1 / 60.0f);
    START_TIMER(engine, this, 1, SafeTools::Rand(500), Api::Unlimited, 16, scene);
    //     scene->RelationPhysicsLayer(0, 1);
    //     scene->RelationPhysicsLayer(0, 0);
    return true;
}

bool TestPhysics::Destroy(iEngine* const engine) {
    return true;
}

void TestPhysics::OnStart(const int id, void* const context, const int64 timestamp) {

}

void TestPhysics::OnTimer(const int id, void* const context, const int64 timestamp) {

}

void TestPhysics::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {

}

void TestPhysics::OnPause(const int id, void* const context, const int64 timestamp) {

}

void TestPhysics::OnResume(const int id, void* const context, const int64 timestamp) {

}

