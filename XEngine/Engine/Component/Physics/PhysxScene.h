#ifndef __PhysxScene_h__
#define __PhysxScene_h__

#include "Header.h"

namespace XEngine {
    class PhysxScene : public Api::iPhysxScene, public PxSimulationEventCallback, public PxContactModifyCallback, public PxCCDContactModifyCallback, public SafeThread {
    public:
        virtual ~PhysxScene() {}
        PhysxScene(PxScene* scene, const float static_friction, const float dynamic_friction, const float restitution);

        virtual void CreatePlane(const float nx, const float ny, const float nz, const float distance, Api::iPhysxContext* const context);
        virtual void CreateBox(const Api::eRigType type, const Vector3& pos, const Quaternion& qt, const Vector3& size, Api::iPhysxContext* const context);
        virtual void CreateCapsule(const Api::eRigType type, const Vector3& pos, const Quaternion& qt, const float radius, const float height, Api::iPhysxContext* const context);
        virtual void CreateConvexMesh(const Api::eRigType type, const Quaternion& qt, Api::iPhysxContext* const context);
        virtual void CreateTriangleMesh(
            const Api::eRigType type,
            const Vector3& pos,
            const Quaternion& qt,
            const Vector3& scale,
            const X3DObj* obj,
            Api::iPhysxContext* const data
        );

        virtual void Simulate(const float elapsed_time);
        virtual bool FetchResults(bool block);

        // 通过 PxSimulationEventCallback 继承
        virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
        virtual void onWake(PxActor** actors, PxU32 count) override;
        virtual void onSleep(PxActor** actors, PxU32 count) override;
        virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
        virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
        virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;

        // 通过 PxContactModifyCallback 继承
        virtual void onContactModify(PxContactModifyPair* const pairs, PxU32 count) override;

        // 通过 PxCCDContactModifyCallback 继承
        virtual void onCCDContactModify(PxContactModifyPair* const pairs, PxU32 count) override;

    protected:
        virtual void Run(void* constext);

    public:
        PxScene* const _scene;
        PxMaterial* const _material;

    };
}

#endif //__PhysxScene_h__
