using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TaskKill
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly Int64[] _MonsterKill;
        public TaskKill(Int64 ID, string Des, Int64[] MonsterKill)
        {
            _ID = ID;
            _Des = Des;
            _MonsterKill = MonsterKill;
        }
        public readonly static ReadOnlyDictionary<Int64, TaskKill> Datas = new ReadOnlyDictionary<Int64, TaskKill>(
            new Dictionary<Int64, TaskKill>() {
                {1001, new TaskKill(1001, "杀死xxx野怪", new Int64[] {61001,61002})},
            }
        );
        public static TaskKill Get(Int64 id) {
            TaskKill config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
