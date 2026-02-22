using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class SpecialEffects
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly string _Prefab;
        public readonly AutoEnum.ePosition _EffectPositionType;
        public readonly Int64 _Time;
        public SpecialEffects(Int64 ID, string Des, string Prefab, AutoEnum.ePosition EffectPositionType, Int64 Time)
        {
            _ID = ID;
            _Des = Des;
            _Prefab = Prefab;
            _EffectPositionType = EffectPositionType;
            _Time = Time;
        }
        public readonly static ReadOnlyDictionary<Int64, SpecialEffects> Datas = new ReadOnlyDictionary<Int64, SpecialEffects>(
            new Dictionary<Int64, SpecialEffects>() {
                {1001, new SpecialEffects(1001, "某某特效", "Effect/Prefabs/Environment/Fire/Cartoon/Pz_Skill1Xuli", AutoEnum.ePosition.MountBone, 45)},
                {1002, new SpecialEffects(1002, "某某特效", "Effect/Prefabs/Environment/Fire/Cartoon/StarFogFlowRed", AutoEnum.ePosition.MountBone, 46)},
                {1003, new SpecialEffects(1003, "某某特效", "Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill2", AutoEnum.ePosition.MountBone, 47)},
                {1004, new SpecialEffects(1004, "某某特效", "Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill2", AutoEnum.ePosition.MountBone, 48)},
                {1005, new SpecialEffects(1005, "某某特效", "Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill3", AutoEnum.ePosition.None, 49)},
                {1006, new SpecialEffects(1006, "直线子弹特效", "Effect/Prefabs/Environment/Fire/Cartoon/PowerupGlow2", AutoEnum.ePosition.None, 0)},
                {1007, new SpecialEffects(1007, "抛物线子弹", "Effect/Prefabs/Environment/Fire/Cartoon/SpinPortalGold", AutoEnum.ePosition.None, 0)},
                {1008, new SpecialEffects(1008, "跟踪弹", "Effect/Prefabs/Environment/Fire/Cartoon/PowerupGlow2", AutoEnum.ePosition.None, 0)},
                {1009, new SpecialEffects(1009, "直线子弹爆炸", "Effect/Prefabs/Environment/Fire/Cartoon/Dead1", AutoEnum.ePosition.None, 500)},
                {1010, new SpecialEffects(1010, "抛物线爆炸", "Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill1_2", AutoEnum.ePosition.None, 750)},
                {1011, new SpecialEffects(1011, "跟踪弹爆炸", "Effect/Prefabs/Environment/Fire/Cartoon/Dead1", AutoEnum.ePosition.None, 500)},
                {1012, new SpecialEffects(1012, "直线弹道拖尾特效", "Effect/Prefabs/Environment/Fire/Cartoon/SoftFireAdditiveBlue", AutoEnum.ePosition.None, 0)},
                {1013, new SpecialEffects(1013, "抛物线拖尾", "Effect/Prefabs/Environment/Fire/Cartoon/SoftFireAdditiveBlue", AutoEnum.ePosition.None, 0)},
                {1014, new SpecialEffects(1014, "跟踪弹拖尾", "Effect/Prefabs/Environment/Fire/Cartoon/SoftFireAdditiveBlue", AutoEnum.ePosition.None, 0)},
                {1015, new SpecialEffects(1015, "普攻特效3时长", "Spine/VFX/Prefabs/Atk11", AutoEnum.ePosition.WorldPosition, 800)},
                {1016, new SpecialEffects(1016, "普攻特效1时长", "Spine/VFX/Prefabs/Atk22", AutoEnum.ePosition.WorldPosition, 700)},
                {1017, new SpecialEffects(1017, "藤曼消失特效", "Spine/VFX/Monster/Prefabs/JiuTouSheBai/BundingBack", AutoEnum.ePosition.WorldPosition, 830)},
                {1018, new SpecialEffects(1018, "藤曼受击特效1", "Spine/VFX/Monster/Prefabs/JiuTouSheBai/BeAttack1", AutoEnum.ePosition.MountBone, 285)},
                {1019, new SpecialEffects(1019, "Boss受击特效1", "Spine/VFX/Monster/Prefabs/JiuTouSheBai/BeAttack2", AutoEnum.ePosition.MountBone, 285)},
                {1020, new SpecialEffects(1020, "声波眩晕", "Effect/Prefabs/Environment/Fire/Cartoon/SpinPortalGold1", AutoEnum.ePosition.MountBone, 300)},
                {1021, new SpecialEffects(1021, "减速", "Effect/Prefabs/Environment/Fire/Cartoon/Pz_Skill2Xuli", AutoEnum.ePosition.MountBone, 300)},
                {1022, new SpecialEffects(1022, "持续流血", "Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill2_1", AutoEnum.ePosition.MountBone, 300)},
                {1023, new SpecialEffects(1023, "测试刀光", "Spine/VFX/Monster/Prefabs/XueShi/XueShiAtk", AutoEnum.ePosition.MountBone, 350)},
                {1024, new SpecialEffects(1024, "持续流血大量", "Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill2_2", AutoEnum.ePosition.MountBone, 300)},
                {1025, new SpecialEffects(1025, "潘子加移速", "Spine/VFX/Prefabs/PanZi/Buff", AutoEnum.ePosition.MountBone, 300)},
                {1026, new SpecialEffects(1026, "爆裂箭爆炸", "Spine/VFX/Bullet/Boom1", AutoEnum.ePosition.MountBone, 350)},
                {1027, new SpecialEffects(1027, "低级升级特效", "Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill5", AutoEnum.ePosition.MountBone, 650)},
            }
        );
        public static SpecialEffects Get(Int64 id) {
            SpecialEffects config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
