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

    namespace Api {
        class iPhysxBase {
        public:
            virtual ~iPhysxBase() {}
            iPhysxBase(iPhysxContext* const context) : _Context(context) {}

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

            virtual void SetLayer(const int layer) = 0;
            virtual int GetLayer() const = 0;

            virtual void SetMass(const float mass) = 0;
            virtual void SetDrag(const float drag) = 0;
            virtual void SetAngularDrag(const float angularDrag) = 0;

            virtual void SetInterpolate(const eInterpolate type) = 0; //物理运动插值模式 建议服务器使用None
            virtual void SetCollisionDetection(const eCollisionDetection type) = 0;

            //virtual void UpdateMaterial()  更新物理材质

            virtual void SetPosition(const Vector3& position) = 0;
            virtual void SetRotation(const Vector3& rotation) = 0;

            virtual void Release() = 0;

            iPhysxContext* const _Context;
        };


        class iCollider {
        public:
            virtual ~iCollider() {}

            virtual iPhysxContext* const GetPhysxContext() const = 0;
        };

        class iCollision {
        public:
            virtual ~iCollision() {}

            virtual iPhysxContext* const GetPhysxContext() const = 0;
            virtual const Vector3& ClosestPoint() const = 0;
            virtual const Vector3& ClosestPointOnBounds() = 0;
        };

        class iPhysxContext {
        public:
            virtual ~iPhysxContext() {}

            virtual void OnPhysxCreated(bool success) = 0;
            virtual void OnPhysxAwake() = 0;
            virtual void OnPhysxRelease() = 0;


            virtual void SetPhysxActive(const bool b) { _PhysxBase ? _PhysxBase->SetActive(b) : void(0); }
            virtual bool IsPhysxActive() const { return _PhysxBase ? _PhysxBase->IsActive() : false; }

            virtual void SetKinematic(const bool b) { _PhysxBase ? _PhysxBase->SetKinematic(b) : void(0); }
            virtual bool IsKinematic() const { return _PhysxBase ? _PhysxBase->IsKinematic() : false; }

            virtual void SetCCD(const bool b) { _PhysxBase ? _PhysxBase->SetCCD(b) : void(0); };
            virtual bool IsCCD() const { return _PhysxBase ? _PhysxBase->IsCCD() : false; }

            virtual void SetTrigger(const bool b) { _PhysxBase ? _PhysxBase->SetTrigger(b) : void(0); }
            virtual bool IsTrigger() const { return _PhysxBase ? _PhysxBase->IsTrigger() : false; }

            virtual void SetUseGravity(const bool b) { _PhysxBase ? _PhysxBase->SetUseGravity(b) : void(0); }
            virtual bool IsUseGravity() const { return _PhysxBase ? _PhysxBase->IsUseGravity() : false; }

            virtual void SetLayer(const int layer) { _PhysxBase ? _PhysxBase->SetLayer(layer) : void(0); }
            virtual int GetLayer() const { return _PhysxBase ? _PhysxBase->GetLayer() : 0; }

            virtual void SetMass(const float mass) { _PhysxBase ? _PhysxBase->SetMass(mass) : void(0); }
            virtual void SetDrag(const float drag) { _PhysxBase ? _PhysxBase->SetDrag(drag) : void(0); }
            virtual void SetAngularDrag(const float angularDrag) { _PhysxBase ? _PhysxBase->SetAngularDrag(angularDrag) : void(0); }

            virtual void SetInterpolate(const eInterpolate type) { _PhysxBase ? _PhysxBase->SetInterpolate(type) : void(0); }
            virtual void SetCollisionDetection(const eCollisionDetection type) { _PhysxBase ? _PhysxBase->SetCollisionDetection(type) : void(0); }

            virtual void OnTriggerEnter(iCollider* const other) {}
            virtual void OnTriggerExit(iCollider* const other) {}

            virtual void OnCollisionEnter(iCollision* const other) = 0;
            virtual void OnCollisionExit(iCollision* const other) = 0;

            iPhysxContext(void* const data) : _Data(data), _PhysxBase(nullptr) {}

            template<typename T>
            T* GetData() { return dynamic_cast<T*>(_Data); }

            void* const _Data;
            iPhysxBase* const _PhysxBase;

            virtual Vector3 Position() {
                return _Position;
            }

            virtual Vector3 Rotation() {
                return _Rotation;
            }

            virtual void SetPosition(const Vector3& pos) {
                _Position = pos;
                _PhysxBase ? _PhysxBase->SetPosition(pos) : void(0);
            }

            virtual void SetRotation(const Vector3& r) {
                _Rotation = r;
                _PhysxBase ? _PhysxBase->SetRotation(r) : void(0);
            }

            virtual void ReleasePhysics() {
                _PhysxBase ? _PhysxBase->Release() : void(0);
            }

            virtual void UpdatePositionAndRotation(const Vector3& pos, const Vector3& r) {
                _Position = pos;
                _Rotation = r;
            }

        private:
            Vector3 _Position;
            Vector3 _Rotation;
        };


        class iPhysxScene {
        public:
            virtual ~iPhysxScene() {}

            virtual void RelationPhysicsLayer(const int layerA, const int layerB) = 0;

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
