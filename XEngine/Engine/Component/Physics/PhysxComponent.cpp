#include "PhysxComponent.h"
#include "PhysxScene.h"
#include "GameObjectManager/GameObject.h"

namespace XEngine {
    PhysxComponent* PhysxComponent::Create(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, GameObject* gameObject, const char* file, const int line) {
        return xnew PhysxComponent(scene, shape, actor, gameObject, file, line);
    }

    PhysxComponent::PhysxComponent(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, GameObject* gameObject, const char* file, const int line)
        : _Scene(scene), _Shape(shape), _Actor(actor), _GameObject(gameObject), _File(file), _Line(line) {
    }

    //     PhysxComponent::PhysxComponent(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* context, const char* file, const int line)
    //         : iPhysxBase(context), _Scene(scene), _Shape(shape), _Actor(actor), _File(file), _Line(line), _IsRelease(false), _UserResetPostionOrRotation(false), _Layer(eLayer::Default) {
    // 
    //         _Actor->userData = context;
    //         {
    //             PxFilterData fd = _Shape->getSimulationFilterData();
    //             GetWords(this, fd.word0, fd.word1);
    //             _Shape->setSimulationFilterData(fd);
    //         }
    //         {
    //             PxFilterData fd = _Shape->getQueryFilterData();
    //             GetWords(this, fd.word0, fd.word1);
    //             _Shape->setQueryFilterData(fd);
    //         }
    // 
    //         Api::iPhysxBase* pb = dynamic_cast<Api::iPhysxBase*>(this);
    // 
    //         if (context) {
    //             SafeMemory::Memcpy((void*)&(context->_PhysxBase), sizeof(Api::iPhysxBase*), &pb, sizeof(Api::iPhysxBase*));
    //             context->OnPhysxCreated(true);
    //         }
    //         shape->userData = this;
    //         //shape->setFlags((shape->getFlags() | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eVISUALIZATION) & ~PxShapeFlag::eTRIGGER_SHAPE);
    //     }

    void PhysxComponent::SetActive(const bool b) {
        _Actor->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, b);
    }

    bool PhysxComponent::IsActive() const {
        return true; // !_Actor->getActorFlags().isSet(PxActorFlag::eSEND_SLEEP_NOTIFIES);
    }

    void PhysxComponent::SetKinematic(const bool b) {
        if (_Actor->getType() != PxActorType::eRIGID_DYNAMIC) {
            return;
        }
        PxRigidDynamic* body = static_cast<PxRigidDynamic*>(_Actor);
        body ? body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, b) : void(0);
    }

    bool PhysxComponent::IsKinematic() const {
        if (_Actor->getType() != PxActorType::eRIGID_DYNAMIC) {
            return false;
        }
        PxRigidDynamic* body = static_cast<PxRigidDynamic*>(_Actor);
        return body ? body->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC) : false;
    }

    void PhysxComponent::SetCCD(const bool b) {
        if (_Actor->getType() != PxActorType::eRIGID_DYNAMIC) {
            return;
        }
        PxRigidDynamic* body = static_cast<PxRigidDynamic*>(_Actor);
        body ? body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, b) : void(0);
    }

    bool PhysxComponent::IsCCD() const {
        if (_Actor->getType() != PxActorType::eRIGID_DYNAMIC) {
            return false;
        }
        PxRigidDynamic* body = static_cast<PxRigidDynamic*>(_Actor);
        return body ? body->getRigidBodyFlags().isSet(PxRigidBodyFlag::eENABLE_CCD) : false;
    }

    void PhysxComponent::SetTrigger(const bool b) {
        if (b) {
            _Shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            _Shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
        }
        else {
            _Shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
            _Shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
        }
    }

    void PhysxComponent::SetMass(const float mass) {

    }

    void PhysxComponent::SetDrag(const float drag) {
    }

    void PhysxComponent::SetAngularDrag(const float angularDrag) {
    }

    void PhysxComponent::SetInterpolate(const eInterpolate type) {
    }

    void PhysxComponent::SetCollisionDetection(const eCollisionDetection type) {
    }


    void PhysxComponent::UpdatePositionAndRotation() {
        //         if (_Actor && _Context) {
        //             if (_UserResetPostionOrRotation) {
        //                 Vector3 pos = _Context->Position();
        //                 Quaternion q = Quaternion::Euler(_Context->Rotation());
        //                 PxTransform tf(PxVec3(pos._X, pos._Y, pos._Z), PxQuat(q._X, q._Z, q._Y, q._W));
        //                 _Actor->setGlobalPose(tf);
        //                 _UserResetPostionOrRotation = false;
        //             }
        //             else {
        //                 PxTransform tf = _Actor->getGlobalPose();
        //                 Vector3 pos(tf.p.x, tf.p.y, tf.p.z);
        //                 Quaternion q(tf.q.x, tf.q.y, tf.q.z, tf.q.w);
        //                 _Context->UpdatePositionAndRotation(pos, q.EulerAngles());
        //             }
        //         }
    }

    bool PhysxComponent::Raycast(const Ray& ray, const float distance, int layerMask, const eQueryTriggerInteraction queryTriggerInteraction, RaycastHit& hit, int layer) {

        return false;
    }

    Api::iGameObject* PhysxComponent::GetGameObject() const {
        return nullptr;
    }
}
