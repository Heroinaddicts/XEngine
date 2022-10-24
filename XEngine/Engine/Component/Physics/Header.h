#pragma once

#include <list>

#include "iPhysics.h"
#include "Engine.h"
#include "PxPhysicsAPI.h"
using namespace physx;

#include "SafeString.h"
#include "SafeMemory.h"
#include "SafeThread.h"

namespace XEngine {
    enum class eMeshColliderCookingOptions {
        CookingNone = 0,
        SimulationPerformance = 1 << 1,
        EnableCleaning = 1 << 2,
        EnableVertexWelding = 1 << 3,

        DefaultCooking = SimulationPerformance | EnableCleaning | EnableVertexWelding
    };

    class PhysicsAllocator : public PxAllocatorCallback {
    public:
        void* allocate(size_t size, const char*, const char*, int) {
            void* p = aligned_malloc(size, 16);
            //XASSERT((reinterpret_cast<size_t>(p) & 15) == 0, "PhysicsAllocator error");
            return p;
        }

        void deallocate(void* p) {
            aligned_free(p);
        }
    };

    class PhysicsErrorCallback : public PxErrorCallback {
    public:
        PhysicsErrorCallback() {

        }

        ~PhysicsErrorCallback() {

        }

        virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) {
            XLOG(Engine::GetInstance(), "physx error, code %d, message %s, file %s, line %d", code, message, file, line);
        }
    };

    extern PhysicsAllocator g_allocator;
    extern PhysicsErrorCallback g_error_callback;

    extern PxFoundation* g_pxfoundation;
    extern PxPhysics* g_pxphysics;
    extern PxCooking* g_cooking;
    extern PxDefaultCpuDispatcher* g_pxdispatcher;
    extern PxPvd* g_pxpvd;
    extern PxCudaContextManager* g_cuda_context_manager;

#pragma pack(push, 1)
    struct PhysxIdentity {
        union {
            struct {
                const int word2;
                const int word3;
            };
            void* const userdata;
        };

        PhysxIdentity(void* p) : userdata(p) {}
    };
#pragma pack(pop)

}
