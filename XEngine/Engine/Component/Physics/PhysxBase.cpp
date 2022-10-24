#include "PhysxBase.h"
#include "PhysxScene.h"

namespace XEngine {
    PhysxBase* PhysxBase::Create(PhysxScene* scene, PxShape* shape, PxActor* actor, Api::iPhysxContext* context) {
        return xnew PhysxBase(scene, shape, actor, context);
    }

    PhysxBase::PhysxBase(PhysxScene* scene, PxShape* shape, PxActor* actor, Api::iPhysxContext* context)
        : iPhysxBase(context), _scene(scene), _shape(shape), _actor(actor) {
        PhysxIdentity id(this);
        {
            PxFilterData fd = _shape->getSimulationFilterData();
            fd.word2 = id.word2;
            fd.word3 = id.word3;
            _shape->setSimulationFilterData(fd);
        }
        {
            PxFilterData fd = _shape->getQueryFilterData();
            fd.word2 = id.word2;
            fd.word3 = id.word3;
            _shape->setQueryFilterData(fd);
        }

        context ? context->OnCreated(true) : void(0);
    }

    void PhysxBase::SetSimulationFlag(const int flags, bool b) {
        PxFilterData fd = _shape->getSimulationFilterData();
        if (b) {
            fd.word0 |= flags;
        }
        else {
            fd.word0 &= ~flags;
        }
        _shape->setSimulationFilterData(fd);
    }

    void PhysxBase::SetQueryFlag(const int flags, bool b) {
        PxFilterData fd = _shape->getQueryFilterData();
        if (b) {
            fd.word0 |= flags;
        }
        else {
            fd.word0 &= ~flags;
        }
        _shape->setQueryFilterData(fd);
    }

    void PhysxBase::SetLayer(const ePhysxLayer index) {
        {
            PxFilterData fd = _shape->getSimulationFilterData();
            fd.word1 = (int)index;
            _shape->setSimulationFilterData(fd);
        }
        {
            PxFilterData fd = _shape->getQueryFilterData();
            fd.word1 = (int)index;
            _shape->setQueryFilterData(fd);
        }
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
        PxTransform tf = _shape->getLocalPose();
        return Vector3(tf.p.x, tf.p.y, tf.p.z);
    }

    Vector3 PhysxBase::Rotation() const {
        return Vector3();
    }

    void PhysxBase::SetPosition(const Vector3& position) {
        PxTransform tf = _shape->getLocalPose();
        tf.p.x = position.x;
        tf.p.y = position.y;
        tf.p.z = position.z;
        _shape->setLocalPose(tf);
    }

    void PhysxBase::SetRotation(const Vector3& rotation) {
        PxTransform tf = _shape->getLocalPose();
        Quaternion q = Quaternion::Euler(rotation);
        tf.q.x = q.x;
        tf.q.y = q.y;
        tf.q.z = q.z;
        tf.q.w = q.w;
        _shape->setLocalPose(tf);
    }
}
