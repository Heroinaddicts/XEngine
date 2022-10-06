#include "PhysxScene.h"
#include "iMeshLoaderApi.h"

namespace XEngine {
    PhysxScene::PhysxScene(
        PxScene* scene,
        const float static_friction,
        const float dynamic_friction,
        const float restitution)
        : _scene(scene),
        _material(g_pxphysics->createMaterial(static_friction, dynamic_friction, restitution)) { }

    Api::iPhysxPlane* PhysxScene::CreatePlane(const float nx, const float ny, const float nz, const float distance, void* const iPhysxContext) {
        PxRigidStatic* groundPlane = PxCreatePlane(*g_pxphysics, PxPlane(nx, ny, nz, distance), *_material);
        if (groundPlane) {
            _scene->addActor(*groundPlane);
        }
        return nullptr;
    }

    Api::iPhysxBox* PhysxScene::CreateBox(const Api::eRigType type, const Vector3& pos, const Quaternion& qt, const Vector3& size, void* const iPhysxContext) {
        PxShape* shape = g_pxphysics->createShape(PxBoxGeometry(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f), *_material);
        PxRigidDynamic* body = g_pxphysics->createRigidDynamic(PxTransform(PxVec3(pos.x, pos.y, pos.z)));
        body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
        body->userData = iPhysxContext;
        body->attachShape(*shape);
        PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
        _scene->addActor(*body);
        shape->release();
        return nullptr;
    }

    Api::iPhysxCapsule* PhysxScene::CreateCapsule(const Api::eRigType type, const Vector3& pos, const Quaternion& qt, const float radius, const float height, void* const iPhysxContext) {
        PxShape* shape = g_pxphysics->createShape(PxCapsuleGeometry(radius, height / 2.0f), *_material);
        PxRigidDynamic* body = g_pxphysics->createRigidDynamic(PxTransform(PxVec3(pos.x, pos.y, pos.z)));
        body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
        body->attachShape(*shape);
        PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
        _scene->addActor(*body);
        shape->release();
        return nullptr;
    }

    Api::iPhysxConvexMesh* PhysxScene::CreateConvexMesh(const Api::eRigType type, const Quaternion& qt, void* const iPhysxContext) {
        return nullptr;
    }

    Api::iPhysxTriangleMesh* PhysxScene::CreateTriangleMesh(const Api::eRigType type, const Vector3& pos, const Quaternion& qt, const float scale, const std::string& file, Api::iPhysxContext* const userdata) {
        Api::MeshData data;
        Engine::GetInstance()->GetMeshLoaderApi()->LoadObjSync(file, data);

        const PxU32 numVertices = data.v.size();
        const PxU32 numTriangles = data.f.size();

        PxVec3* vertices = x_new PxVec3[numVertices];
        PxU32* indices = x_new PxU32[numTriangles * 3];

        // 加载顶点
        for (int i = 0; i < numVertices; ++i) {
            PxVec3 vectmp(data.v[i].x * scale, data.v[i].y * scale, data.v[i].z * scale);
            vertices[i] = vectmp;
        }
        //memcpy(vertices + 1, &objtmp->v[0], sizeof(PxVec3)* (numVertices));

        // 加载面
        auto faceIt = data.f.begin();
        for (int i = 0; i < numTriangles && faceIt != data.f.end(); faceIt++, ++i) {
            indices[i * 3 + 0] = (*faceIt)[0].u;
            indices[i * 3 + 1] = (*faceIt)[1].u;
            if ((*faceIt).size() >= 3)
                indices[i * 3 + 2] = (*faceIt)[2].u;
        }

        PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = numVertices;
        meshDesc.points.data = vertices;
        meshDesc.points.stride = sizeof(PxVec3);

        meshDesc.triangles.count = numTriangles;
        meshDesc.triangles.data = indices;
        meshDesc.triangles.stride = sizeof(PxU32) * 3;
        PxTriangleMesh* triMesh = g_cooking->createTriangleMesh(meshDesc, g_pxphysics->getPhysicsInsertionCallback());
        if (nullptr == triMesh) {
            return nullptr;
        }

        PxTriangleMeshGeometry geom(triMesh);
        PxRigidActor* actor = nullptr;
        switch (type) {
        case Api::eRigType::Dynamic: {
            actor = g_pxphysics->createRigidDynamic(PxTransform(pos.x, pos.y, pos.z, PxQuat(qt.x, qt.y, qt.z, qt.w)));
            break;
        }
        case Api::eRigType::Static: {
            actor = g_pxphysics->createRigidStatic(PxTransform(pos.x, pos.y, pos.z, PxQuat(qt.x, qt.y, qt.z, qt.w)));
            break;
        }
        }

        PxShape* shape = g_pxphysics->createShape(geom, *_material);
        {
            // 设置厚度， 相当于多了一层 0.03厚的皮肤，也就是为了提前预判
            shape->setContactOffset(0.03f);
            // A negative rest offset helps to avoid jittering when the deformed mesh moves away from objects resting on it.
            // 允许穿透的厚度，当穿透指定的厚度后，就是发生弹开等动作 -0.02f 负数代表穿透后，正数代表穿透前
            shape->setRestOffset(-0.02f);
        }

        actor->attachShape(*shape);
        shape->release();
        actor->userData = userdata;
        _scene->addActor(*actor);
        return nullptr;
    }

    void PhysxScene::Simulate(const float elapsed_time) {
        _scene->simulate(elapsed_time);
    }

    bool PhysxScene::FetchResults(bool block) {
        return _scene->fetchResults(block);
    }
}
