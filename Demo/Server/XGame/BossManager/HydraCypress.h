#ifndef __HydraCypress_h__
#define __HydraCypress_h__

#include "Header.h"

class BossTimerData;

class HydraCypress : public IHydraCypress, public Api::iTimer {
public:
    virtual ~HydraCypress();
    
    HydraCypress() { _NextTimerId = 0; };

    IGameObject* CreateBossHydraCypress(IScene* const scene, const Int64 configId, const Vector2& pos) override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp);
    void OnEnd(const int id, const UInt64 context, bool nonviolent, const Int64 timestamp);
private:
    static void OnMonsterBossHpChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
    static bool OnMonsterBossHPChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
    void UpdateBossState(float newHpValue);
    const Int64 GetStateNormalAtkConfigID(Int32 index);
    const Int64* GetStateSkillConfigIDs();
    void StopTimersForBoss(UInt64 bossGuid, const std::vector<int>& timerTypes);
    void UpdateAtkTimer(IGameObject* boss, bool IsNeedStopOld = false);
    BossTimerData* GetSkillTimerData(UInt64 bossGuid, Int64 skillConfigId, int timerType);

    UInt64 GenerateTimerId();

private:
    std::unordered_map<UInt64, BossTimerData*> _TimerContexts;
    UInt64 _NextTimerId;
};                                     

#endif //__HydraCypress_h__
