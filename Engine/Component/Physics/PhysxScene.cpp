#include "PhysxScene.h"

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

    Api::iPhysxBox* PhysxScene::CreateBox(const Api::eRigType type, const Vector3& pos, const Vector3& size, void* const iPhysxContext) {
        PxShape* shape = g_pxphysics->createShape(PxBoxGeometry(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f), *_material);
        PxTransform localTm(PxVec3(pos.x, pos.y, pos.z));
        PxRigidDynamic* body = g_pxphysics->createRigidDynamic(localTm);
        body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
        body->userData = iPhysxContext;
        body->attachShape(*shape);
        PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
        _scene->addActor(*body);
        shape->release();
        return nullptr;
    }

    Api::iPhysxCapsule* PhysxScene::CreateCapsule(const Api::eRigType type, const Vector3& pos, const float radius, const float height, void* const iPhysxContext) {
        PxShape* shape = g_pxphysics->createShape(PxCapsuleGeometry(radius, height / 2.0f), *_material);
        PxTransform localTm(PxVec3(pos.x, pos.y, pos.z));
        PxRigidDynamic* body = g_pxphysics->createRigidDynamic(localTm);
        body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
        body->attachShape(*shape);
        PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
        _scene->addActor(*body);
        shape->release();
        return nullptr;
    }

    Api::iPhysxConvexMesh* PhysxScene::CreateConvexMesh(const Api::eRigType type, void* const iPhysxContext) {
        return nullptr;
    }

    void PhysxScene::Simulate(const float elapsed_time) {
        _scene->simulate(elapsed_time);
    }

    bool PhysxScene::FetchResults(bool block) {
        return _scene->fetchResults(block);
    }
}
