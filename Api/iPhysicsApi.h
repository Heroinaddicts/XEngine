#ifndef __iPhysicsApi_h__
#define __iPhysicsApi_h__

#include "MultiSys.h"
#include "Geometry/Vector.h"
#include "Geometry/Quaternion.h"

namespace XEngine {
    namespace Api {
        enum class eUnitType {
            Plane,
            Box,
            Capsule,
            ConvexMesh,
            TriangleMesh
        };

        enum class eMeshFileType {
            Obj,
            Fbx
        };

        enum class eRigType {
            Dynamic,
            Kinematic,
            Static
        };

        struct PhysxMesh {

        };

        class iPhysxContext;
        class iPhysxBase {
        public:
            virtual ~iPhysxBase() {}
            iPhysxBase(const eUnitType type, iPhysxContext* const context) : _type(type), _context(context) {}

            virtual Vector3 WorldPosition() = 0;
            virtual Vector3 WorldRotation() = 0;

            virtual Vector3 LocalPosition() = 0;
            virtual Vector3 LocalRotation() = 0;

            const eUnitType _type;
            iPhysxContext* const _context;
        };

        class iPhysxContext {
        public:
            virtual ~iPhysxContext() {}

            virtual void OnPhysxCreated(bool successed) = 0;
            virtual void OnPhysxReleased() = 0;

            virtual void OnCollisionEnter() = 0;
            virtual void OnCollisionExit() = 0;

            virtual Vector3 PhysxWorldPosition() {
                if (_physx_base) {
                    return _physx_base->WorldPosition();
                }
            }

            virtual Vector3 PhysxWorldRotation() {
                if (_physx_base) {
                    return _physx_base->WorldRotation();
                }
            }

            virtual Vector3 PhysxLocalPosition() {
                if (_physx_base) {
                    return _physx_base->LocalPosition();
                }
            }

            virtual Vector3 PhysxLocalRotation() {
                if (_physx_base) {
                    return _physx_base->LocalRotation();
                }
            }

            iPhysxContext(void* const data) : _data(data), _physx_base(nullptr) {}
            void* const _data;
            iPhysxBase* const _physx_base;
        };

        class iPhysxPlane : public iPhysxBase {
        public:
            virtual ~iPhysxPlane() {}
        };

        class iPhysxBox : public iPhysxBase {
        public:
            virtual ~iPhysxBox() {}
        };

        class iPhysxCapsule : public iPhysxBase {
        public:
            virtual ~iPhysxCapsule() {}
        };

        class iPhysxConvexMesh : public iPhysxBase {
        public:
            virtual ~iPhysxConvexMesh() {}
        };

        class iPhysxTriangleMesh : public iPhysxBase {
        public:
            virtual ~iPhysxTriangleMesh() {}
            iPhysxTriangleMesh(const eUnitType type, iPhysxContext* const context) :iPhysxBase(type, context) {}
        };

        class iPhysxScene {
        public:
            virtual ~iPhysxScene() {}

            virtual iPhysxPlane* CreatePlane(const float nx, const float ny, const float nz, const float distance, Api::iPhysxContext* const context = nullptr) = 0;
            virtual iPhysxBox* CreateBox(const eRigType type, const Vector3& pos, const Quaternion& qt, const Vector3& size, Api::iPhysxContext* const context = nullptr) = 0;
            virtual iPhysxCapsule* CreateCapsule(const eRigType type, const Vector3& pos, const Quaternion& qt, const float radius, const float height, Api::iPhysxContext* const context = nullptr) = 0;
            virtual iPhysxConvexMesh* CreateConvexMesh(const eRigType type, const Quaternion& qt, Api::iPhysxContext* const context = nullptr) = 0;
            virtual iPhysxTriangleMesh* CreateTriangleMesh(const eRigType type, const Vector3& pos, const Quaternion& qt, const float scale, const eMeshFileType meshfiletype, const std::string& file, iPhysxContext* const data = nullptr) = 0;

            virtual void Simulate(float dt) = 0;
            virtual bool FetchResults(bool block) = 0;
        };

        class iPhysicsApi {
        public:
            virtual ~iPhysicsApi() {}

            virtual iPhysxScene* CreateScene(const float static_friction, const float dynamic_friction, const float restitution) = 0;
            virtual void ReleaseScene(iPhysxScene* scene) = 0;
        };
    }
}

#endif //__iPhysicsApi_h__
