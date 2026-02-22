using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TaskCompleteCondition
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly AutoEnum.eTaskType _TaskType;
        public readonly Int64 _TaskTypeID;
        public TaskCompleteCondition(Int64 ID, AutoEnum.eTaskType TaskType, Int64 TaskTypeID)
        {
            _ID = ID;
            _TaskType = TaskType;
            _TaskTypeID = TaskTypeID;
        }
        public readonly static ReadOnlyDictionary<Int64, TaskCompleteCondition> Datas = new ReadOnlyDictionary<Int64, TaskCompleteCondition>(
            new Dictionary<Int64, TaskCompleteCondition>() {
                {11001, new TaskCompleteCondition(11001, AutoEnum.eTaskType.TaskLevel, 1001)},
                {11002, new TaskCompleteCondition(11002, AutoEnum.eTaskType.TaskContact, 51002)},
                {11003, new TaskCompleteCondition(11003, AutoEnum.eTaskType.TaskMaterail, 1001)},
                {11004, new TaskCompleteCondition(11004, AutoEnum.eTaskType.TaskKill, 1001)},
                {11005, new TaskCompleteCondition(11005, AutoEnum.eTaskType.TaskUse, 1001)},
                {11006, new TaskCompleteCondition(11006, AutoEnum.eTaskType.TaskContact, 51001)},
            }
        );
        public static TaskCompleteCondition Get(Int64 id) {
            TaskCompleteCondition config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
