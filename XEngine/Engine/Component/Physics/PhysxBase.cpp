#include "PhysxBase.h"
#include "PhysxScene.h"

namespace XEngine {
    PhysxBase* PhysxBase::Create(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* context, const char* file, const int line) {
        return xnew PhysxBase(scene, shape, actor, context, file, line);
    }

    PhysxBase::PhysxBase(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* context, const char* file, const int line)
        : iPhysxBase(context), _Scene(scene), _Shape(shape), _Actor(actor), _Layer(0), _File(file), _Line(line), _IsRelease(false), _UserResetPostionOrRotation(false) {

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
            SafeMemory::Memcpy((void*)&(context->_PhysxBase), sizeof(Api::iPhysxBase*), &pb, sizeof(Api::iPhysxBase*));
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

    void PhysxBase::SetPosition(const Vector3& position) {
        _UserResetPostionOrRotation = true;
    }

    void PhysxBase::SetRotation(const Vector3& rotation) {
        _UserResetPostionOrRotation = true;
    }

    void PhysxBase::UpdatePositionAndRotation() {
        if (_Actor && _Context) {
            if (_UserResetPostionOrRotation) {
                Vector3 pos = _Context->Position();
                Quaternion q = Quaternion::Euler(_Context->Rotation());
                PxTransform tf(PxVec3(pos._X, pos._Y, pos._Z), PxQuat(q._X, q._Z, q._Y, q._W));
                _Actor->setGlobalPose(tf);
                _UserResetPostionOrRotation = false;
            }
            else {
                PxTransform tf = _Actor->getGlobalPose();
                Vector3 pos(tf.p.x, tf.p.y, tf.p.z);
                Quaternion q(tf.q.x, tf.q.y, tf.q.z, tf.q.w);
                _Context->UpdatePositionAndRotation(pos, q.EulerAngles());
            }
        }
    }

    void PhysxBase::Release() {
        if (_IsRelease) {
            return;
        }

        SafeMemory::Memset((void*)&(_Context->_PhysxBase), sizeof(_Context->_PhysxBase), 0, sizeof(_Context->_PhysxBase));
        _Context->OnPhysxRelease();
        SafeMemory::Memset((void*)&_Context, sizeof(_Context), 0, sizeof(_Context));
        _Scene->Release(this);
    }
}
