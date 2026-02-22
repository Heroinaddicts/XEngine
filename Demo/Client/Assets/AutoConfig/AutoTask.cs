using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Task
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly AutoEnum.eTaskReceiveType _TaskReceiveType;
        public readonly Int64[] _Resource;
        public readonly string _TaskBrief;
        public readonly Int64 _Prerequisite;
        public readonly Int64[] _TaskCompleteCondition;
        public readonly Int64 _AcceptNpc;
        public readonly Int64 _CompleteNpc;
        public readonly string _TaskAnimation;
        public readonly string[] _TaskPicture;
        public readonly Int64 _TaskSystem;
        public readonly string _TaskName;
        public readonly string _TaskDes;
        public Task(Int64 ID, AutoEnum.eTaskReceiveType TaskReceiveType, Int64[] Resource, string TaskBrief, Int64 Prerequisite, Int64[] TaskCompleteCondition, Int64 AcceptNpc, Int64 CompleteNpc, string TaskAnimation, string[] TaskPicture, Int64 TaskSystem, string TaskName, string TaskDes)
        {
            _ID = ID;
            _TaskReceiveType = TaskReceiveType;
            _Resource = Resource;
            _TaskBrief = TaskBrief;
            _Prerequisite = Prerequisite;
            _TaskCompleteCondition = TaskCompleteCondition;
            _AcceptNpc = AcceptNpc;
            _CompleteNpc = CompleteNpc;
            _TaskAnimation = TaskAnimation;
            _TaskPicture = TaskPicture;
            _TaskSystem = TaskSystem;
            _TaskName = TaskName;
            _TaskDes = TaskDes;
        }
        public readonly static ReadOnlyDictionary<Int64, Task> Datas = new ReadOnlyDictionary<Int64, Task>(
            new Dictionary<Int64, Task>() {
                {10001, new Task(10001, AutoEnum.eTaskReceiveType.TriggleNpc, new Int64[] {4004}, "默认的初始任务，找npc1接触", 0, new Int64[] {11006}, 1001, 1001, "xxx", new string[] {"xxx"}, 20001, "我是王涛", "")},
                {10002, new Task(10002, AutoEnum.eTaskReceiveType.NoTriggle, new Int64[] {4002}, "完成上个后直接进入的任务。接触任务npc1", 6001, new Int64[] {11002}, 0, 1002, "xxx", new string[] {"xxx"}, 20001, "xxxx", "")},
                {10003, new Task(10003, AutoEnum.eTaskReceiveType.TriggleNpc, new Int64[] {4003}, "3穿戴装备任务", 6002, new Int64[] {11005}, 1001, 1001, "xxx", new string[] {"xxx"}, 20001, "xxxx", "")},
            }
        );
        public static Task Get(Int64 id) {
            Task config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
