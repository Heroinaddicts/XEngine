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
        Vector3 _Origin;
        Vector3 _Direction;

        Ray() {}
        Ray(const Vector3& org, const Vector3& dir) : _Origin(org), _Direction(dir) {}
        Vector3 GetPoint(const float t) const { return _Origin + _Direction * t; }
    };

    struct RaycastHit {
        Vector3 _Point;
        Vector3 _Normal;
        unsigned_int32 _Face;
        Vector2 _UV;
        Api::iPhysxContext* _Context;
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

    enum class eLayer {  //You can use UserDefine like this >> #define Monster eLayer::UserDefine0
        Default = 1 << 0,
        TransparentFX = 1 << 1,
        IgnoreRaycast = 1 << 2,
        UserDefine0 = 1 << 3,
        Water = 1 << 4,
        UI = 1 << 5,
        UserDefine1 = 1 << 6,
        UserDefine2 = 1 << 7,
        UserDefine3 = 1 << 8,
        UserDefine4 = 1 << 9,
        UserDefine5 = 1 << 10,
        UserDefine6 = 1 << 11,
        UserDefine7 = 1 << 12,
        UserDefine8 = 1 << 13,
        UserDefine9 = 1 << 14,
        UserDefine10 = 1 << 15,
        UserDefine11 = 1 << 16,
        UserDefine12 = 1 << 17,
        UserDefine13 = 1 << 18,
        UserDefine14 = 1 << 19,
        UserDefine15 = 1 << 20,
        UserDefine16 = 1 << 21,
        UserDefine17 = 1 << 22,
        UserDefine18 = 1 << 23,
        UserDefine19 = 1 << 24,
        UserDefine20 = 1 << 25,
        UserDefine21 = 1 << 26,
        UserDefine22 = 1 << 27,
        UserDefine23 = 1 << 28,
        UserDefine24 = 1 << 29,
        UserDefine25 = 1 << 30,
        UserDefine26 = 1 << 31
    };

    namespace Api {
        class iGameObject;

        class iPhysxComponent {
        public:
            virtual ~iPhysxComponent() {}

            virtual iGameObject* GetGameObject() const = 0;

            virtual void SetActive(const bool b) = 0;
            virtual bool IsActive() const = 0;

            virtual void SetKinematic(const bool b) = 0;
            virtual bool IsKinematic() const = 0;

            virtual void SetCCD(const bool b) = 0;
            virtual bool IsCCD() const = 0;

            virtual void SetTrigger(const bool b) = 0;
            virtual bool IsTrigger() const = 0;

            virtual void SetUseGravity(const bool b) = 0;
            virtual bool IsUseGravity() const = 0;

            virtual void SetLayer(const eLayer layer) = 0;
            virtual eLayer GetLayer() const = 0;

            virtual void SetMass(const float mass) = 0;
            virtual void SetDrag(const float drag) = 0;
            virtual void SetAngularDrag(const float angularDrag) = 0;

            virtual void SetInterpolate(const eInterpolate type) = 0;
            virtual void SetCollisionDetection(const eCollisionDetection type) = 0;
        };


        class iPhysxScene {
        public:
            virtual ~iPhysxScene() {}

            virtual void RelationPhysicsLayer(const eLayer layerA, const eLayer layerB) = 0;

            virtual iPhysxComponent* CreatePlane(const float nx, const float ny, const float nz, const float distance, iGameObject* const gameObject) = 0;
            virtual iPhysxComponent* CreateBox(const eRigType type, const Vector3& pos, const Quaternion& qt, const Vector3& size, iGameObject* const gameObject) = 0;
            virtual iPhysxComponent* CreateCapsule(const eRigType type, const Vector3& pos, const Quaternion& qt, const float radius, const float height, iGameObject* const gameObject) = 0;
            virtual iPhysxComponent* CreateConvexMesh(const eRigType type, const Quaternion& qt, iGameObject* const gameObject) = 0;
            virtual iPhysxComponent* CreateTriangleMesh(
                const eRigType type,
                const Vector3& pos,
                const Quaternion& qt,
                const Vector3& scale,
                const X3DObj* obj,
                iGameObject* const gameObject
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
