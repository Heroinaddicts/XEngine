using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class OrganType
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly Int64[] _Buff;
        public readonly AutoEnum.eSkillType _SkillType;
        public readonly Int64 _RelevantID;
        public readonly float[] _Position;
        public readonly string _Material;
        public readonly AutoEnum.eTriggleType _TriggleType;
        public readonly bool _Destroy;
        public readonly Int64 _Hp;
        public OrganType(Int64 ID, string Des, Int64[] Buff, AutoEnum.eSkillType SkillType, Int64 RelevantID, float[] Position, string Material, AutoEnum.eTriggleType TriggleType, bool Destroy, Int64 Hp)
        {
            _ID = ID;
            _Des = Des;
            _Buff = Buff;
            _SkillType = SkillType;
            _RelevantID = RelevantID;
            _Position = Position;
            _Material = Material;
            _TriggleType = TriggleType;
            _Destroy = Destroy;
            _Hp = Hp;
        }
        public readonly static ReadOnlyDictionary<Int64, OrganType> Datas = new ReadOnlyDictionary<Int64, OrganType>(
            new Dictionary<Int64, OrganType>() {
                {4001, new OrganType(4001, "碎石机关", new Int64[] {1005}, AutoEnum.eSkillType.Launch, 21001, new float[] {44f,100f}, "xxx", AutoEnum.eTriggleType.Near, true, 100)},
            }
        );
        public static OrganType Get(Int64 id) {
            OrganType config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
