#include "TestPhysics.h"

iEngine* g_engine = nullptr;

bool TestPhysics::Initialize(iEngine* const engine) {
    g_engine = engine;
    return true;
}


bool TestPhysics::Launch(iEngine* const engine) {
    for (int i = 0; i < 20; i++) {
        iPhysxScene* scene = engine->GetPhysicsApi()->CreateScene(0.5f, 0.5f, 0.6f);
        scene->CreatePlane(0, 1, 0, 0, nullptr);
        START_TIMER(engine, this, 0, 20 + SafeTools::Rand(500), 50, 100, scene);
        START_TIMER(engine, this, 1, SafeTools::Rand(500), Api::Unlimited, 17, scene);
        scene->Simulate(1 / 60.0f);
    }

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
        if (index == 0) {
            scene->CreateBox(eRigType::Dynamic, Vector3(0, 500, 0), Vector3(SafeTools::Rand(10) + 1, SafeTools::Rand(10) + 1, SafeTools::Rand(10) + 1));
        }
        else {
            scene->CreateCapsule(eRigType::Dynamic, Vector3(0, 500, 0), SafeTools::Rand(3) + 3, SafeTools::Rand(10) + 10);
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

