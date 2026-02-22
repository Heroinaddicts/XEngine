using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TaskUse
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly Int64 _Resource;
        public TaskUse(Int64 ID, string Des, Int64 Resource)
        {
            _ID = ID;
            _Des = Des;
            _Resource = Resource;
        }
        public readonly static ReadOnlyDictionary<Int64, TaskUse> Datas = new ReadOnlyDictionary<Int64, TaskUse>(
            new Dictionary<Int64, TaskUse>() {
                {1001, new TaskUse(1001, "穿戴衣服", 4004)},
            }
        );
        public static TaskUse Get(Int64 id) {
            TaskUse config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
