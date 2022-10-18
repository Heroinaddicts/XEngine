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
