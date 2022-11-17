#ifndef __PhysxComponent_h__
#define __PhysxComponent_h__

#include "Header.h"

namespace XEngine {
    class GameObject;
    class PhysxScene;

#define CREATE_PHYSX_BASE(scene, shape, actor, context) PhysxComponent::Create(scene, shape, actor, context, __FILE__, __LINE__)

    class PhysxComponent : public Api::iPhysxComponent {
    public:
        virtual ~PhysxComponent() {}

        static PhysxComponent* Create(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, GameObject* gameObject, const char* file, const int line);

        // 通过 iPhysxComponent 继承
        virtual void SetActive(const bool b) override;
        virtual bool IsActive() const override;

        virtual void SetKinematic(const bool b) override;
        virtual bool IsKinematic() const override;

        virtual void SetCCD(const bool b) override;
        virtual bool IsCCD() const override;

        virtual void SetTrigger(const bool b) override;
        __forceinline bool IsTrigger() const override { return _Shape->getFlags().isSet(PxShapeFlag::eTRIGGER_SHAPE); }

        __forceinline void SetUseGravity(const bool b) override { _Actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, b); }
        __forceinline bool IsUseGravity() const override { return !_Actor->getActorFlags().isSet(PxActorFlag::eDISABLE_GRAVITY); }

        __forceinline void SetLayer(const eLayer layer) override { _Layer = layer; }
        __forceinline eLayer GetLayer() const override { return _Layer; }
        virtual void SetMass(const float mass) override;
        virtual void SetDrag(const float drag) override;
        virtual void SetAngularDrag(const float angularDrag) override;
        virtual void SetInterpolate(const eInterpolate type) override;
        virtual void SetCollisionDetection(const eCollisionDetection type) override;

        // 通过 iPhysxComponent 继承
        virtual Api::iGameObject* GetGameObject() const override;

        virtual void UpdatePositionAndRotation();

        virtual bool Raycast(const Ray& ray, const float distance, int layerMask, const eQueryTriggerInteraction queryTriggerInteraction, RaycastHit& hit, int layer);

        PhysxScene* GetScene() const { return _Scene; }

    private:
        PhysxComponent(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, GameObject* gameObject, const char* file, const int line);

    public:
        PhysxScene* const _Scene;
        PxShape* const _Shape;
        PxRigidActor* const _Actor;
        GameObject* _GameObject;

        const std::string _File;
        const int _Line;
        bool _IsRelease;
        bool _UserResetPostionOrRotation;
    private:
        eLayer _Layer;
    };
}

#endif //__PhysxComponent_h__
