using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Move
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly AutoEnum.eMoveType _MoveType;
        public readonly Int64 _Speed;
        public readonly float _AddSpeed;
        public readonly Int64 _Time;
        public readonly bool _ProcessDamageType;
        public readonly float _SkillDamage;
        public readonly Int64 _DamageRange;
        public readonly Int64[] _Buff;
        public readonly AutoEnum.ePhysicType[] _Obstacle;
        public readonly float[] _ReleasePosition;
        public readonly float _DelayTime;
        public readonly AutoEnum.eDamageType _DamgaeType;
        public readonly Int64 _DamageCount;
        public readonly Int64 _DamageInteval;
        public readonly Int64 _DelayEffectTime;
        public readonly AutoEnum.eTargetType _TargetType;
        public readonly bool _DamageTarget;
        public readonly Int64 _UpdateInterval;
        public readonly float _MaxTurnAngle;
        public readonly Int64 _TrailerEffect;
        public readonly Int64 _MoveEffect;
        public readonly float _HeightParameter;
        public Move(Int64 ID, string Des, AutoEnum.eMoveType MoveType, Int64 Speed, float AddSpeed, Int64 Time, bool ProcessDamageType, float SkillDamage, Int64 DamageRange, Int64[] Buff, AutoEnum.ePhysicType[] Obstacle, float[] ReleasePosition, float DelayTime, AutoEnum.eDamageType DamgaeType, Int64 DamageCount, Int64 DamageInteval, Int64 DelayEffectTime, AutoEnum.eTargetType TargetType, bool DamageTarget, Int64 UpdateInterval, float MaxTurnAngle, Int64 TrailerEffect, Int64 MoveEffect, float HeightParameter)
        {
            _ID = ID;
            _Des = Des;
            _MoveType = MoveType;
            _Speed = Speed;
            _AddSpeed = AddSpeed;
            _Time = Time;
            _ProcessDamageType = ProcessDamageType;
            _SkillDamage = SkillDamage;
            _DamageRange = DamageRange;
            _Buff = Buff;
            _Obstacle = Obstacle;
            _ReleasePosition = ReleasePosition;
            _DelayTime = DelayTime;
            _DamgaeType = DamgaeType;
            _DamageCount = DamageCount;
            _DamageInteval = DamageInteval;
            _DelayEffectTime = DelayEffectTime;
            _TargetType = TargetType;
            _DamageTarget = DamageTarget;
            _UpdateInterval = UpdateInterval;
            _MaxTurnAngle = MaxTurnAngle;
            _TrailerEffect = TrailerEffect;
            _MoveEffect = MoveEffect;
            _HeightParameter = HeightParameter;
        }
        public readonly static ReadOnlyDictionary<Int64, Move> Datas = new ReadOnlyDictionary<Int64, Move>(
            new Dictionary<Int64, Move>() {
                {1001, new Move(1001, "小哥跟踪位移位移", AutoEnum.eMoveType.TrackMove, 2, 1f, 5000, false, 10f, 1, new Int64[] {1005}, new AutoEnum.ePhysicType[] {AutoEnum.ePhysicType.Wall}, new float[] {0f,0f}, 1000f, AutoEnum.eDamageType.Physic, 1, 100, 100, AutoEnum.eTargetType.EnemyCharacter, true, 50, 10f, 1012, 1010, 0f)},
                {1002, new Move(1002, "直线位移", AutoEnum.eMoveType.LineMove, 22, 1f, 565, false, 100f, 1, new Int64[] {1005}, new AutoEnum.ePhysicType[] {AutoEnum.ePhysicType.Wall}, new float[] {0f,0f}, 10f, AutoEnum.eDamageType.Physic, 1, 100, 10, AutoEnum.eTargetType.EnemyCharacter, true, 50, 10f, 0, 0, 0f)},
                {1003, new Move(1003, "瞬间移动", AutoEnum.eMoveType.Immediate, 0, 0f, 3000, false, 100f, 120, new Int64[] {1005}, new AutoEnum.ePhysicType[] {AutoEnum.ePhysicType.None}, new float[] {0f,0f}, 20f, AutoEnum.eDamageType.Physic, 1, 100, 100, AutoEnum.eTargetType.EnemyCharacter, true, 50, 10f, 1013, 1011, 0f)},
                {1004, new Move(1004, "抛物线位移", AutoEnum.eMoveType.ParabolaMove, 0, 0f, 3000, false, 100f, 120, new Int64[] {1005}, new AutoEnum.ePhysicType[] {AutoEnum.ePhysicType.None}, new float[] {0f,0f}, 20f, AutoEnum.eDamageType.Physic, 1, 100, 100, AutoEnum.eTargetType.EnemyCharacter, true, 50, 10f, 1013, 1011, 1.5f)},
                {1005, new Move(1005, "藤曼自由选择目的地直线位移", AutoEnum.eMoveType.EverLineMove, 15, 1f, 0, false, 100f, 1, new Int64[] {1005}, new AutoEnum.ePhysicType[] {AutoEnum.ePhysicType.Wall}, new float[] {0f,0f}, 10f, AutoEnum.eDamageType.Physic, 1, 100, 10, AutoEnum.eTargetType.EnemyCharacter, true, 50, 10f, 0, 0, 0f)},
                {1006, new Move(1006, "血尸位移打人", AutoEnum.eMoveType.LineMove, 12, 0.5f, 1600, true, 5f, 2, new Int64[] {1001}, new AutoEnum.ePhysicType[] {AutoEnum.ePhysicType.Wall}, new float[] {0f,0f}, 2550f, AutoEnum.eDamageType.Physic, 0, 600, 10, AutoEnum.eTargetType.EnemyCharacter, false, 50, 10f, 0, 0, 0f)},
                {1007, new Move(1007, "潘子向后位移", AutoEnum.eMoveType.LineMove, -10, 0f, 475, false, 100f, 1, new Int64[] {1005}, new AutoEnum.ePhysicType[] {AutoEnum.ePhysicType.Wall}, new float[] {0f,0f}, 285f, AutoEnum.eDamageType.Physic, 1, 100, 10, AutoEnum.eTargetType.EnemyCharacter, true, 50, 10f, 0, 0, 0f)},
            }
        );
        public static Move Get(Int64 id) {
            Move config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
