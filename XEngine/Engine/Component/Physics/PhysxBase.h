#ifndef __PhysxBase_h__
#define __PhysxBase_h__

#include "Header.h"

namespace XEngine {
    class PhysxScene;

#define CREATE_PHYSX_BASE(scene, shape, actor, context) PhysxBase::Create(scene, shape, actor, context, __FILE__, __LINE__)

    class PhysxBase : public Api::iPhysxBase {
    public:
        virtual ~PhysxBase() {}

        static PhysxBase* Create(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* contexnt, const char* file, const int line);

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
        virtual void SetPosition(const Vector3& position) override;
        virtual void SetRotation(const Vector3& rotation) override;

        virtual void UpdatePositionAndRotation();

        virtual void Release() override;

        PhysxScene* GetScene() const { return _Scene; }

    private:
        PhysxBase(PhysxScene* scene, PxShape* shape, PxRigidActor* actor, Api::iPhysxContext* context, const char* file, const int line);

    public:
        PhysxScene* const _Scene;
        PxRigidActor* const _Actor;
        PxShape* const _Shape;

        const std::string _File;
        const int _Line;
        bool _IsRelease;
        bool _UserResetPostionOrRotation;
    private:
        int _Layer;
    };
}

#endif //__PhysxBase_h__
