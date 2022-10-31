#include "Physics.h"
#include "PhysxScene.h"
#include "PhysxSceneFilter.h"

namespace XEngine {
    PhysicsAllocator g_Allocator;
    PhysicsErrorCallback g_ErrorCallback;

    PxFoundation* g_PxFoundation = nullptr;
    PxPhysics* g_PxPhysics = nullptr;
    PxCooking* g_Cooking = nullptr;
    PxDefaultCpuDispatcher* g_Pxdispatcher = nullptr;
    PxPvd* g_Pxpvd = nullptr;
    PxCudaContextManager* g_CudaContextManager = nullptr;

    iPhysics* Physics::GetInstance() {
        static Physics s_PhysicsInstance;
        return &s_PhysicsInstance;
    }

    bool Physics::Initialize(Api::iEngine* const engine) {
        g_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, g_Allocator, g_ErrorCallback);
        XASSERT(g_PxFoundation, "create foundation error");

        const char* pvd_host = engine->GetLaunchParameter("pvd_host");
        const char* pvd_port = engine->GetLaunchParameter("pvd_port");
        if (pvd_host && pvd_port) {
            g_Pxpvd = PxCreatePvd(*g_PxFoundation);
            PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(pvd_host, SafeString::StringToInt(pvd_port), 10);
            g_Pxpvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
        }
        PxTolerancesScale scale;
        scale.length = 1;        // typical length of an object
        scale.speed = 9.81;         // typical speed of an object, gravity*1s is a reasonable choice

        g_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_PxFoundation, scale, true, g_Pxpvd);
        XASSERT(g_PxPhysics, "create physics error");
        g_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *g_PxFoundation, PxCookingParams(PxTolerancesScale()));
        PxCudaContextManagerDesc ccm_desc;
        g_CudaContextManager = PxCreateCudaContextManager(*g_PxFoundation, ccm_desc, PxGetProfilerCallback());
        return true;
    }

    bool Physics::Launch(Api::iEngine* const engine) {
        return false;
    }

    void Physics::Release(Api::iEngine* const engine) {

    }

    void Physics::EarlyUpdate(Api::iEngine* const engine) {

    }

    void Physics::Update(Api::iEngine* const engine) {

    }

    void Physics::FixedUpdate(Api::iEngine* const engine) {

    }

    void Physics::LaterUpdate(Api::iEngine* const engine) {

    }

    Api::iPhysxScene* Physics::CreateScene(const float static_friction, const float dynamic_friction, const float restitution) {
        PxSceneDesc desc(g_PxPhysics->getTolerancesScale());
        desc.filterShader = PhysxScene::PhysxSimulationFilterShader;
        desc.filterCallback = PhysxSceneFilterCallback::GetInstance();

        desc.gravity = PxVec3(0.0f, -98.1f, 0.0f);
        desc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
        desc.cudaContextManager = g_CudaContextManager;
        desc.flags | PxSceneFlag::eENABLE_GPU_DYNAMICS | PxSceneFlag::eENABLE_ACTIVE_ACTORS | PxSceneFlag::eENABLE_CCD;
        desc.broadPhaseType = PxBroadPhaseType::eGPU;

        PxScene* px_scene = g_PxPhysics->createScene(desc);
        XASSERT(px_scene, "create physx scene error");
        px_scene->setCCDMaxPasses(0);
        if (px_scene) {
            PhysxScene* scene = xnew PhysxScene(px_scene, static_friction, dynamic_friction, restitution);

            PxPvdSceneClient* pvdClient = px_scene->getScenePvdClient();
            if (pvdClient) {
                pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
                pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
                pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
            }
            return scene;
        }

        XERROR(Engine::GetInstance(), "create physx scene error");
        return nullptr;
    }

    void Physics::ReleaseScene(Api::iPhysxScene* scene) {
        PhysxScene* p = dynamic_cast<PhysxScene*>(scene);
        XASSERT(p, "wtf");
        if (p) {
            p->_Scene->release();
            xdel scene;
        }
        else {
            XERROR(Engine::GetInstance(), "error iPhysxScene address %lld", scene);
        }
    }
}
