#ifndef __PhysxScene_h__
#define __PhysxScene_h__

#include "Header.h"

namespace XEngine {
    class PhysxScene : public Api::iPhysxScene, public SafeThread {
        struct LoadMesh {
            MeshData* data;
            const Vector3& pos;
        };

    public:
        virtual ~PhysxScene() {}
        PhysxScene(PxScene* scene, const float static_friction, const float dynamic_friction, const float restitution);

        virtual Api::iPhysxPlane* CreatePlane(const float nx, const float ny, const float nz, const float distance, Api::iPhysxContext* const context);
        virtual Api::iPhysxBox* CreateBox(const Api::eRigType type, const Vector3& pos, const Quaternion& qt, const Vector3& size, Api::iPhysxContext* const context);
        virtual Api::iPhysxCapsule* CreateCapsule(const Api::eRigType type, const Vector3& pos, const Quaternion& qt, const float radius, const float height, Api::iPhysxContext* const context);
        virtual Api::iPhysxConvexMesh* CreateConvexMesh(const Api::eRigType type, const Quaternion& qt, Api::iPhysxContext* const context);
        virtual Api::iPhysxTriangleMesh* CreateTriangleMesh(const Api::eRigType type, const Vector3& pos, const Quaternion& qt, const float scale, const Api::eMeshFileType meshfiletype, const std::string& file, Api::iPhysxContext* const data);

        virtual void Simulate(const float elapsed_time);
        virtual bool FetchResults(bool block);

    protected:
        virtual void Run(void* constext);
    private:
        PxScene* const _scene;
        PxMaterial* const _material;
    };
}

#endif //__PhysxScene_h__
