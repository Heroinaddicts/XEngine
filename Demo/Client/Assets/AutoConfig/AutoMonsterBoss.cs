using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class MonsterBoss
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _Id;
        public readonly string _Name;
        public readonly float _Atk;
        public readonly float _Speed;
        public readonly float _Hp;
        public readonly float _Critical;
        public readonly float _CriticalDamage;
        public readonly float _Defend;
        public readonly float _MagicDefend;
        public readonly float _Blood;
        public readonly float _MagicAttack;
        public readonly float _PhysicalPierce;
        public readonly float _MagicPierce;
        public readonly float _Dodge;
        public readonly float _Resilience;
        public readonly float _ChaseSpeed;
        public readonly Int64[] _DropResource;
        public readonly Int64 _Exp;
        public readonly string _Prefab;
        public readonly float _AwarenessRange;
        public readonly float _PatrolRange;
        public readonly float _ExitRange;
        public readonly Int64 _Atk1;
        public readonly Int64 _Atk2;
        public readonly Int64 _SecondAtk1;
        public readonly Int64 _SecondAtk2;
        public readonly Int64 _ThirdAtk1;
        public readonly Int64 _ThirdAtk2;
        public readonly float _BackBorn;
        public readonly float _HpRecover;
        public readonly Int64 _HpRecoverRate;
        public readonly Int64 _StartRecover;
        public readonly float _ExchangeMode2;
        public readonly float _ExchangeMode3;
        public readonly Int64[] _MonsterSkill1;
        public readonly Int64[] _MonsterSkill2;
        public readonly Int64[] _MonsterSkill3;
        public readonly float _PhysicalPierceRate;
        public readonly float _MagicPierceRate;
        public readonly Int64 _MonsterQuality;
        public readonly float _WaterMagicDefend;
        public readonly float _GoldMagicDefend;
        public readonly string _DeadAct;
        public readonly Int64 _SoundHurt;
        public readonly Int64 _SoundDie;
        public readonly float _MonsterBody;
        public readonly AutoEnum.eDamagePriority _DamagePriority;
        public readonly Int64[] _MonsterDie;
        public readonly AutoEnum.eBoss _BossType;
        public readonly string _MonsterIdle;
        public readonly string _BossHead;
        public readonly Int64 _HurtEffect;
        public readonly float _VisualField;
        public readonly float _CircleArea;
        public readonly string[] _ConcludePartDie;
        public readonly string[] _PartDieAct;
        public readonly Int64 _Die;
        public readonly Int64 _DropTime;
        public readonly Int64 _DropDelayTime;
        public MonsterBoss(Int64 Id, string Name, float Atk, float Speed, float Hp, float Critical, float CriticalDamage, float Defend, float MagicDefend, float Blood, float MagicAttack, float PhysicalPierce, float MagicPierce, float Dodge, float Resilience, float ChaseSpeed, Int64[] DropResource, Int64 Exp, string Prefab, float AwarenessRange, float PatrolRange, float ExitRange, Int64 Atk1, Int64 Atk2, Int64 SecondAtk1, Int64 SecondAtk2, Int64 ThirdAtk1, Int64 ThirdAtk2, float BackBorn, float HpRecover, Int64 HpRecoverRate, Int64 StartRecover, float ExchangeMode2, float ExchangeMode3, Int64[] MonsterSkill1, Int64[] MonsterSkill2, Int64[] MonsterSkill3, float PhysicalPierceRate, float MagicPierceRate, Int64 MonsterQuality, float WaterMagicDefend, float GoldMagicDefend, string DeadAct, Int64 SoundHurt, Int64 SoundDie, float MonsterBody, AutoEnum.eDamagePriority DamagePriority, Int64[] MonsterDie, AutoEnum.eBoss BossType, string MonsterIdle, string BossHead, Int64 HurtEffect, float VisualField, float CircleArea, string[] ConcludePartDie, string[] PartDieAct, Int64 Die, Int64 DropTime, Int64 DropDelayTime)
        {
            _Id = Id;
            _Name = Name;
            _Atk = Atk;
            _Speed = Speed;
            _Hp = Hp;
            _Critical = Critical;
            _CriticalDamage = CriticalDamage;
            _Defend = Defend;
            _MagicDefend = MagicDefend;
            _Blood = Blood;
            _MagicAttack = MagicAttack;
            _PhysicalPierce = PhysicalPierce;
            _MagicPierce = MagicPierce;
            _Dodge = Dodge;
            _Resilience = Resilience;
            _ChaseSpeed = ChaseSpeed;
            _DropResource = DropResource;
            _Exp = Exp;
            _Prefab = Prefab;
            _AwarenessRange = AwarenessRange;
            _PatrolRange = PatrolRange;
            _ExitRange = ExitRange;
            _Atk1 = Atk1;
            _Atk2 = Atk2;
            _SecondAtk1 = SecondAtk1;
            _SecondAtk2 = SecondAtk2;
            _ThirdAtk1 = ThirdAtk1;
            _ThirdAtk2 = ThirdAtk2;
            _BackBorn = BackBorn;
            _HpRecover = HpRecover;
            _HpRecoverRate = HpRecoverRate;
            _StartRecover = StartRecover;
            _ExchangeMode2 = ExchangeMode2;
            _ExchangeMode3 = ExchangeMode3;
            _MonsterSkill1 = MonsterSkill1;
            _MonsterSkill2 = MonsterSkill2;
            _MonsterSkill3 = MonsterSkill3;
            _PhysicalPierceRate = PhysicalPierceRate;
            _MagicPierceRate = MagicPierceRate;
            _MonsterQuality = MonsterQuality;
            _WaterMagicDefend = WaterMagicDefend;
            _GoldMagicDefend = GoldMagicDefend;
            _DeadAct = DeadAct;
            _SoundHurt = SoundHurt;
            _SoundDie = SoundDie;
            _MonsterBody = MonsterBody;
            _DamagePriority = DamagePriority;
            _MonsterDie = MonsterDie;
            _BossType = BossType;
            _MonsterIdle = MonsterIdle;
            _BossHead = BossHead;
            _HurtEffect = HurtEffect;
            _VisualField = VisualField;
            _CircleArea = CircleArea;
            _ConcludePartDie = ConcludePartDie;
            _PartDieAct = PartDieAct;
            _Die = Die;
            _DropTime = DropTime;
            _DropDelayTime = DropDelayTime;
        }
        public readonly static ReadOnlyDictionary<Int64, MonsterBoss> Datas = new ReadOnlyDictionary<Int64, MonsterBoss>(
            new Dictionary<Int64, MonsterBoss>() {
                {4001, new MonsterBoss(4001, "九头蛇柏", 6f, 0f, 5427f, 0.08f, 1.85f, 20f, 25f, 1f, 1f, 1f, 1f, 1f, 1f, 3f, new Int64[] {4002}, 10, "Spine/Monster/Prefabs/JiuTouSheBai", 30f, 7f, 0f, 11002, 11003, 11004, 11005, 11006, 11007, 15f, 0.001f, 200, 12000, 0.6f, 0.2f, new Int64[] {11002,11003,11004,11005,11006,11007,11008,11009,11010,11011,11012,11013,11014,11015,11016,11017,11018,11019,11020,11021,11022,11023,11024,11025,11026,11027,11028,11029,11030,11031,11032,11033,11034,11035,11036,11037,11038,11039,11040,11041,11042,11043,11044,11045,11046,11047,11048,11049,11050,11051,11052,11053,11054,11055,11056,11057,11058,11059,11060,11061,11062,11063,11064,11065,11067,11068,11069}, new Int64[] {11002,11003,11004,11005,11006,11007,11008,11009,11010,11011,11012,11013,11014,11015,11016,11017,11018,11019,11020,11021,11022,11023,11024,11025,11026,11027,11028,11029,11030,11031,11032,11033,11034,11035,11036,11037,11038,11039,11040,11041,11042,11043,11044,11045,11046,11047,11048,11049,11050,11051,11052,11053,11054,11055,11056,11057,11058,11059,11060,11061,11062,11063,11064,11065,11067,11068,11069}, new Int64[] {11002,11003,11004,11005,11006,11007,11008,11009,11010,11011,11012,11013,11014,11015,11016,11017,11018,11019,11020,11021,11022,11023,11024,11025,11026,11027,11028,11029,11030,11031,11032,11033,11034,11035,11036,11037,11038,11039,11040,11041,11042,11043,11044,11045,11046,11047,11048,11049,11050,11051,11052,11053,11054,11055,11056,11057,11058,11059,11060,11061,11062,11063,11064,11065,11067,11068,11069}, 0.1f, 0.1f, 91001, 1f, 1f, "die", 4012, 4013, 2.65f, AutoEnum.eDamagePriority.First, new Int64[] {4003}, AutoEnum.eBoss.HydraCypress, "Snake_Idle", "UI/Sprites/TopUI/MonsterIcon/Boss_1", 1019, 30f, 3f, new string[] {"SnakeA","SnakeB","SnakeC","SnakeD","Tree"}, new string[] {"Snake_Die-A","Snake_Die-B","Snake_Die-C","Snake_Die-D","Snake_Tree_Die"}, 5200, 450, 500)},
            }
        );
        public static MonsterBoss Get(Int64 id) {
            MonsterBoss config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
