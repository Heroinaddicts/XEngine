#ifndef __BossManager_h__
#define __BossManager_h__

#include "Header.h"

class BossManager : public IBossManager {
public:
    virtual ~BossManager() {}

    IGameObject* CreateBoss(IScene* const scene, const Int64 configId, const Vector2& pos, const fInitBoss& initer) override;
private:
    // Í¨¹ý IBossManager ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    IHydraCypress* CreateHydraCypress(IScene* const scene, const Int64 configId, const Vector2& pos);

    static void OnSceneCreate(IScene* const scene);

    static void OnSceneRelease(IScene* const scene);
};

#endif //__BossManager_h__
