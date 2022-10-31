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

    extern PhysicsAllocator g_Allocator;
    extern PhysicsErrorCallback g_ErrorCallback;

    extern PxFoundation* g_PxFoundation;
    extern PxPhysics* g_PxPhysics;
    extern PxCooking* g_Cooking;
    extern PxDefaultCpuDispatcher* g_Pxdispatcher;
    extern PxPvd* g_Pxpvd;
    extern PxCudaContextManager* g_CudaContextManager;


#pragma pack(push, 1)
    struct UserDataIdentity {
        union {
            struct {
                const PxU32 _WordA;
                const PxU32 _WordB;
            };
            void* const _Data;
        };

        UserDataIdentity(PxU32 w0, PxU32 w1) : _WordA(w0), _WordB(w1) {}
        UserDataIdentity(void* p) : _Data(p) {}
    };
#pragma pack(pop)

    __forceinline void* GetUserData(PxU32 a, PxU32 b) {
        return UserDataIdentity(a, b)._Data;
    }

    __forceinline void GetWords(void* p, PxU32& a, PxU32& b) {
        UserDataIdentity udi(p);
        a = udi._WordA;
        b = udi._WordB;
    }

    struct PhysicsLayerRelation {
        const eLayer _LayerA;
        const eLayer _LayerB;

        PhysicsLayerRelation(eLayer a, eLayer b) : _LayerA(a), _LayerB(b) {}
        PhysicsLayerRelation(const PhysicsLayerRelation& target) : _LayerA(target._LayerA), _LayerB(target._LayerB) {}

        bool operator==(const PhysicsLayerRelation& target) const {
            return (_LayerA == target._LayerA && _LayerB == target._LayerB) || (_LayerA == target._LayerB && _LayerB == target._LayerA);
        }

        bool operator <(const PhysicsLayerRelation& target) const {
            return ((int)_LayerA + (int)_LayerB) < ((int)target._LayerA + (int)target._LayerB);
        }
    };
}
