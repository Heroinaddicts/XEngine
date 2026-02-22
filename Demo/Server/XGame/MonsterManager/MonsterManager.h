#ifndef __MonsterManager_h__
#define __MonsterManager_h__

#include "Header.h"

class MonsterManager : public IMonsterManager, public Api::iTimer {
public:
    virtual ~MonsterManager() {}
private:
    // 通过 IMonsterManager 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;


private:
    static void MonsterGoBackToBronPosition(IGameObject* const monster);

private:
    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

private:
    static void OnSceneCreate(IScene* const scene);
    static void OnSceneRelease(IScene* const scene);

    // 通过 IMonsterManager 继承
    IGameObject* CreateMonster(IScene* const scene, const Int64 configId, const Vector2& pos, const fInitMonster& initer) override;

    static void OnMonsterHPChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
    static bool OnMonsterHPChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
};

#endif //__MonsterManager_h__
