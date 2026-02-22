using System;
using Unity.Burst.CompilerServices;
using UnityEngine;
using XEngine;
using XGame;

public interface IOptimizationOfOperationalFeel : IModule
{
    public class OptimizationSkillInfo
    {
        public UInt64 SkillID = 0;
        public Int64 skillConfigid = 0;
        public float angle = 0;
        public oVector2 targetpos = new oVector2();
        public UInt64 targetGuid = 0;
        public oVector2 castPos = new oVector2();
    }

    void OptimizationCastSkill(in UInt64 canCastTick, in OptimizationSkillInfo info, in Action<OptimizationSkillInfo> callback);
}

class OptimizationOfOperationalFeel : IOptimizationOfOperationalFeel
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]

    static void Regist()
    {
        Launcher.RegistModule<IOptimizationOfOperationalFeel, OptimizationOfOperationalFeel>();
    }
    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        return true;
    }

    public void Release()
    {

    }

    IOptimizationOfOperationalFeel.OptimizationSkillInfo _LastOptimizationSkillInfo = null;

    void OnOptimizationCastSkill(in XApi.ITimer.eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
    {
        if (state == XApi.ITimer.eState.BEAT)
        {
            Action<IOptimizationOfOperationalFeel.OptimizationSkillInfo> callback = context as Action<IOptimizationOfOperationalFeel.OptimizationSkillInfo>;
            callback(_LastOptimizationSkillInfo);
            //Debug.LogError($"队列技能释放");
        }

        if (state == XApi.ITimer.eState.END)
        {
            _LastOptimizationSkillInfo = null;
            //Debug.LogError($"队列技能释放结束");
        }
    }

    public void OptimizationCastSkill(in UInt64 canCastTick, in IOptimizationOfOperationalFeel.OptimizationSkillInfo info, in Action<IOptimizationOfOperationalFeel.OptimizationSkillInfo> callback)
    {
        if (null != _LastOptimizationSkillInfo)
        {
            XApi.TimerApi.StopTimer(OnOptimizationCastSkill, _LastOptimizationSkillInfo);
            _LastOptimizationSkillInfo = null;
        }

        if (canCastTick > (UInt64)AutoConfig.AutoConst.BattleSetting.SkillTolerance)
        {
            //Debug.LogError($"技能{info.skillConfigid} 技能间隔太短: {canCastTick}");
            return;
        }

        _LastOptimizationSkillInfo = info;
        XApi.TimerApi.StartTimer(OnOptimizationCastSkill, _LastOptimizationSkillInfo, callback, (int)canCastTick, 1, (int)canCastTick);
       //Debug.LogError($"技能进入优化释放，等待时间:{canCastTick}");
    }
}
