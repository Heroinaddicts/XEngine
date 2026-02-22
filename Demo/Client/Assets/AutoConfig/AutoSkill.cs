using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Skill
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Name;
        public readonly string _Desc;
        public readonly AutoEnum.eSkillEffect[] _SkillEffect;
        public readonly Int64[] _SkillEffectConfigID;
        public readonly string _SkillIcon;
        public readonly bool _CanEquip;
        public readonly string _UINode;
        public readonly Int64 _CoolDown;
        public readonly Int64 _SpecialEffects;
        public readonly AutoEnum.eSelectedType _SelectedType;
        public readonly AutoEnum.eTargetType[] _TargetType;
        public readonly bool _StudyOrNot;
        public readonly Int64 _CharacterLevel;
        public readonly Int64 _UpInterval;
        public readonly Int64[] _SkillCommand;
        public readonly AutoEnum.eConsumeType _Consume;
        public readonly Int64 _ConsumeCount;
        public readonly Int64 _MaxSkillDistacne;
        public readonly string _MaxSheetsPrefab;
        public readonly float _MaxRangeRadius;
        public readonly string _UseSheetsPrefab;
        public readonly AutoEnum.eSheetsType _SheetsType;
        public readonly float[] _RangeParameter;
        public readonly bool _CanSeparated;
        public readonly string _ActPrefab;
        public readonly Int64 _ActTime;
        public readonly Int64 _ShowRangeTime;
        public readonly bool _ActIsLoop;
        public readonly bool _IsNromalAtk;
        public readonly bool _IsPost;
        public readonly string _PostName;
        public readonly bool _CanMove;
        public readonly Int64 _CantMoveTime;
        public readonly Int64 _Sound;
        public readonly Int64 _SpecialEffectsDelay;
        public readonly Int64 _EffectCycleLife;
        public readonly bool _IsEffectLoop;
        public readonly Int64 _SkillInterval;
        public readonly float _SheetsMove;
        public readonly Int64 _ConsumeSkill;
        public readonly bool _IsAtkSheets;
        public Skill(Int64 ID, string Name, string Desc, AutoEnum.eSkillEffect[] SkillEffect, Int64[] SkillEffectConfigID, string SkillIcon, bool CanEquip, string UINode, Int64 CoolDown, Int64 SpecialEffects, AutoEnum.eSelectedType SelectedType, AutoEnum.eTargetType[] TargetType, bool StudyOrNot, Int64 CharacterLevel, Int64 UpInterval, Int64[] SkillCommand, AutoEnum.eConsumeType Consume, Int64 ConsumeCount, Int64 MaxSkillDistacne, string MaxSheetsPrefab, float MaxRangeRadius, string UseSheetsPrefab, AutoEnum.eSheetsType SheetsType, float[] RangeParameter, bool CanSeparated, string ActPrefab, Int64 ActTime, Int64 ShowRangeTime, bool ActIsLoop, bool IsNromalAtk, bool IsPost, string PostName, bool CanMove, Int64 CantMoveTime, Int64 Sound, Int64 SpecialEffectsDelay, Int64 EffectCycleLife, bool IsEffectLoop, Int64 SkillInterval, float SheetsMove, Int64 ConsumeSkill, bool IsAtkSheets)
        {
            _ID = ID;
            _Name = Name;
            _Desc = Desc;
            _SkillEffect = SkillEffect;
            _SkillEffectConfigID = SkillEffectConfigID;
            _SkillIcon = SkillIcon;
            _CanEquip = CanEquip;
            _UINode = UINode;
            _CoolDown = CoolDown;
            _SpecialEffects = SpecialEffects;
            _SelectedType = SelectedType;
            _TargetType = TargetType;
            _StudyOrNot = StudyOrNot;
            _CharacterLevel = CharacterLevel;
            _UpInterval = UpInterval;
            _SkillCommand = SkillCommand;
            _Consume = Consume;
            _ConsumeCount = ConsumeCount;
            _MaxSkillDistacne = MaxSkillDistacne;
            _MaxSheetsPrefab = MaxSheetsPrefab;
            _MaxRangeRadius = MaxRangeRadius;
            _UseSheetsPrefab = UseSheetsPrefab;
            _SheetsType = SheetsType;
            _RangeParameter = RangeParameter;
            _CanSeparated = CanSeparated;
            _ActPrefab = ActPrefab;
            _ActTime = ActTime;
            _ShowRangeTime = ShowRangeTime;
            _ActIsLoop = ActIsLoop;
            _IsNromalAtk = IsNromalAtk;
            _IsPost = IsPost;
            _PostName = PostName;
            _CanMove = CanMove;
            _CantMoveTime = CantMoveTime;
            _Sound = Sound;
            _SpecialEffectsDelay = SpecialEffectsDelay;
            _EffectCycleLife = EffectCycleLife;
            _IsEffectLoop = IsEffectLoop;
            _SkillInterval = SkillInterval;
            _SheetsMove = SheetsMove;
            _ConsumeSkill = ConsumeSkill;
            _IsAtkSheets = IsAtkSheets;
        }
        public readonly static ReadOnlyDictionary<Int64, Skill> Datas = new ReadOnlyDictionary<Int64, Skill>(
            new Dictionary<Int64, Skill>() {
                {11001, new Skill(11001, "踢腿", "向前快速踢腿，对敌人造成伤害同时造成流血效果", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {4}, "UI/Sprites/Skill/Skill9_Wx", true, "UISkill1", 9500, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 20, 10, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {4.7f,11f}, false, "xiaoge-attack-A", 850, 254, false, false, false, "", false, 880, 4007, 20, 100, false, 765, 0.2f, 1, false)},
                {11002, new Skill(11002, "血阵", "释放圆形血阵，对敌人造成大量伤害", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {6}, "UI/Sprites/Skill/Skill1_Wx", true, "UISkill2", 15000, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 30, 12, "UI/Sprites/Character/TrueHead", 23.5f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Circle, new float[] {5.65f}, true, "xiaoge-attack-B", 750, 251, false, false, false, "", false, 790, 4005, 21, 101, false, 675, 13f, 1, false)},
                {11003, new Skill(11003, "疾步", "向前快速滑行一段距离", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Move}, new Int64[] {1002}, "UI/Sprites/Skill/Skill6_Wx", true, "UISkill3", 7000, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 25, 10, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {3f,13.5f}, false, "chongci", 545, 252, false, false, false, "", false, 665, 4008, 22, 102, false, 570, 0.6f, 1, false)},
                {11004, new Skill(11004, "鬼哨", "对敌人造成伤害同时减速敌人", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher,AutoEnum.eSkillEffect.Launcher}, new Int64[] {5,30}, "UI/Sprites/Skill/Skill8_Wx", true, "UISkill4", 18500, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {11004}, AutoEnum.eConsumeType.Mp, 35, 10, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {6.5f,13f}, false, "xiaoge-attack-C", 1450, 253, false, false, false, "", false, 1650, 4006, 24, 104, false, 1480, 1f, 1, false)},
                {11005, new Skill(11005, "小哥跟踪技能测试", "技能描述测试", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Move}, new Int64[] {1001}, "UI/Sprites/Skill/Skill7_Wx", true, "", 1503, 0, AutoEnum.eSelectedType.Aim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {11003}, AutoEnum.eConsumeType.Mp, 18, 10, "UI/Sprites/Character/TrueHead", 20f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Circle, new float[] {1f}, true, "x", 103, 253, false, false, false, "", false, 103, 4008, 23, 103, false, 75, 0.8f, 1, false)},
                {11006, new Skill(11006, "技能测试普攻1", "技能描述测试", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {8}, "UI/Sprites/Bag/WeaponSkill1", false, "", 775, 1016, AutoEnum.eSelectedType.Liberty, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {11005}, AutoEnum.eConsumeType.Mp, 0, 10, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {2f,13f}, false, "xiaoge-attack3", 735, 250, false, true, false, "", false, 735, 4001, 25, 300, false, 675, 0.5f, 1, true)},
                {11007, new Skill(11007, "技能测试普攻2", "技能描述测试", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {9}, "UI/Sprites/Bag/WeaponSkill1", false, "", 705, 1016, AutoEnum.eSelectedType.Liberty, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {11006}, AutoEnum.eConsumeType.Mp, 0, 10, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {2f,13f}, false, "xiaoge-attack1", 500, 250, false, true, false, "", false, 705, 4002, 26, 300, false, 475, 0.5f, 2, false)},
                {11008, new Skill(11008, "技能测试普攻3", "技能描述测试", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {10}, "UI/Sprites/Bag/WeaponSkill1", false, "", 1090, 1015, AutoEnum.eSelectedType.Liberty, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {11007}, AutoEnum.eConsumeType.Mp, 0, 10, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {2f,13f}, false, "xiaoge-attack5", 975, 250, false, true, false, "", false, 1090, 4003, 550, 300, true, 965, 0.5f, 2, false)},
                {11009, new Skill(11009, "潘子剑雨", "潘子剑雨", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {25}, "UI/Sprites/Skill/Skill5_Wx", true, "UISkill2", 15000, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 30, 11, "UI/Sprites/Character/TrueHead", 22f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Circle, new float[] {5.5f}, true, "attack1", 1200, 250, false, false, false, "", false, 1220, 4022, 20, 100, false, 1205, 13f, 1, true)},
                {11010, new Skill(11010, "潘子爆裂箭", "潘子爆裂箭", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher,AutoEnum.eSkillEffect.Launcher,AutoEnum.eSkillEffect.Launcher,AutoEnum.eSkillEffect.Launcher,AutoEnum.eSkillEffect.Launcher}, new Int64[] {26,31,32,33,34}, "UI/Sprites/Skill/Skill8_Wx8", true, "UISkill3", 20000, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 40, 16, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {3.5f,15f}, false, "attack3", 1150, 250, false, false, false, "", false, 1205, 4021, 20, 100, false, 1150, 0.8f, 1, true)},
                {11011, new Skill(11011, "潘子向后位移", "潘子向后位移向前射箭", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Move,AutoEnum.eSkillEffect.Launcher}, new Int64[] {1007,29}, "UI/Sprites/Skill/Skill4_Wx", true, "UISkill1", 8500, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 25, 16, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {6.5f,16f}, false, "attack2", 750, 252, false, false, false, "", false, 775, 4023, 22, 102, false, 750, 0.6f, 1, false)},
                {11012, new Skill(11012, "潘子加buff", "潘子加buff", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {27}, "UI/Sprites/Skill/Skill10_Wx", true, "UISkill4", 15000, 0, AutoEnum.eSelectedType.Liberty, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 20, 10, "UI/Sprites/Character/TrueHead", 2f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {2.5f,19f}, false, "attack1", 10, 250, false, false, false, "", false, 20, 4024, 20, 100, false, 15, 0.8f, 1, false)},
                {11013, new Skill(11013, "潘子普攻", "测试潘子普攻", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {24}, "UI/Sprites/Skill/Skill9_Wx", false, "", 1000, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 0, 18, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {2.5f,19f}, false, "attack", 650, 250, false, false, false, "", false, 680, 4020, 20, 100, false, 625, 0.8f, 1, true)},
                {11014, new Skill(11014, "潘子普攻", "测试潘子普攻", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {24}, "UI/Sprites/Skill/Skill9_Wx", false, "", 1000, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 0, 18, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {2.5f,19f}, false, "attack", 650, 250, false, false, false, "", false, 680, 4020, 20, 100, false, 625, 0.8f, 1, true)},
                {11015, new Skill(11015, "潘子普攻", "测试潘子普攻", new AutoEnum.eSkillEffect[] {AutoEnum.eSkillEffect.Launcher}, new Int64[] {24}, "UI/Sprites/Skill/Skill9_Wx", false, "", 1000, 0, AutoEnum.eSelectedType.UnAim, new AutoEnum.eTargetType[] {AutoEnum.eTargetType.Monster,AutoEnum.eTargetType.Boss,AutoEnum.eTargetType.EnemyCharacter}, false, 1, 3, new Int64[] {0}, AutoEnum.eConsumeType.Mp, 0, 10, "UI/Sprites/Character/TrueHead", 0f, "UI/Sprites/Character/TrueHead", AutoEnum.eSheetsType.Rectangle, new float[] {2f,13f}, false, "attack", 650, 250, false, false, false, "", false, 680, 4020, 20, 100, false, 625, 0.8f, 1, true)},
            }
        );
        public static Skill Get(Int64 id) {
            Skill config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
