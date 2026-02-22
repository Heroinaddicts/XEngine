using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TaskLevel
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly Int64 _Scene;
        public TaskLevel(Int64 ID, string Des, Int64 Scene)
        {
            _ID = ID;
            _Des = Des;
            _Scene = Scene;
        }
        public readonly static ReadOnlyDictionary<Int64, TaskLevel> Datas = new ReadOnlyDictionary<Int64, TaskLevel>(
            new Dictionary<Int64, TaskLevel>() {
                {1001, new TaskLevel(1001, "通关鲁王宫副本", 20001)},
            }
        );
        public static TaskLevel Get(Int64 id) {
            TaskLevel config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
