#ifndef __SceneManager_h__
#define __SceneManager_h__

#include "Header.h"

class Scene;
class SceneManager : public ISceneManager, public Api::iTimer {
public:
    virtual ~SceneManager() {}

    void CreateCopyScene(const Int64 configId, const fCreateCopySceneCallback& callback)  override;
    void ReleaseCopyScene(IScene* const scene) override;

    IScene* QueryCopyScene(IGameObject* const character) const override;
    IScene* QueryPublicScene(const Int64 configId) const override;
    IScene* QuerySceneByEntityGuid(const UInt64 guid) const override;
    IScene* QuerySceneBySceneID(const UInt64 sceneGuid) const override;

    // 通过 ISceneManager 继承
    void RegistSceneEvent(const eSceneEvent ev, const fSceneEvent& callback) override;

private:
    static void OnExitCopyScene(IPlayer* const nothing, const UInt64 account, const oExitCopyScene& body);

private:
    Scene* QuerySceneByGuid(const UInt64 sceneGuid) const;
    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

private:
    // 通过 ISceneManager 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;
};

#endif //__SceneManager_h__