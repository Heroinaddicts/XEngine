#ifndef __PhysxBase_h__
#define __PhysxBase_h__

#include "Header.h"

namespace XEngine {
    class PhysxScene;

    class PhysxBase : public Api::iPhysxBase {
    public:
        virtual ~PhysxBase() {}

        static PhysxBase* Create(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* contexnt);

        // Í¨¹ý iPhysxBase ¼Ì³Ð
        virtual void SetActive(const bool b) override;
        virtual bool IsActive() const override;

        virtual void SetKinematic(const bool b) override;
        virtual bool IsKinematic() const override;

        virtual void SetCCD(const bool b) override;
        virtual bool IsCCD() const override;

        virtual void SetTrigger(const bool b) override;
        virtual bool IsTrigger() const override;

        virtual void SetUseGravity(const bool b) override;
        virtual bool IsUseGravity() const override;

        virtual void SetLayer(const int layer) override;
        virtual int GetLayer() const override;

        virtual void SetMass(const float mass) override;
        virtual void SetDrag(const float drag) override;
        virtual void SetAngularDrag(const float angularDrag) override;
        virtual void SetInterpolate(const eInterpolate type) override;
        virtual void SetCollisionDetection(const eCollisionDetection type) override;
        virtual Vector3 Position() const override;
        virtual Vector3 Rotation() const override;
        virtual void SetPosition(const Vector3& position) override;
        virtual void SetRotation(const Vector3& rotation) override;

        virtual void Release() override;

        PhysxScene* GetScene() const { return _Scene; }
    private:
        PhysxBase(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* context);

    public:
        PhysxScene* const _Scene;
        PxRigidActor* const _Actor;
        PxShape* const _Shape;

    private:
        int _Layer;
        bool _CCD;
    };
}

#endif //__PhysxBase_h__
