#ifndef __TriangleMeshActor_h__
#define __TriangleMeshActor_h__

#include "Header.h"

namespace XEngine {
    class TriangleMeshActor : public Api::iPhysxTriangleMesh {
    public:
        virtual ~TriangleMeshActor() {}
        virtual Vector3 Position() override;
        virtual Vector3 Rotation() override;

        TriangleMeshActor(Api::iPhysxContext* context, PxActor* actor);

        PxActor* const _actor;
    };
}

#endif //__TriangleMeshActor_h__
