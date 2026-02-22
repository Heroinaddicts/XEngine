using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Buff
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly AutoEnum.eAttribute _Attribute;
        public readonly string _Name;
        public readonly float _EffectParameter;
        public readonly Int64 _Delay;
        public readonly Int64 _LifeCycle;
        public readonly Int64 _Interval;
        public readonly Int64 _MaxStackCount;
        public readonly string _Material;
        public readonly bool _IfRfresh;
        public readonly AutoEnum.eCalculationMethod _CalculationMethod;
        public readonly AutoEnum.eDamageType _DamageType;
        public readonly Int64 _SpecialEffects;
        public readonly bool _ProgressBar;
        public readonly AutoEnum.eAddType _AddType;
        public readonly AutoEnum.eTargetType _eBuffTargetConfig;
        public readonly bool _IsBuffAct;
        public readonly string _BuffAct;
        public readonly bool _actIsLoop;
        public readonly bool _IsHostDisappear;
        public readonly bool _CanAct;
        public readonly bool _CanCamera;
        public readonly Int64 _CameraDisatance;
        public readonly Int64 _CameraTime;
        public readonly Int64 _CameraRecover;
        public Buff(Int64 ID, AutoEnum.eAttribute Attribute, string Name, float EffectParameter, Int64 Delay, Int64 LifeCycle, Int64 Interval, Int64 MaxStackCount, string Material, bool IfRfresh, AutoEnum.eCalculationMethod CalculationMethod, AutoEnum.eDamageType DamageType, Int64 SpecialEffects, bool ProgressBar, AutoEnum.eAddType AddType, AutoEnum.eTargetType eBuffTargetConfig, bool IsBuffAct, string BuffAct, bool actIsLoop, bool IsHostDisappear, bool CanAct, bool CanCamera, Int64 CameraDisatance, Int64 CameraTime, Int64 CameraRecover)
        {
            _ID = ID;
            _Attribute = Attribute;
            _Name = Name;
            _EffectParameter = EffectParameter;
            _Delay = Delay;
            _LifeCycle = LifeCycle;
            _Interval = Interval;
            _MaxStackCount = MaxStackCount;
            _Material = Material;
            _IfRfresh = IfRfresh;
            _CalculationMethod = CalculationMethod;
            _DamageType = DamageType;
            _SpecialEffects = SpecialEffects;
            _ProgressBar = ProgressBar;
            _AddType = AddType;
            _eBuffTargetConfig = eBuffTargetConfig;
            _IsBuffAct = IsBuffAct;
            _BuffAct = BuffAct;
            _actIsLoop = actIsLoop;
            _IsHostDisappear = IsHostDisappear;
            _CanAct = CanAct;
            _CanCamera = CanCamera;
            _CameraDisatance = CameraDisatance;
            _CameraTime = CameraTime;
            _CameraRecover = CameraRecover;
        }
        public readonly static ReadOnlyDictionary<Int64, Buff> Datas = new ReadOnlyDictionary<Int64, Buff>(
            new Dictionary<Int64, Buff>() {
                {1001, new Buff(1001, AutoEnum.eAttribute.CantController, "眩晕束缚", 1f, 10, 7200, 0, 1, "UI/Sprites/Battle/Buff/BundBuff", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1020, true, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Character, true, "Entangle", true, true, false, true, 5, 1200, 1200)},
                {1002, new Buff(1002, AutoEnum.eAttribute.Invincible, "无敌（免疫伤害）", 1f, 0, 15000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1002, false, AutoEnum.eAddType.Current, AutoEnum.eTargetType.PlayerSelf, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1003, new Buff(1003, AutoEnum.eAttribute.Hp, "加血", 0.1f, 0, 2000, 1000, 1, "UI/Sprites/Character/TrueHead", false, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1003, false, AutoEnum.eAddType.Max, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1004, new Buff(1004, AutoEnum.eAttribute.MoveSpeed, "减速", -0.45f, 0, 2000, 0, 1, "UI/Sprites/Battle/Buff/DesSpeed", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1004, false, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, true, "xx", false, false, true, false, 12, 1200, 1200)},
                {1005, new Buff(1005, AutoEnum.eAttribute.Mockery, "嘲讽", 1f, 0, 3000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1005, false, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1006, new Buff(1006, AutoEnum.eAttribute.ExpAdd, "加经验", 1.1f, 0, 0, 10, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1001, false, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1007, new Buff(1007, AutoEnum.eAttribute.Critical, "眩晕", 0.1f, 0, 4500, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1002, true, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1008, new Buff(1008, AutoEnum.eAttribute.Blood, "加吸血", 0.01f, 0, 2000, 0, 1, "UI/Sprites/Character/TrueHead", false, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1003, false, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1009, new Buff(1009, AutoEnum.eAttribute.AddDefend, "加物理防御", 0.01f, 0, 2000, 0, 1, "UI/Sprites/Character/TrueHead", false, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1004, false, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1010, new Buff(1010, AutoEnum.eAttribute.AddMagicDefend, "魔法防御", -0.1f, 0, 2000, 0, 1, "UI/Sprites/Character/TrueHead", false, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1005, false, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1011, new Buff(1011, AutoEnum.eAttribute.Mp, "蓝值", 3f, 0, 5000, 1000, 1, "UI/Sprites/Character/TrueHead", false, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1001, false, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1012, new Buff(1012, AutoEnum.eAttribute.ExpandVield, "扩大视野范围", 100f, 0, 4000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1002, false, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1013, new Buff(1013, AutoEnum.eAttribute.SuperArmor, "霸体（免疫控制）", 150f, 0, 3000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1003, false, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1014, new Buff(1014, AutoEnum.eAttribute.AtkSpeed, "攻击速度", 150f, 0, 1000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1004, false, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1015, new Buff(1015, AutoEnum.eAttribute.Restraint, "束缚", 0f, 0, 2000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1005, true, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1016, new Buff(1016, AutoEnum.eAttribute.Silent, "沉默", 0f, 0, 2000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1001, false, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1017, new Buff(1017, AutoEnum.eAttribute.Attack, "攻击力", 1.5f, 0, 2000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1023, false, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1018, new Buff(1018, AutoEnum.eAttribute.MoveSpeed, "移动速度", 0f, 0, 2000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1003, false, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1019, new Buff(1019, AutoEnum.eAttribute.CriticalAtk, "暴击伤害", 0.5f, 0, 1000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Normal, AutoEnum.eDamageType.Physic, 1004, false, AutoEnum.eAddType.None, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1020, new Buff(1020, AutoEnum.eAttribute.CantController, "声波眩晕", 1f, 10, 1500, 0, 1, "UI/Sprites/Battle/Buff/Dizzy", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1020, true, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Character, false, "xx", false, true, false, false, 12, 1200, 1200)},
                {1021, new Buff(1021, AutoEnum.eAttribute.MoveSpeed, "声波减速", -0.4f, 1, 4500, 0, 1, "UI/Sprites/Battle/Buff/DesSpeed", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1021, true, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1022, new Buff(1022, AutoEnum.eAttribute.CantController, "声波眩晕", 1f, 10, 3500, 0, 1, "UI/Sprites/Battle/Buff/Dizzy", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1020, true, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Character, false, "xx", false, true, false, false, 12, 1200, 1200)},
                {1023, new Buff(1023, AutoEnum.eAttribute.Hp, "九头蛇柏缠绕扣血", -0.04f, 0, 7200, 1200, 1, "UI/Sprites/Battle/Buff/BloodBuff", false, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1024, false, AutoEnum.eAddType.Max, AutoEnum.eTargetType.Monster, false, "xx", false, true, true, false, 12, 1200, 1200)},
                {1024, new Buff(1024, AutoEnum.eAttribute.MoveSpeed, "减速", -0.35f, 0, 2500, 0, 1, "UI/Sprites/Battle/Buff/DesSpeed", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1021, true, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, false, "xx", true, false, true, false, 12, 1200, 1200)},
                {1025, new Buff(1025, AutoEnum.eAttribute.Hp, "踢腿持续流血", -0.02f, 0, 2400, 1800, 1, "UI/Sprites/Battle/Buff/BloodBuff", false, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1024, false, AutoEnum.eAddType.Max, AutoEnum.eTargetType.Monster, false, "xx", false, true, true, false, 12, 1200, 1200)},
                {1026, new Buff(1026, AutoEnum.eAttribute.Hp, "血尸攻击流血", -0.02f, 0, 2000, 1000, 1, "UI/Sprites/Battle/Buff/BloodBuff", false, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1022, false, AutoEnum.eAddType.Max, AutoEnum.eTargetType.Monster, false, "xx", false, true, true, false, 12, 1200, 1200)},
                {1027, new Buff(1027, AutoEnum.eAttribute.Attack, "潘子加攻击力", 0.4f, 0, 6000, 0, 1, "UI/Sprites/Character/TrueHead", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1025, false, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
                {1028, new Buff(1028, AutoEnum.eAttribute.MoveSpeed, "潘子加移速", 0.3f, 0, 6000, 0, 1, "UI/Sprites/Battle/Buff/DesSpeed", true, AutoEnum.eCalculationMethod.Percent, AutoEnum.eDamageType.Physic, 1025, false, AutoEnum.eAddType.Current, AutoEnum.eTargetType.Monster, false, "xx", false, false, true, false, 12, 1200, 1200)},
            }
        );
        public static Buff Get(Int64 id) {
            Buff config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
