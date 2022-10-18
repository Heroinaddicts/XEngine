#ifndef __iPhysicsApi_h__
#define __iPhysicsApi_h__

#include "MultiSys.h"
#include "Geometry/Vector.h"
#include "Geometry/Quaternion.h"
#include "X3DObj.h"

namespace XEngine {
    namespace Api {
        enum class eUnitType {
            Plane,
            Box,
            Capsule,
            ConvexMesh,
            TriangleMesh
        };

        enum class eRigType {
            Dynamic,
            Kinematic,
            Static
        };


        class iPhysxContext;
        class iPhysxBase {
        public:
            virtual ~iPhysxBase() {}
            iPhysxBase(const eUnitType type, iPhysxContext* const context) : _type(type), _context(context) {}

            virtual Vector3 Position() = 0;
            virtual Vector3 Rotation() = 0;

            const eUnitType _type;
            iPhysxContext* const _context;
        };

        class iPhysxContext {
        public:
            virtual ~iPhysxContext() {}

            virtual void OnCreated(bool success) = 0;
            virtual void OnAwake() = 0;
            virtual void OnRelease() = 0;

            virtual void OnTriggerEnter(iPhysxBase* const other, const Vector3& pos, const Vector3& normal) = 0;
            virtual void OnTriggerExit(iPhysxBase* const other, const Vector3& pos, const Vector3& normal) = 0;

            virtual void OnCollisionEnter(iPhysxBase* const other, const Vector3& pos, const Vector3& normal) = 0;
            virtual void OnCollisionExit(iPhysxBase* const other, const Vector3& pos, const Vector3& normal) = 0;

            virtual Vector3 Position() {
                if (_physx_base) {
                    return _physx_base->Position();
                }
                return Vector3();
            }

            virtual Vector3 Rotation() {
                if (_physx_base) {
                    return _physx_base->Rotation();
                }
                return Vector3();
            }

            iPhysxContext(void* const data) : _data(data), _physx_base(nullptr) {}

            template<typename T>
            T* GetData() { return dynamic_cast<T*>(_data); }

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

            virtual void CreatePlane(const float nx, const float ny, const float nz, const float distance, Api::iPhysxContext* const context = nullptr) = 0;
            virtual void CreateBox(const eRigType type, const Vector3& pos, const Quaternion& qt, const Vector3& size, Api::iPhysxContext* const context = nullptr) = 0;
            virtual void CreateCapsule(const eRigType type, const Vector3& pos, const Quaternion& qt, const float radius, const float height, Api::iPhysxContext* const context = nullptr) = 0;
            virtual void CreateConvexMesh(const eRigType type, const Quaternion& qt, Api::iPhysxContext* const context = nullptr) = 0;
            virtual void CreateTriangleMesh(
                const eRigType type,
                const Vector3& pos,
                const Quaternion& qt,
                const Vector3& scale,
                const X3DObj* obj,
                iPhysxContext* const data = nullptr
            ) = 0;

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
