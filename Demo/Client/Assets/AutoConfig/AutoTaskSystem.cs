using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TaskSystem
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly AutoEnum.eTaskVariety _TaskVariety;
        public readonly string _Name;
        public readonly Int64 _Prerequisite;
        public readonly Int64 _CompletePrequisite;
        public readonly Int64[] _TaskSon;
        public TaskSystem(Int64 ID, AutoEnum.eTaskVariety TaskVariety, string Name, Int64 Prerequisite, Int64 CompletePrequisite, Int64[] TaskSon)
        {
            _ID = ID;
            _TaskVariety = TaskVariety;
            _Name = Name;
            _Prerequisite = Prerequisite;
            _CompletePrequisite = CompletePrequisite;
            _TaskSon = TaskSon;
        }
        public readonly static ReadOnlyDictionary<Int64, TaskSystem> Datas = new ReadOnlyDictionary<Int64, TaskSystem>(
            new Dictionary<Int64, TaskSystem>() {
                {20001, new TaskSystem(20001, AutoEnum.eTaskVariety.MainMission, "秦岭神树1", 6001, 6001, new Int64[] {10001,10002,10003})},
                {20002, new TaskSystem(20002, AutoEnum.eTaskVariety.SideMission, "秦岭神树2", 6001, 6001, new Int64[] {10001})},
                {20003, new TaskSystem(20003, AutoEnum.eTaskVariety.SpecialMission, "秦岭神树3", 6001, 6001, new Int64[] {10001})},
                {20004, new TaskSystem(20004, AutoEnum.eTaskVariety.MainMission, "秦岭神树4", 6001, 6001, new Int64[] {10001})},
                {20005, new TaskSystem(20005, AutoEnum.eTaskVariety.SideMission, "秦岭神树5", 6001, 6001, new Int64[] {10001})},
            }
        );
        public static TaskSystem Get(Int64 id) {
            TaskSystem config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
