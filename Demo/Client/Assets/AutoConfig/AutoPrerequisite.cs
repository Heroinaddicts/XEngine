using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Prerequisite
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly Int64 _CharacterLevel;
        public readonly Int64 _TaskEvent;
        public readonly Int64[] _Resource;
        public Prerequisite(Int64 ID, Int64 CharacterLevel, Int64 TaskEvent, Int64[] Resource)
        {
            _ID = ID;
            _CharacterLevel = CharacterLevel;
            _TaskEvent = TaskEvent;
            _Resource = Resource;
        }
        public readonly static ReadOnlyDictionary<Int64, Prerequisite> Datas = new ReadOnlyDictionary<Int64, Prerequisite>(
            new Dictionary<Int64, Prerequisite>() {
                {6001, new Prerequisite(6001, 0, 4004, new Int64[] {0})},
                {6002, new Prerequisite(6002, 0, 4005, new Int64[] {0})},
            }
        );
        public static Prerequisite Get(Int64 id) {
            Prerequisite config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
