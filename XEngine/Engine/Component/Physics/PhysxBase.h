#ifndef __PhysxBase_h__
#define __PhysxBase_h__

#include "Header.h"

namespace XEngine {
    class PhysxScene;

    class PhysxBase : public Api::iPhysxBase {
    public:
        virtual ~PhysxBase() {}

        static PhysxBase* Create(PhysxScene* _scene, PxShape* shape, PxActor* actor, Api::iPhysxContext* contexnt);

        // Í¨¹ý iPhysxBase ¼Ì³Ð
        virtual void SetFlags(ePhysxFlags flags, bool b) override;
        virtual void SetLayer(const int index) override;
        virtual void SetMass(const float mass) override;
        virtual void SetDrag(const float drag) override;
        virtual void SetAngularDrag(const float angularDrag) override;
        virtual void SetInterpolate(const eInterpolate type) override;
        virtual void SetCollisionDetection(const eCollisionDetection type) override;
        virtual Vector3 Position() const override;
        virtual Vector3 Rotation() const override;
        virtual void SetPosition(const Vector3& position) override;
        virtual void SetRotation(const Vector3& rotation) override;

    private:
        PhysxBase(PhysxScene* scene, PxShape* shape, PxActor* actor, Api::iPhysxContext* context);

    private:
        PhysxScene* const _scene;
        PxActor* const _actor;
        PxShape* const _shape;
    };
}

#endif //__PhysxBase_h__
