using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class NonePerfom
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly Int64[] _Buff;
        public NonePerfom(Int64 ID, string Des, Int64[] Buff)
        {
            _ID = ID;
            _Des = Des;
            _Buff = Buff;
        }
        public readonly static ReadOnlyDictionary<Int64, NonePerfom> Datas = new ReadOnlyDictionary<Int64, NonePerfom>(
            new Dictionary<Int64, NonePerfom>() {
                {1001, new NonePerfom(1001, "小哥被动加buff技能", new Int64[] {1017,1022})},
            }
        );
        public static NonePerfom Get(Int64 id) {
            NonePerfom config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
