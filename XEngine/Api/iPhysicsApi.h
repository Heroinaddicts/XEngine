#ifndef __iPhysicsApi_h__
#define __iPhysicsApi_h__

#include "MultiSys.h"
#include "Geometry/Vector.h"
#include "Geometry/Quaternion.h"
#include "X3DObj.h"

namespace XEngine {
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

    namespace Api {
        class iPhysxBase {
        public:
            virtual ~iPhysxBase() {}
            iPhysxBase(const eUnitType type, iPhysxContext* const context) : _type(type), _context(context) {}

            virtual void SetLayer(const int index) = 0; //index range in [0-31]

            virtual void SetOnlyTrigger(const bool value) = 0;

            virtual void SetMass(const float mass) = 0;
            virtual void SetDrag(const float drag) = 0;
            virtual void SetAngularDrag(const float angularDrag) = 0;

            virtual void UseGravity(const bool use) = 0;
            virtual void SetKinematic(const bool value) = 0;

            virtual void SetInterpolate(const eInterpolate type) = 0; //物理运动插值模式 建议服务器使用None
            virtual void SetCollisionDetection(const eCollisionDetection type) = 0;

            //virtual void UpdateMaterial()  更新物理材质

            virtual Vector3 Position() const = 0;
            virtual Vector3 Rotation() const = 0;

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

            virtual void SetLayer(const int index) { //index range in [0-31]
                XASSERT(_physx_base, "wtf");
                if (_physx_base) {
                    _physx_base->SetLayer(index);
                }
            }

            virtual void SetOnlyTrigger(const bool value) {
                XASSERT(_physx_base, "wtf");
                if (_physx_base) {
                    _physx_base->SetOnlyTrigger(value);
                }
            }

            virtual void SetMass(const float mass) {
                XASSERT(_physx_base, "wtf");
                if (_physx_base) {
                    _physx_base->SetMass(mass);
                }
            }

            virtual void SetDrag(const float drag) {
                XASSERT(_physx_base, "wtf");
                if (_physx_base) {
                    _physx_base->SetDrag(drag);
                }
            }

            virtual void SetAngularDrag(const float angularDrag) {
                XASSERT(_physx_base, "wtf");
                if (_physx_base) {
                    _physx_base->SetAngularDrag(angularDrag);
                }
            }

            virtual void UseGravity(const bool use) {
                XASSERT(_physx_base, "wtf");
                if (_physx_base) {
                    _physx_base->UseGravity(use);
                }
            }

            virtual void SetKinematic(const bool value) {
                XASSERT(_physx_base, "wtf");
                if (_physx_base) {
                    _physx_base->SetKinematic(value);
                }
            }

            virtual void SetInterpolate(const eInterpolate type) { //物理运动插值模式 建议服务器使用None
                XASSERT(_physx_base, "wtf");
                if (_physx_base) {
                    _physx_base->SetInterpolate(type);
                }
            }

            virtual void SetCollisionDetection(const eCollisionDetection type) {
                XASSERT(_physx_base, "wtf");
                if (_physx_base) {
                    _physx_base->SetCollisionDetection(type);
                }
            }

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
