using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class NpcPositionChange
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly Int64 _TaskEvent;
        public readonly float[] _ChangePosition;
        public NpcPositionChange(Int64 ID, Int64 TaskEvent, float[] ChangePosition)
        {
            _ID = ID;
            _TaskEvent = TaskEvent;
            _ChangePosition = ChangePosition;
        }
        public readonly static ReadOnlyDictionary<Int64, NpcPositionChange> Datas = new ReadOnlyDictionary<Int64, NpcPositionChange>(
            new Dictionary<Int64, NpcPositionChange>() {
                {10001, new NpcPositionChange(10001, 31001, new float[] {1f,1f,1f})},
                {10002, new NpcPositionChange(10002, 31002, new float[] {1f,1f,2f})},
            }
        );
        public static NpcPositionChange Get(Int64 id) {
            NpcPositionChange config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
