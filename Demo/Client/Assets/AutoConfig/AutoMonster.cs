using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Monster
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
        public readonly float _AtkRange;
        public readonly Int64 _AttackInterval;
        public readonly float _BackBorn;
        public readonly float _RegainHp;
        public readonly Int64 _RegainHpSpeed;
        public readonly Int64 _StartRecover;
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
        public readonly bool _IsHurtAppear;
        public readonly Int64 _DieTime;
        public readonly Int64 _DelayDieTime;
        public readonly Int64 _MonsterSkill;
        public readonly Int64 _HurtEffect;
        public readonly float _VisualField;
        public readonly float _HurtDistance;
        public readonly float _CircleArea;
        public readonly Int64 _Die;
        public readonly Int64 _MonsteAtkSkill;
        public readonly Int64 _DropTime;
        public readonly Int64 _DropDelayTime;
        public Monster(Int64 Id, string Name, float Atk, float Speed, float Hp, float Critical, float CriticalDamage, float Defend, float MagicDefend, float Blood, float MagicAttack, float PhysicalPierce, float MagicPierce, float Dodge, float Resilience, float ChaseSpeed, Int64[] DropResource, Int64 Exp, string Prefab, float AwarenessRange, float PatrolRange, float ExitRange, float AtkRange, Int64 AttackInterval, float BackBorn, float RegainHp, Int64 RegainHpSpeed, Int64 StartRecover, float PhysicalPierceRate, float MagicPierceRate, Int64 MonsterQuality, float WaterMagicDefend, float GoldMagicDefend, string DeadAct, Int64 SoundHurt, Int64 SoundDie, float MonsterBody, AutoEnum.eDamagePriority DamagePriority, bool IsHurtAppear, Int64 DieTime, Int64 DelayDieTime, Int64 MonsterSkill, Int64 HurtEffect, float VisualField, float HurtDistance, float CircleArea, Int64 Die, Int64 MonsteAtkSkill, Int64 DropTime, Int64 DropDelayTime)
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
            _AtkRange = AtkRange;
            _AttackInterval = AttackInterval;
            _BackBorn = BackBorn;
            _RegainHp = RegainHp;
            _RegainHpSpeed = RegainHpSpeed;
            _StartRecover = StartRecover;
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
            _IsHurtAppear = IsHurtAppear;
            _DieTime = DieTime;
            _DelayDieTime = DelayDieTime;
            _MonsterSkill = MonsterSkill;
            _HurtEffect = HurtEffect;
            _VisualField = VisualField;
            _HurtDistance = HurtDistance;
            _CircleArea = CircleArea;
            _Die = Die;
            _MonsteAtkSkill = MonsteAtkSkill;
            _DropTime = DropTime;
            _DropDelayTime = DropDelayTime;
        }
        public readonly static ReadOnlyDictionary<Int64, Monster> Datas = new ReadOnlyDictionary<Int64, Monster>(
            new Dictionary<Int64, Monster>() {
                {4001, new Monster(4001, "尸鳖", 2f, 5f, 30f, 0.1f, 2f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 5f, new Int64[] {4002}, 10, "Spine/Monster/Prefabs/ShiBie", 18f, 6f, 35f, 2f, 10, 16f, 0.15f, 400, 10, 0.1f, 0.1f, 91001, 1f, 1f, "die", 4012, 4013, 1.25f, AutoEnum.eDamagePriority.Fourth, false, 0, 0, 11070, 1018, 21f, 0.25f, 0.85f, 1000, 11070, 450, 500)},
                {4002, new Monster(4002, "血尸", 9f, 6.5f, 720f, 0.25f, 1.5f, 15f, 10f, 1f, 1f, 1f, 1f, 1f, 1f, 6.5f, new Int64[] {4002}, 50, "Spine/Monster/Prefabs/XueShi", 18f, 6f, 35f, 4f, 10, 25f, 0.25f, 400, 10, 0.2f, 0.2f, 91002, 1f, 1f, "die", 4012, 4013, 1.75f, AutoEnum.eDamagePriority.Fourth, false, 0, 0, 11071, 1018, 21f, 0.25f, 4f, 1000, 11078, 450, 500)},
                {4003, new Monster(4003, "藤曼", 1f, 0f, 20f, 0f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 0f, new Int64[] {4002}, 10, "Spine/Monster/Prefabs/NonePre", 35f, 1f, 20f, 2f, 500, 20f, 0.25f, 400, 10, 0.2f, 0.2f, 91002, 1f, 1f, "die", 4012, 4013, 1.5f, AutoEnum.eDamagePriority.Fourth, true, 1200, 6000, 11001, 1018, 16f, 0.25f, 1f, 1000, 0, 450, 500)},
                {4004, new Monster(4004, "藤曼", 1f, 0f, 20f, 0f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 0f, new Int64[] {4002}, 10, "Spine/Monster/Prefabs/NonePre", 35f, 1f, 20f, 2f, 500, 20f, 0.25f, 400, 10, 0.2f, 0.2f, 91002, 1f, 1f, "die", 4012, 4013, 15f, AutoEnum.eDamagePriority.Fourth, true, 1200, 6000, 11001, 1018, 16f, 0.25f, 1f, 1000, 0, 450, 500)},
            }
        );
        public static Monster Get(Int64 id) {
            Monster config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
