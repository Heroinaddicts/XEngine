#ifndef __Physics_h__
#define __Physics_h__

#include "Header.h"
#include <map>
namespace XEngine {
    class PhysxScene;
    class Physics : public iPhysics {
    public:
        virtual ~Physics() {}

        static iPhysics* GetInstance();

        // 通过 iPhysics 继承
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;

        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void FixedUpdate(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        virtual Api::iPhysxScene* CreateScene(const float static_friction, const float dynamic_friction, const float restitution);
        virtual void ReleaseScene(Api::iPhysxScene* scene);
    private:
        Physics() {}
    };
}

#endif //__Physics_h__
