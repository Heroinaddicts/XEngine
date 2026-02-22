using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TaskContact
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly Int64 _TaskNpc;
        public TaskContact(Int64 ID, string Des, Int64 TaskNpc)
        {
            _ID = ID;
            _Des = Des;
            _TaskNpc = TaskNpc;
        }
        public readonly static ReadOnlyDictionary<Int64, TaskContact> Datas = new ReadOnlyDictionary<Int64, TaskContact>(
            new Dictionary<Int64, TaskContact>() {
                {51001, new TaskContact(51001, "和Npc1交谈", 1001)},
                {51002, new TaskContact(51002, "和张爹交流", 1002)},
            }
        );
        public static TaskContact Get(Int64 id) {
            TaskContact config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
