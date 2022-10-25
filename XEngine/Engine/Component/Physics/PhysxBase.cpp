#include "PhysxBase.h"
#include "PhysxScene.h"

namespace XEngine {
    PhysxBase* PhysxBase::Create(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* context) {
        return xnew PhysxBase(scene, shape, actor, context);
    }

    PhysxBase::PhysxBase(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* context)
        : iPhysxBase(context), _Scene(scene), _Shape(shape), _Actor(actor), _Layer(0), _Active(true), _Kinematic(false), _CCD(false), _Trigger(false), _UseGravity(false) {

        _Actor->userData = context;
        {
            PxFilterData fd = _Shape->getSimulationFilterData();
            GetWords(this, fd.word0, fd.word1);
            _Shape->setSimulationFilterData(fd);
        }
        {
            PxFilterData fd = _Shape->getQueryFilterData();
            GetWords(this, fd.word0, fd.word1);
            _Shape->setQueryFilterData(fd);
        }

        Api::iPhysxBase* pb = dynamic_cast<Api::iPhysxBase*>(this);

        if (context) {
            SafeMemory::Memcpy((void*)&(context->_physx_base), sizeof(Api::iPhysxBase*), &pb, sizeof(Api::iPhysxBase*));
            context->OnCreated(true);
        }
    }

    void PhysxBase::SetActive(const bool b) {
        _Active = b;
    }

    bool PhysxBase::IsActive() const {
        return _Active;
    }

    void PhysxBase::SetKinematic(const bool b) {
        _Kinematic = b;
    }

    bool PhysxBase::GetKinematic() const {
        return _Kinematic;
    }

    void PhysxBase::ActiveCCD(const bool b) {
        _CCD = b;
    }

    bool PhysxBase::IsCCD() const {
        return _CCD;
    }

    void PhysxBase::SetTrigger(const bool b) {
        _Trigger = b;
    }

    bool PhysxBase::IsTrigger() const {
        return _Trigger;
    }

    void PhysxBase::SetUseGravity(const bool b) {
        _UseGravity = b;
    }

    bool PhysxBase::IsUseGravity() const {
        return _UseGravity;
    }

    void PhysxBase::SetLayer(const int layer) {
        _Layer = layer;
    }

    int PhysxBase::GetLayer() const {
        return _Layer;
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
        PxTransform tf = _Shape->getLocalPose();
        return Vector3(tf.p.x, tf.p.y, tf.p.z);
    }

    Vector3 PhysxBase::Rotation() const {
        return Vector3();
    }

    void PhysxBase::SetPosition(const Vector3& position) {
        PxTransform tf = _Shape->getLocalPose();
        tf.p.x = position.x;
        tf.p.y = position.y;
        tf.p.z = position.z;
        _Shape->setLocalPose(tf);
    }

    void PhysxBase::SetRotation(const Vector3& rotation) {
        PxTransform tf = _Shape->getLocalPose();
        Quaternion q = Quaternion::Euler(rotation);
        tf.q.x = q.x;
        tf.q.y = q.y;
        tf.q.z = q.z;
        tf.q.w = q.w;
        _Shape->setLocalPose(tf);
    }
}
