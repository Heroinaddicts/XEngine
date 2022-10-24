#ifndef __iPhysicsApi_h__
#define __iPhysicsApi_h__

#include "MultiSys.h"
#include "Geometry/Vector.h"
#include "Geometry/Quaternion.h"
#include "X3DObj.h"

namespace XEngine {
    enum class eRigType {
        Dynamic,
        Kinematic,
        Static
    };

    enum eQueryTriggerInteraction {
        Global,
        Ignore,
        Hit
    };

    namespace Api {
        class iPhysxContext;
    }

    class Ray {
    public:
        Vector3 origin;
        Vector3 direction;

        Ray() {}
        Ray(const Vector3& org, const Vector3& dir) : origin(org), direction(dir) {}
        Vector3 GetPoint(const float t) const { return origin + direction * t; }
    };

    struct RaycastHit {
        Vector3 point;
        Vector3 normal;
        unsigned_int32 face;
        Vector2 uv;
        Api::iPhysxContext* context;
    };

    enum class eInterpolate {
        None,
        Interpolate,
        Extrapolate
    };

    enum class eCollisionDetection {
        Discrete,
        Continuous,
        ContinuousDynamic,
        ContinuousSpeculative
    };

    enum class ePhysxFlags {
        IsActive = 1,
        IsStatic = 1 << 1,
        IsKinematic = 1 << 2,
        IsActiveCCD = 1 << 3,
        OnlyTirgger = 1 << 4,
        UseGravity = 1 << 5
    };

    namespace Api {
        class iPhysxBase {
        public:
            virtual ~iPhysxBase() {}
            iPhysxBase(iPhysxContext* const context) : _context(context) {}

            virtual void SetFlags(ePhysxFlags flags, bool b) = 0;
            virtual void SetLayer(const int index) = 0; //index range in [0-31]

            virtual void SetMass(const float mass) = 0;
            virtual void SetDrag(const float drag) = 0;
            virtual void SetAngularDrag(const float angularDrag) = 0;

            virtual void SetInterpolate(const eInterpolate type) = 0; //物理运动插值模式 建议服务器使用None
            virtual void SetCollisionDetection(const eCollisionDetection type) = 0;

            //virtual void UpdateMaterial()  更新物理材质

            virtual Vector3 Position() const = 0;
            virtual Vector3 Rotation() const = 0;

            virtual void SetPosition(const Vector3& position) = 0;
            virtual void SetRotation(const Vector3& rotation) = 0;

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

            virtual void SetFlags(ePhysxFlags flags, bool b) { _physx_base ? _physx_base->SetFlags(flags, b) : void(0); }
            virtual void SetLayer(const int index) { _physx_base ? _physx_base->SetLayer(index) : void(0); }

            virtual void SetMass(const float mass) { _physx_base ? _physx_base->SetMass(mass) : void(0); }
            virtual void SetDrag(const float drag) { _physx_base ? _physx_base->SetDrag(drag) : void(0); }
            virtual void SetAngularDrag(const float angularDrag) { _physx_base ? _physx_base->SetAngularDrag(angularDrag) : void(0); }

            virtual void SetInterpolate(const eInterpolate type) { _physx_base ? _physx_base->SetInterpolate(type) : void(0); }
            virtual void SetCollisionDetection(const eCollisionDetection type) { _physx_base ? _physx_base->SetCollisionDetection(type) : void(0); }

            iPhysxContext(void* const data) : _data(data), _physx_base(nullptr) {}

            template<typename T>
            T* GetData() { return dynamic_cast<T*>(_data); }

            void* const _data;
            iPhysxBase* const _physx_base;


            virtual Vector3 Position() {
                return position;
            }

            virtual Vector3 Rotation() {
                return rotation;
            }

            virtual void SetPosition(const Vector3& pos) {
                position = pos;
                _physx_base ? _physx_base->SetPosition(pos) : void(0);
            }

            virtual void SetRotation(const Vector3& r) {
                rotation = r;
                _physx_base ? _physx_base->SetRotation(r) : void(0);
            }

        private:
            Vector3 position;
            Vector3 rotation;
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

            virtual bool Raycast(const Ray& ray, const float distance, int layerMask, const eQueryTriggerInteraction queryTriggerInteraction, RaycastHit& hit) = 0;

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
