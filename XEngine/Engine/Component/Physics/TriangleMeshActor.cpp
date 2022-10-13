#include "TriangleMeshActor.h"

namespace XEngine {

    Vector3 TriangleMeshActor::WorldPosition() {
        return Vector3();
    }

    Vector3 TriangleMeshActor::WorldRotation() {
        return Vector3();
    }

    Vector3 TriangleMeshActor::LocalPosition() {
        return Vector3();
    }

    Vector3 TriangleMeshActor::LocalRotation() {
        return Vector3();
    }

    TriangleMeshActor::TriangleMeshActor(Api::iPhysxContext* context, PxActor* actor) : Api::iPhysxTriangleMesh(Api::eUnitType::TriangleMesh, context), _actor(actor) {
    }
}
