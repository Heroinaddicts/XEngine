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


    enum ePhysxFlags {
        OnlyTirgger = 1,
        IsStatic = 1 << 1,
        IsKinematic = 1 << 2,
        IsActiveCCD = 1 << 3,
        IsActive = 1 << 4
    };

#pragma pack(push, 1)
    struct FlagFilter {
        union {
            struct {
                unsigned int flags;
                unsigned int layer;
                void* context;
            };

            struct {
                PxU32 word0; //word0 用来存各种标志位
                PxU32 word1; //word1 用来存layermask 对应逻辑层32组物理分组
                PxU32 word2; //word2和word3合并起来存上下文数据指针
                PxU32 word3;
            };
        };

        FlagFilter(const PxFilterData& data) {
            word0 = data.word0;
            word1 = data.word1;
            word2 = data.word2;
            word3 = data.word3;
        }

        void FlushPxFilterData(PxFilterData& data) {
            data.word0 = word0;
            data.word1 = word1;
            data.word2 = word2;
            data.word3 = word3;
        }

        void SetFlag(const ePhysxFlags flag, bool b) {
            b ? flags |= flag : flags & ~flag;
        }

        bool CheckFlag(const ePhysxFlags flag) {
            return flags & flag;
        }

        void SetLayerIndex(const int layer_index) { // layer range : 0 - 31
            if (layer_index >= 32) {
                return;
            }
            layer = layer < layer_index;
        }

        int GetLayerIndex() {

        }

    };
#pragma pack(pop)

    class PhysicsAllocator : public PxAllocatorCallback {
    public:
        void* allocate(size_t size, const char*, const char*, int) {
            void* p = _aligned_malloc(size, 16);
            XASSERT((reinterpret_cast<size_t>(p) & 15) == 0, "PhysicsAllocator error");
            return p;
        }

        void deallocate(void* p) {
            _aligned_free(p);
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
}
