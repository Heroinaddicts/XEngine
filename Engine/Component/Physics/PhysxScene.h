#ifndef __PhysxScene_h__
#define __PhysxScene_h__

#include "Header.h"

namespace XEngine {
    class PhysxScene : public Api::iPhysxScene {
    public:
        virtual ~PhysxScene() {}
        PhysxScene(PxScene* scene, const float static_friction, const float dynamic_friction, const float restitution);

        virtual Api::iPhysxPlane* CreatePlane(const float nx, const float ny, const float nz, const float distance, void* const iPhysxContext);
        virtual Api::iPhysxBox* CreateBox(const Api::eRigType type, const Vector3& pos, const Vector3& size, void* const iPhysxContext);
        virtual Api::iPhysxCapsule* CreateCapsule(const Api::eRigType type, const Vector3& pos, const float radius, const float height, void* const iPhysxContext);
        virtual Api::iPhysxConvexMesh* CreateConvexMesh(const Api::eRigType type, void* const iPhysxContext);

        virtual void Simulate(const float elapsed_time);
        virtual bool FetchResults(bool block);

    private:
        PxScene* const _scene;
        PxMaterial* const _material;
    };
}

#endif //__PhysxScene_h__
