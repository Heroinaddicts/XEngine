#include "PhysxBase.h"
#include "PhysxScene.h"

namespace XEngine {
    PhysxBase* PhysxBase::Create(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* context, const char* file, const int line) {
        return xnew PhysxBase(scene, shape, actor, context, file, line);
    }

    PhysxBase::PhysxBase(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* context, const char* file, const int line)
        : iPhysxBase(context), _Scene(scene), _Shape(shape), _Actor(actor), _Layer(0), _File(file), _Line(line), _IsRelease(false) {

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
            context->OnPhysxCreated(true);
        }
        shape->userData = this;
        //shape->setFlags((shape->getFlags() | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eVISUALIZATION) & ~PxShapeFlag::eTRIGGER_SHAPE);
    }

    void PhysxBase::SetActive(const bool b) {
        _Actor->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, b);
    }

    bool PhysxBase::IsActive() const {
        return true; // !_Actor->getActorFlags().isSet(PxActorFlag::eSEND_SLEEP_NOTIFIES);
    }

    void PhysxBase::SetKinematic(const bool b) {
        if (_Actor->getType() != PxActorType::eRIGID_DYNAMIC) {
            return;
        }
        PxRigidDynamic* body = static_cast<PxRigidDynamic*>(_Actor);
        body ? body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, b) : void(0);
    }

    bool PhysxBase::IsKinematic() const {
        if (_Actor->getType() != PxActorType::eRIGID_DYNAMIC) {
            return false;
        }
        PxRigidDynamic* body = static_cast<PxRigidDynamic*>(_Actor);
        return body ? body->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC) : false;
    }

    void PhysxBase::SetCCD(const bool b) {
        if (_Actor->getType() != PxActorType::eRIGID_DYNAMIC) {
            return;
        }
        PxRigidDynamic* body = static_cast<PxRigidDynamic*>(_Actor);
        body ? body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, b) : void(0);
    }

    bool PhysxBase::IsCCD() const {
        if (_Actor->getType() != PxActorType::eRIGID_DYNAMIC) {
            return false;
        }
        PxRigidDynamic* body = static_cast<PxRigidDynamic*>(_Actor);
        return body ? body->getRigidBodyFlags().isSet(PxRigidBodyFlag::eENABLE_CCD) : false;
    }

    void PhysxBase::SetTrigger(const bool b) {
        if (b) {
            _Shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            _Shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
        }
        else {
            _Shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
            _Shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
        }
    }

    bool PhysxBase::IsTrigger() const {
        return _Shape->getFlags().isSet(PxShapeFlag::eTRIGGER_SHAPE);
    }

    void PhysxBase::SetUseGravity(const bool b) {
        _Actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, b);
    }

    bool PhysxBase::IsUseGravity() const {
        return !_Actor->getActorFlags().isSet(PxActorFlag::eDISABLE_GRAVITY);
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
        tf.p.x = position._X;
        tf.p.y = position._Y;
        tf.p.z = position._Z;
        _Shape->setLocalPose(tf);
    }

    void PhysxBase::SetRotation(const Vector3& rotation) {
        PxTransform tf = _Shape->getLocalPose();
        Quaternion q = Quaternion::Euler(rotation);
        tf.q.x = q._X;
        tf.q.y = q._Y;
        tf.q.z = q._Z;
        tf.q.w = q._W;
        _Shape->setLocalPose(tf);
    }

    void PhysxBase::Release() {
        if (_IsRelease) {
            return;
        }

        SafeMemory::Memset((void*)&(_context->_physx_base), sizeof(_context->_physx_base), 0, sizeof(_context->_physx_base));
        _context->OnPhysxRelease();
        SafeMemory::Memset((void*)&_context, sizeof(_context), 0, sizeof(_context));
        _Scene->Release(this);
        //_Actor->release();
        //_Actor->setBaseFlag(PxBaseFlag::eIS_RELEASABLE)
    }
}
