using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TaskMaterail
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly Int64[] _Resource;
        public TaskMaterail(Int64 ID, string Des, Int64[] Resource)
        {
            _ID = ID;
            _Des = Des;
            _Resource = Resource;
        }
        public readonly static ReadOnlyDictionary<Int64, TaskMaterail> Datas = new ReadOnlyDictionary<Int64, TaskMaterail>(
            new Dictionary<Int64, TaskMaterail>() {
                {1001, new TaskMaterail(1001, "收集材料xxx材料", new Int64[] {4004})},
            }
        );
        public static TaskMaterail Get(Int64 id) {
            TaskMaterail config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
