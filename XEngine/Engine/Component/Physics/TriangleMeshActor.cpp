#include "TriangleMeshActor.h"

namespace XEngine {

    Vector3 TriangleMeshActor::Position() {
        return Vector3();
    }

    Vector3 TriangleMeshActor::Rotation() {
        return Vector3();
    }

    TriangleMeshActor::TriangleMeshActor(Api::iPhysxContext* context, PxActor* actor) : Api::iPhysxTriangleMesh(Api::eUnitType::TriangleMesh, context), _actor(actor) {
    }
}
