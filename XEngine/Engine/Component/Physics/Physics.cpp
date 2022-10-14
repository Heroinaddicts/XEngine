#include "Physics.h"
#include "PhysxScene.h"

namespace XEngine {
    PhysicsAllocator g_allocator;
    PhysicsErrorCallback g_error_callback;

    PxFoundation* g_pxfoundation = nullptr;
    PxPhysics* g_pxphysics = nullptr;
    PxCooking* g_cooking = nullptr;
    PxDefaultCpuDispatcher* g_pxdispatcher = nullptr;
    PxPvd* g_pxpvd = nullptr;
    PxCudaContextManager* g_cuda_context_manager = nullptr;

    iPhysics* Physics::GetInstance() {
        static Physics static_physics;
        return &static_physics;
    }

    bool Physics::Initialize(Api::iEngine* const engine) {
        g_pxfoundation = PxCreateFoundation(PX_PHYSICS_VERSION, g_allocator, g_error_callback);
        XASSERT(g_pxfoundation, "create foundation error");

        const char* pvd_host = engine->GetLaunchParameter("pvd_host");
        const char* pvd_port = engine->GetLaunchParameter("pvd_port");
        if (pvd_host && pvd_port) {
            g_pxpvd = PxCreatePvd(*g_pxfoundation);
            PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(pvd_host, SafeString::StringToInt(pvd_port), 10);
            g_pxpvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
        }
        PxTolerancesScale scale;
        scale.length = 1;        // typical length of an object
        scale.speed = 9.81;         // typical speed of an object, gravity*1s is a reasonable choice

        g_pxphysics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_pxfoundation, scale, true, g_pxpvd);
        XASSERT(g_pxphysics, "create physics error");
        g_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *g_pxfoundation, PxCookingParams(PxTolerancesScale()));
        PxCudaContextManagerDesc ccm_desc;
        g_cuda_context_manager = PxCreateCudaContextManager(*g_pxfoundation, ccm_desc, PxGetProfilerCallback());
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

    PxFilterFlags _PxSimulationFilterShader(
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize) {

        return PxFilterFlag::eNOTIFY;
    }

    Api::iPhysxScene* Physics::CreateScene(const float static_friction, const float dynamic_friction, const float restitution) {
        PxSceneDesc desc(g_pxphysics->getTolerancesScale());
        desc.filterShader = _PxSimulationFilterShader;
        desc.gravity = PxVec3(0.0f, -98.1f, 0.0f);
        desc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
        desc.filterShader = PxDefaultSimulationFilterShader;
        desc.cudaContextManager = g_cuda_context_manager;
        //desc.flags | PxSceneFlag::eENABLE_CCD;
        desc.flags | PxSceneFlag::eENABLE_GPU_DYNAMICS | PxSceneFlag::eENABLE_ACTIVE_ACTORS | PxSceneFlag::eENABLE_CCD;
        desc.broadPhaseType = PxBroadPhaseType::eGPU;

        PxScene* px_scene = g_pxphysics->createScene(desc);
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

        XERROR(Engine::GetInstance(), "create physx scene error, name %s");
        return nullptr;
    }

    void Physics::ReleaseScene(Api::iPhysxScene* scene) {

    }
}
