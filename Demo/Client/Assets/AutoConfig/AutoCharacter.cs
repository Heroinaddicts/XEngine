using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Character
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Name;
        public readonly string _Prefab;
        public readonly float _InitialAtk;
        public readonly float _InitialSpeed;
        public readonly float _InitialHp;
        public readonly float _InitialCritical;
        public readonly float _InitialCriticalDamage;
        public readonly float _InitialDefend;
        public readonly float _InitialMagicDefend;
        public readonly float _InitialBlood;
        public readonly float _InitialMagicAttack;
        public readonly float _InitialPhysicalPierce;
        public readonly float _InitialMagicPierce;
        public readonly float _InitialAfford;
        public readonly float _InitialDodge;
        public readonly float _InitialResilience;
        public readonly Int64 _CharacterStrengthen;
        public readonly float _VisualField;
        public readonly Int64 _CharacterLevel;
        public readonly float _InitialMp;
        public readonly float _InitialHpReserve;
        public readonly float _InitialMpReserve;
        public readonly Int64 _InitialWeapon;
        public readonly string _PrefabShow;
        public readonly Int64[] _Skill;
        public readonly Int64[] _InitialSkill;
        public readonly string _Icon;
        public readonly Int64 _Atk1;
        public readonly Int64 _Atk2;
        public readonly Int64 _Atk3;
        public readonly string _DirIcon;
        public readonly string _CharacterLogo;
        public readonly AutoEnum.eResources _Resource;
        public readonly Int64 _Count;
        public readonly float _InitialAtkSpeed1;
        public readonly float _PhysicalPierceRate;
        public readonly float _MagicPierceRate;
        public readonly float _AtkInteval;
        public readonly float _InitialFireDefend;
        public readonly float _InitialWaterDefend;
        public readonly Int64 _InitialAtkSpeed2;
        public readonly Int64 _InitialAtkSpeed3;
        public readonly float _CharacterBody;
        public readonly AutoEnum.eDamagePriority _DamagePriority;
        public readonly string _DeadAct;
        public readonly Int64 _DeadSound;
        public readonly Int64 _HpReserveSpeed;
        public readonly Int64 _MpReserveSpeed;
        public readonly float _CircleArea;
        public Character(Int64 ID, string Name, string Prefab, float InitialAtk, float InitialSpeed, float InitialHp, float InitialCritical, float InitialCriticalDamage, float InitialDefend, float InitialMagicDefend, float InitialBlood, float InitialMagicAttack, float InitialPhysicalPierce, float InitialMagicPierce, float InitialAfford, float InitialDodge, float InitialResilience, Int64 CharacterStrengthen, float VisualField, Int64 CharacterLevel, float InitialMp, float InitialHpReserve, float InitialMpReserve, Int64 InitialWeapon, string PrefabShow, Int64[] Skill, Int64[] InitialSkill, string Icon, Int64 Atk1, Int64 Atk2, Int64 Atk3, string DirIcon, string CharacterLogo, AutoEnum.eResources Resource, Int64 Count, float InitialAtkSpeed1, float PhysicalPierceRate, float MagicPierceRate, float AtkInteval, float InitialFireDefend, float InitialWaterDefend, Int64 InitialAtkSpeed2, Int64 InitialAtkSpeed3, float CharacterBody, AutoEnum.eDamagePriority DamagePriority, string DeadAct, Int64 DeadSound, Int64 HpReserveSpeed, Int64 MpReserveSpeed, float CircleArea)
        {
            _ID = ID;
            _Name = Name;
            _Prefab = Prefab;
            _InitialAtk = InitialAtk;
            _InitialSpeed = InitialSpeed;
            _InitialHp = InitialHp;
            _InitialCritical = InitialCritical;
            _InitialCriticalDamage = InitialCriticalDamage;
            _InitialDefend = InitialDefend;
            _InitialMagicDefend = InitialMagicDefend;
            _InitialBlood = InitialBlood;
            _InitialMagicAttack = InitialMagicAttack;
            _InitialPhysicalPierce = InitialPhysicalPierce;
            _InitialMagicPierce = InitialMagicPierce;
            _InitialAfford = InitialAfford;
            _InitialDodge = InitialDodge;
            _InitialResilience = InitialResilience;
            _CharacterStrengthen = CharacterStrengthen;
            _VisualField = VisualField;
            _CharacterLevel = CharacterLevel;
            _InitialMp = InitialMp;
            _InitialHpReserve = InitialHpReserve;
            _InitialMpReserve = InitialMpReserve;
            _InitialWeapon = InitialWeapon;
            _PrefabShow = PrefabShow;
            _Skill = Skill;
            _InitialSkill = InitialSkill;
            _Icon = Icon;
            _Atk1 = Atk1;
            _Atk2 = Atk2;
            _Atk3 = Atk3;
            _DirIcon = DirIcon;
            _CharacterLogo = CharacterLogo;
            _Resource = Resource;
            _Count = Count;
            _InitialAtkSpeed1 = InitialAtkSpeed1;
            _PhysicalPierceRate = PhysicalPierceRate;
            _MagicPierceRate = MagicPierceRate;
            _AtkInteval = AtkInteval;
            _InitialFireDefend = InitialFireDefend;
            _InitialWaterDefend = InitialWaterDefend;
            _InitialAtkSpeed2 = InitialAtkSpeed2;
            _InitialAtkSpeed3 = InitialAtkSpeed3;
            _CharacterBody = CharacterBody;
            _DamagePriority = DamagePriority;
            _DeadAct = DeadAct;
            _DeadSound = DeadSound;
            _HpReserveSpeed = HpReserveSpeed;
            _MpReserveSpeed = MpReserveSpeed;
            _CircleArea = CircleArea;
        }
        public readonly static ReadOnlyDictionary<Int64, Character> Datas = new ReadOnlyDictionary<Int64, Character>(
            new Dictionary<Int64, Character>() {
                {100001, new Character(100001, "小哥", "Spine/Character/Prefabs/Xiaoge", 7f, 5.8f, 380f, 0.15f, 2.25f, 10f, 9f, 0f, 10f, 0f, 0f, 50f, 0.03f, 0.03f, 1001, 8f, 1, 425f, 1f, 1f, 2002, "Spine/Character/Res/WuXie/wuxie_SkeletonData", new Int64[] {11001,11002,11003,11004}, new Int64[] {11001,11002,11003,11004,11006,11007,11008}, "UI/Sprites/Character/XiaogeHead", 11006, 11007, 11008, "UI/Sprites/Character/img_head_wuxie", "UI/Sprites/Character/img_character_job1", AutoEnum.eResources.Brass, 100, 600f, 0.1f, 0.1f, 1200f, 9f, 9f, 455, 980, 1.5f, AutoEnum.eDamagePriority.First, "die", 4018, 2250, 1450, 1f)},
                {100002, new Character(100002, "小哥", "Spine/Character/Prefabs/WuXie", 5f, 3f, 140f, 0f, 2.25f, 17f, 14f, 0f, 8f, 0f, 0f, 40f, 0.05f, 0.05f, 1001, 8f, 2, 140f, 1f, 1f, 2002, "Spine/Character/Res/WuXie/wuxie_SkeletonData", new Int64[] {11001,11003}, new Int64[] {11001,11003}, "UI/Sprites/Character/WuXieHead", 11002, 11002, 11002, "UI/Sprites/Character/img_head_xiaoge", "UI/Sprites/Character/img_character_job2", AutoEnum.eResources.Brass, 101, 1001f, 0.2f, 0.2f, 1801f, 14f, 14f, 1001, 1001, 1.5f, AutoEnum.eDamagePriority.First, "die", 4018, 2250, 1450, 1f)},
                {100003, new Character(100003, "王胖子", "Spine/Character/Prefabs/WuXie", 4f, 5f, 115f, 0f, 1.5f, 12f, 10f, 0f, 12f, 0f, 0f, 38f, 0.01f, 0.01f, 1001, 8f, 3, 115f, 1f, 1f, 2002, "Spine/Character/Res/WuXie/wuxie_SkeletonData", new Int64[] {11001,11002}, new Int64[] {11001,11002}, "UI/Sprites/Character/WpzHead", 11003, 11003, 11003, "UI/Sprites/Character/img_head_wangpangzi", "UI/Sprites/Character/img_character_job3", AutoEnum.eResources.Brass, 102, 1002f, 0.3f, 0.3f, 1802f, 10f, 10f, 1002, 1002, 1.5f, AutoEnum.eDamagePriority.First, "die", 4018, 2250, 1450, 1f)},
                {100004, new Character(100004, "大奎", "Spine/Character/Prefabs/WuXie", 4f, 5f, 250f, 0f, 1.5f, 24f, 13f, 0f, 5f, 0f, 0f, 32f, 0.01f, 0.01f, 1001, 8f, 4, 180f, 1f, 1f, 2002, "Spine/Character/Res/WuXie/wuxie_SkeletonData", new Int64[] {11001,11005}, new Int64[] {11001,11005}, "UI/Sprites/Character/DkHead", 11004, 11004, 11004, "UI/Sprites/Character/img_head_wangpangzi", "UI/Sprites/Character/img_character_job4", AutoEnum.eResources.Brass, 103, 1003f, 0.4f, 0.4f, 1803f, 13f, 13f, 1003, 1003, 1.5f, AutoEnum.eDamagePriority.First, "die", 4018, 2250, 1450, 1f)},
                {100005, new Character(100005, "潘子", "Spine/Character/Prefabs/PanZi", 5f, 5f, 265f, 0f, 1.8f, 10f, 9f, 0f, 7f, 0f, 0f, 36f, 0.02f, 0.02f, 1001, 8f, 5, 370f, 1f, 2f, 2002, "Spine/Character/Res/PanZi/panzi_SkeletonData", new Int64[] {11009,11010,11011,11012}, new Int64[] {11009,11010,11011,11012,11013,11014,11015}, "UI/Sprites/Character/WpzHead", 11013, 11014, 11015, "UI/Sprites/Character/img_head_panzi", "UI/Sprites/Character/img_character_job5", AutoEnum.eResources.Brass, 104, 1004f, 0.5f, 0.5f, 1804f, 9f, 9f, 1004, 1004, 1.5f, AutoEnum.eDamagePriority.First, "die", 4018, 2250, 1450, 1f)},
            }
        );
        public static Character Get(Int64 id) {
            Character config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
