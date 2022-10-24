#include "PhysxBase.h"
#include "PhysxScene.h"

namespace XEngine {
    PhysxBase* PhysxBase::Create(PhysxScene* _scene, PxShape* shape, PxActor* actor, Api::iPhysxContext* context) {
        return nullptr;
    }


    PhysxBase::PhysxBase(PhysxScene* scene, PxShape* shape, PxActor* actor, Api::iPhysxContext* context) : iPhysxBase(context), _scene(scene), _shape(shape), _actor(actor) {}

    void PhysxBase::SetFlags(ePhysxFlags flags, bool b) {
        PxFilterData fd = _shape->getQueryFilterData();
        if (b) {
            fd.word0 |= (int)flags;
        }
        else {
            fd.word0 &= ~(int)flags;
        }

        _shape->setQueryFilterData(fd);
    }

    void PhysxBase::SetLayer(const int index) {
        PxFilterData fd = _shape->getQueryFilterData();
        fd.word1 = index;
        _shape->setQueryFilterData(fd);
    }

    void PhysxBase::SetMass(const float mass) {

    }

    void PhysxBase::SetDrag(const float drag) {

    }

    void PhysxBase::SetAngularDrag(const float angularDrag) {

    }

    void PhysxBase::SetInterpolate(const eInterpolate type) {

    }

    void PhysxBase::SetCollisionDetection(const eCollisionDetection type) {

    }

    Vector3 PhysxBase::Position() const {
        return Vector3();
    }

    Vector3 PhysxBase::Rotation() const {
        return Vector3();
    }

    void PhysxBase::SetPosition(const Vector3& position) {

    }

    void PhysxBase::SetRotation(const Vector3& rotation) {

    }
}
