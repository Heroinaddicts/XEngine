#include "PhysxScene.h"
#include "SafeSystem.h"
#include "PhysxBase.h"
#include "Collider.hpp"
#include "PhysxSceneFilterCallback.h"

namespace XEngine {
    PhysxScene::PhysxScene(
        PxScene* scene,
        const float static_friction,
        const float dynamic_friction,
        const float restitution)
        : _Scene(scene),
        _Material(g_PxPhysics->createMaterial(static_friction, dynamic_friction, restitution)) {

        _Scene->userData = this;
        PxFilterData* pfd = xnew PxFilterData;
        GetWords(this, pfd->word0, pfd->word1);
        _Scene->setFilterShaderData(pfd, sizeof(PxFilterData));

        _Scene->setSimulationEventCallback(this);
        _Scene->setCCDContactModifyCallback(this);
        _Scene->setContactModifyCallback(this);
        _Scene->setSceneQueryUpdateMode(PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_ENABLED);
    }

    void PhysxScene::RelationPhysicsLayer(const int layerA, const int layerB) {
        _PhysicsLayerRelations.insert(PhysicsLayerRelation(layerA, layerB));
    }

    PxFilterFlags PhysxScene::PhysxSimulationFilterShader(
        PxFilterObjectAttributes attributes0,
        PxFilterData filterData0,
        PxFilterObjectAttributes attributes1,
        PxFilterData filterData1,
        PxPairFlags& pairFlags,
        const void* constantBlock,
        PxU32 constantBlockSize) {

        //printf("PhysxScene::PhysxSimulationFilterShader, %lld\n", SafeSystem::Process::GetCurrentThreadID());

        PhysxBase* pb0 = static_cast<PhysxBase*>(GetUserData(filterData0.word0, filterData0.word1));
        PhysxBase* pb1 = static_cast<PhysxBase*>(GetUserData(filterData1.word0, filterData1.word1));

        if (nullptr == pb0 || nullptr == pb1) {
            return PxFilterFlag::eDEFAULT;
        }

        XASSERT(pb0->GetScene() && pb1->GetScene() && pb0->GetScene() == pb1->GetScene(), "wtf");
        if (pb0->GetScene() == nullptr || pb1->GetScene() == nullptr || pb0->GetScene() != pb1->GetScene()) {
            XERROR(Engine::GetInstance(), "fatal error");
            return PxFilterFlag::eSUPPRESS;
        }

        if (!pb0->IsActive() || !pb1->IsActive()) {
            return PxFilterFlag::eSUPPRESS;
        }

        if (PxFilterObjectIsKinematic(attributes0) && PxFilterObjectIsKinematic(attributes1)) {
            pairFlags = PxPairFlags();
            return PxFilterFlag::eSUPPRESS;
        }

        PhysxScene* scene = pb0->GetScene();
        if (scene->_PhysicsLayerRelations.find(PhysicsLayerRelation(pb0->GetLayer(), pb1->GetLayer())) == scene->_PhysicsLayerRelations.end()) {
            return PxFilterFlag::eSUPPRESS;
        }

        if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1)) {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        }
        else {
            pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;
        }

        return PxFilterFlag::eDEFAULT;
    }

    void PhysxScene::CreatePlane(const float nx, const float ny, const float nz, const float distance, Api::iPhysxContext* const context) {
        PxRigidStatic* groundPlane = PxCreatePlane(*g_PxPhysics, PxPlane(nx, ny, nz, distance), *_Material);
        if (nullptr == groundPlane) {
            context ? context->OnPhysxCreated(false) : void(0);
            return;
        }

        PxShape* shape = nullptr;
        groundPlane->userData = context;
        groundPlane->getShapes(&shape, sizeof(shape));
        _Scene->addActor(*groundPlane);
        CREATE_PHYSX_BASE(this, shape, groundPlane, context);
    }

    void PhysxScene::CreateBox(const eRigType type, const Vector3& pos, const Quaternion& qt, const Vector3& size, Api::iPhysxContext* const context) {
        PxShape* shape = g_PxPhysics->createShape(PxBoxGeometry(size._X / 2.0f, size._Y / 2.0f, size._Z / 2.0f), *_Material);
        if (nullptr == shape) {
            context ? context->OnPhysxCreated(false) : void(0);
            return;
        }

        PxFilterData fd = shape->getSimulationFilterData();
        shape->setSimulationFilterData(fd);

        PxRigidActor* actor = nullptr;
        switch (type) {
        case eRigType::Dynamic:
            actor = g_PxPhysics->createRigidDynamic(PxTransform(PxVec3(pos._X, pos._Y, pos._Z)));
            break;
        case eRigType::Static:
            actor = g_PxPhysics->createRigidStatic(PxTransform(PxVec3(pos._X, pos._Y, pos._Z)));
            break;
        }

        if (nullptr == actor) {
            context ? context->OnPhysxCreated(false) : void(0);
            shape->release();
            return;
        }

        if (context) {
            context->SetPosition(pos);
            context->SetRotation(qt.EulerAngles());
        }

        PhysxBase* pb = CREATE_PHYSX_BASE(this, shape, actor, context);
        type == eRigType::Dynamic ? _PhysxBasePool.insert(pb) : void(0);

        actor->attachShape(*shape);
        _Scene->addActor(*actor);
        shape->release();
    }

    void PhysxScene::CreateCapsule(const eRigType type, const Vector3& pos, const Quaternion& qt, const float radius, const float height, Api::iPhysxContext* const context) {
        PxShape* shape = g_PxPhysics->createShape(PxCapsuleGeometry(radius, height / 2.0f), *_Material);
        if (nullptr == shape) {
            context ? context->OnPhysxCreated(false) : void(0);
            return;
        }

        PxRigidActor* actor = nullptr;
        switch (type) {
        case eRigType::Dynamic:
            actor = g_PxPhysics->createRigidDynamic(PxTransform(PxVec3(pos._X, pos._Y, pos._Z)));
            break;
        case eRigType::Static:
            actor = g_PxPhysics->createRigidStatic(PxTransform(PxVec3(pos._X, pos._Y, pos._Z)));
            break;
        }

        if (nullptr == actor) {
            context ? context->OnPhysxCreated(false) : void(0);
            shape->release();
            return;
        }

        if (context) {
            context->SetPosition(pos);
            context->SetRotation(qt.EulerAngles());
        }

        PhysxBase* pb = CREATE_PHYSX_BASE(this, shape, actor, context);
        type == eRigType::Dynamic ? _PhysxBasePool.insert(pb) : void(0);

        actor->attachShape(*shape);
        _Scene->addActor(*actor);
        shape->release();
    }

    void PhysxScene::CreateConvexMesh(const eRigType type, const Quaternion& qt, Api::iPhysxContext* const context) {
    }

    void PhysxScene::CreateTriangleMesh(
        const eRigType type,
        const Vector3& pos,
        const Quaternion& qt,
        const Vector3& scale,
        const X3DObj* obj,
        Api::iPhysxContext* context
    ) {
        const PxU32 numVertices = obj->GetV().size();
        const PxU32 numTriangles = obj->GetF().size();

        PxVec3* vertices = (PxVec3*)_alloca(sizeof(PxVec3) * numVertices);
        PxU32* indices = (PxU32*)_alloca(sizeof(PxU32) * numTriangles * 3);

        // 加载顶点
        for (int i = 0; i < numVertices; ++i) {
            PxVec3 vectmp(obj->GetV()[i]._X * scale._X, obj->GetV()[i]._Y * scale._Y, obj->GetV()[i]._Z * scale._Z);
            vertices[i] = vectmp;
        }

        // 加载面
        auto faceIt = obj->GetF().begin();
        for (int i = 0; i < numTriangles && faceIt != obj->GetF().end(); faceIt++, ++i) {
            indices[i * 3 + 0] = (*faceIt)[0]._U;
            indices[i * 3 + 1] = (*faceIt)[1]._U;
            if ((*faceIt).size() >= 3)
                indices[i * 3 + 2] = (*faceIt)[2]._U;
        }

        PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = numVertices;
        meshDesc.points.data = vertices;
        meshDesc.points.stride = sizeof(PxVec3);

        meshDesc.triangles.count = numTriangles;
        meshDesc.triangles.data = indices;
        meshDesc.triangles.stride = sizeof(PxU32) * 3;
        PxTriangleMesh* mesh = g_Cooking->createTriangleMesh(meshDesc, g_PxPhysics->getPhysicsInsertionCallback());
        if (nullptr == mesh) {
            context ? context->OnPhysxCreated(false) : void(0);
            return;
        }

        PxTriangleMeshGeometry geom(mesh);
        PxShape* shape = g_PxPhysics->createShape(geom, *_Material);
        if (nullptr == shape) {
            mesh->release();
            context ? context->OnPhysxCreated(false) : void(0);
            return;
        }

        // 设置厚度， 相当于多了一层 0.03厚的皮肤，也就是为了提前预判
        shape->setContactOffset(0.03f);
        // A negative rest offset helps to avoid jittering when the deformed mesh moves away from objects resting on it.
        // 允许穿透的厚度，当穿透指定的厚度后，就是发生弹开等动作 -0.02f 负数代表穿透后，正数代表穿透前
        shape->setRestOffset(0.01);


        PxRigidActor* actor = nullptr;
        switch (type) {
        case eRigType::Dynamic: {
            PxRigidDynamic* body = g_PxPhysics->createRigidDynamic(PxTransform(pos._X, pos._Y, pos._Z, PxQuat(qt._X, qt._Y, qt._Z, qt._W)));
            body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
            actor = body;
            break;
        }
        case eRigType::Static: {
            PxRigidStatic* body = g_PxPhysics->createRigidStatic(PxTransform(pos._X, pos._Y, pos._Z, PxQuat(qt._X, qt._Y, qt._Z, qt._W)));
            actor = body;
            break;
        }
        }

        if (nullptr == actor) {
            context ? context->OnPhysxCreated(false) : void(0);
            return;
        }

        if (context) {
            context->SetPosition(pos);
            context->SetRotation(qt.EulerAngles());
        }

        PhysxBase* pb = CREATE_PHYSX_BASE(this, shape, actor, context);
        type == eRigType::Dynamic ? _PhysxBasePool.insert(pb) : void(0);

        actor->attachShape(*shape);
        _Scene->addActor(*actor);
        shape->release();
    }

    bool PhysxScene::Raycast(const Ray& ray, const float distance, int layerMask, const eQueryTriggerInteraction queryTriggerInteraction, RaycastHit& hit) {
        return false; // return _Scene->raycast(PxVec3(ray._Origin._X, ray._Origin._Y, ray._Origin._Z), PxVec3(ray._Direction._X, ray._Direction._Y, ray._Direction._Z), distance, *PhysxSceneFilterCallback::GetInstance(), )
    }

    void PhysxScene::Simulate(const float elapsed_time) {
        _Scene->simulate(elapsed_time);
    }

    bool PhysxScene::FetchResults(bool block) {
        bool ret = _Scene->fetchResults(block);
        for (auto i = _ReleasePool.begin(); i != _ReleasePool.end(); i++) {
            _PhysxBasePool.erase(*i);
            (*i)->_Actor->release();
            xdel(*i);
        }

        for (auto i = _PhysxBasePool.begin(); i != _PhysxBasePool.end(); i++) {
            (*i)->UpdatePositionAndRotation();
        }

        _ReleasePool.clear();
        return ret;
    }

    void PhysxScene::Run(void* constext) {

    }

    void PhysxScene::onCCDContactModify(PxContactModifyPair* const pairs, PxU32 count) {
        printf("PhysxScene::onCCDContactModify, %lld\n", SafeSystem::Process::GetCurrentThreadID());
    }
    void PhysxScene::onContactModify(PxContactModifyPair* const pairs, PxU32 count) {
        printf("PhysxScene::onContactModify, %lld\n", SafeSystem::Process::GetCurrentThreadID());
    }

    void PhysxScene::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {
        printf("PhysxScene::onConstraintBreak, %lld\n", SafeSystem::Process::GetCurrentThreadID());
    }

    void PhysxScene::onWake(PxActor** actors, PxU32 count) {
        printf("PhysxScene::onWake, %lld\n", SafeSystem::Process::GetCurrentThreadID());
    }

    void PhysxScene::onSleep(PxActor** actors, PxU32 count) {
        printf("PhysxScene::onSleep, %lld\n", SafeSystem::Process::GetCurrentThreadID());
    }

    void PhysxScene::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
        printf("PhysxScene::onContact, %lld\n", SafeSystem::Process::GetCurrentThreadID());
    }

    void PhysxScene::onTrigger(PxTriggerPair* pairs, PxU32 count) {
        //printf("PhysxScene::onTrigger, %lld\n", SafeSystem::Process::GetCurrentThreadID());
        for (int i = 0; i < count; i++) {
            PhysxBase* base0 = static_cast<PhysxBase*>(pairs[i].triggerShape->userData);
            PhysxBase* base1 = static_cast<PhysxBase*>(pairs[i].otherShape->userData);
            if (base0->_Context && !base0->_IsRelease
                && base1->_Context && !base1->_IsRelease) {
                Collider collider1(base1->_Context);
                Collider collider0(base0->_Context);
                if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                    base0->_Context ? base0->_Context->OnTriggerEnter(&collider1) : void(0);
                    base1->_Context ? base1->_Context->OnTriggerEnter(&collider0) : void(0);;
                }

                if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST) {
                    base0->_Context ? base0->_Context->OnTriggerExit(&collider1) : void(0);
                    base1->_Context ? base1->_Context->OnTriggerExit(&collider0) : void(0);
                }
            }
        }
    }

    void PhysxScene::Release(PhysxBase* pb) {
        _ReleasePool.insert(pb);
    }

    void PhysxScene::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {
        printf("PhysxScene::onAdvance, %lld\n", SafeSystem::Process::GetCurrentThreadID());
    }
}
