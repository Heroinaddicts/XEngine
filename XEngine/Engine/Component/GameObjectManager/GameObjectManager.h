#ifndef __GameObjectManager_h__
#define __GameObjectManager_h__

#include "Header.h"

namespace XEngine {
    class GameObject;
    class GameObjectManager : public iGameObjectManager {
    public:
        virtual ~GameObjectManager() {}

        // Í¨¹ý iGameObjectManager ¼Ì³Ð
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;
        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void FixedUpdate(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        virtual Api::iGameObject* CreateGameObject(const char* file, const int line) override;
        virtual void DestroyGameObject(Api::iGameObject* gameObject) override;
    };
}

#endif //__GameObjectManager_h__
