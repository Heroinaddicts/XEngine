#pragma once

#include <list>

#include "iPhysics.h"
#include "Engine.h"
#include "PxPhysicsAPI.h"
#include <set>
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
    struct UserDataIdentity {
        union {
            struct {
                const PxU32 worda;
                const PxU32 wordb;
            };
            void* const data;
        };

        UserDataIdentity(PxU32 w0, PxU32 w1) : worda(w0), wordb(w1) {}
        UserDataIdentity(void* p) : data(p) {}
    };
#pragma pack(pop)

    __forceinline void* GetUserData(PxU32 a, PxU32 b) {
        return UserDataIdentity(a, b).data;
    }

    __forceinline void GetWords(void* p, PxU32& a, PxU32& b) {
        UserDataIdentity udi(p);
        a = udi.worda;
        b = udi.wordb;
    }

    struct PhysicsLayerRelation {
        const int layerA;
        const int layerB;

        PhysicsLayerRelation(int a, int b) : layerA(a), layerB(b) {}
        PhysicsLayerRelation(const PhysicsLayerRelation& target) : layerA(target.layerA), layerB(target.layerB) {}

        bool operator==(const PhysicsLayerRelation& target) const {
            return (layerA == target.layerA && layerB == target.layerB) || (layerA == target.layerB && layerB == target.layerA);
        }

        bool operator <(const PhysicsLayerRelation& target) const {
            return (layerA + layerB) < (target.layerA + target.layerB);
        }
    };
}
