#ifndef __BuffManager_h__
#define __BuffManager_h__

#include "Header.h"

class BuffManager : public IBuffManager, public Api::iTimer {
public:
    virtual ~BuffManager() {}
private:
    // Í¨¹ý IBuffManager ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    UInt64 AddBuff(IGameObject* const caster, IGameObject* const entity, const Int64 buffConfigID) override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

private:
    static void OnBuff(IGameObject* const buffGo);
    static void OffBuff(IGameObject* const buffGo);

    static void OnGameEntityCreate(IGameObject* const go);
    static void OnGameEntityRelease(IGameObject* const go);

    static void ReleaseBuff(IGameObject* buffGo);                       
    static void OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState state);
    static void OnBuffTest(IPlayer* const player, const UInt64 account, const oBuffTest& req);

    static void OnEnterDeath(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& pos, const float angle);

    static bool OnExitStunedJudge(const UInt64 guid, const AutoConfig::Enum::eState, const Vector2&, const float);
    static bool OnCantControllerRefChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue);
    static void OnCantControllerRefChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue);

    static void OnAppear(IScene* const, const UInt64 guid, const std::vector<UInt64>&);
    static void OnDisappear(IScene* const, const UInt64 guid, const std::vector<UInt64>&);

    static bool OnInvincibleRefChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const Int64 oldValue, const Int64 newValue);
};

#endif //__BuffManager_h__
