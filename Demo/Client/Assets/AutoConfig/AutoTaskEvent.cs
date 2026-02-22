using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TaskEvent
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly AutoEnum.eTaskEvent _TaskProgress;
        public readonly Int64 _Task;
        public TaskEvent(Int64 ID, AutoEnum.eTaskEvent TaskProgress, Int64 Task)
        {
            _ID = ID;
            _TaskProgress = TaskProgress;
            _Task = Task;
        }
        public readonly static ReadOnlyDictionary<Int64, TaskEvent> Datas = new ReadOnlyDictionary<Int64, TaskEvent>(
            new Dictionary<Int64, TaskEvent>() {
                {4001, new TaskEvent(4001, AutoEnum.eTaskEvent.PreRecive, 10001)},
                {4002, new TaskEvent(4002, AutoEnum.eTaskEvent.Received, 10001)},
                {4003, new TaskEvent(4003, AutoEnum.eTaskEvent.Complete, 10001)},
                {4004, new TaskEvent(4004, AutoEnum.eTaskEvent.Completed, 10001)},
                {4005, new TaskEvent(4005, AutoEnum.eTaskEvent.Completed, 10002)},
                {4006, new TaskEvent(4006, AutoEnum.eTaskEvent.None, 10001)},
            }
        );
        public static TaskEvent Get(Int64 id) {
            TaskEvent config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
