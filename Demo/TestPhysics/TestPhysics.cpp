#include "TestPhysics.h"
#include "tinyxml2.h"
#include "SafeFile.h"

iEngine* g_engine = nullptr;

bool TestPhysics::Initialize(iEngine* const engine) {
    g_engine = engine;
    return true;
}


bool TestPhysics::Launch(iEngine* const engine) {
    iPhysxScene* scene = engine->GetPhysicsApi()->CreateScene(0.5f, 0.5f, 0.6f);
    scene->CreatePlane(0, 1, 0, 0, nullptr);
    int64 tick = SafeSystem::GetMilliSecond();

    START_TIMER(engine, this, 0, 20 + SafeTools::Rand(500), 100, 50, scene);
    START_TIMER(engine, this, 1, SafeTools::Rand(500), Api::Unlimited, 17, scene);

    tinyxml2::XMLDocument doc;
    if (tinyxml2::XMLError::XML_SUCCESS != doc.LoadFile((SafeFile::GetApplicationPath() + "/TestPhysx/SceneInfo.xml").c_str())) {
        XASSERT(false, "load xml error");
        return false;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    tinyxml2::XMLElement* sceneObj = root->FirstChildElement("SceneObj");
    while (nullptr != sceneObj) {
        float x = SafeString::StringToFloat(sceneObj->Attribute("PositionX"));
        float y = SafeString::StringToFloat(sceneObj->Attribute("PositionY"));
        float z = SafeString::StringToFloat(sceneObj->Attribute("PositionZ"));

        float qx = SafeString::StringToFloat(sceneObj->Attribute("QuaternionX"));
        float qy = SafeString::StringToFloat(sceneObj->Attribute("QuaternionY"));
        float qz = SafeString::StringToFloat(sceneObj->Attribute("QuaternionZ"));
        float qw = SafeString::StringToFloat(sceneObj->Attribute("QuaternionW"));

        std::string file = SafeFile::GetApplicationPath() + "/TestPhysx/" + sceneObj->Attribute("SceneObj");

        scene->CreateTriangleMesh(XEngine::Api::eRigType::Static, XEngine::Vector3(x, y, z), Quaternion(qx, qy, qz, qw), 1, file);
        printf("Load %s, %.2f, %.2f, %.2f\n", file.c_str(), x, y, z);
        sceneObj = sceneObj->NextSiblingElement("SceneObj");
    }

    scene->Simulate(1 / 60.0f);

    return true;
}

bool TestPhysics::Destroy(iEngine* const engine) {
    return true;
}

void TestPhysics::OnStart(const int id, void* const context, const int64 timestamp) {

}

void TestPhysics::OnTimer(const int id, void* const context, const int64 timestamp) {
    if (0 == id) {
        iPhysxScene* scene = static_cast<iPhysxScene*>(context);

        int index = SafeTools::Rand(100000000) % 2;
        Quaternion qt;
        if (index == 0) {
            scene->CreateBox(eRigType::Dynamic, Vector3(0, 100, 0), qt, Vector3(0.2, 0.2, 0.1));
        }
        else {
            scene->CreateCapsule(eRigType::Dynamic, Vector3(0, 100, 0), qt, 0.1, 0.2);
        }
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

