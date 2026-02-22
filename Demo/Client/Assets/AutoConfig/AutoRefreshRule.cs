using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class RefreshRule
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly AutoEnum.eRefreshThing _RefreshThing;
        public readonly Int64 _RelevantId;
        public readonly Int64 _Count;
        public readonly Int64 _MaxCount;
        public readonly Int64 _Interval;
        public readonly float[] _MonsterPosition;
        public readonly Int64 _PositionRange;
        public RefreshRule(Int64 ID, AutoEnum.eRefreshThing RefreshThing, Int64 RelevantId, Int64 Count, Int64 MaxCount, Int64 Interval, float[] MonsterPosition, Int64 PositionRange)
        {
            _ID = ID;
            _RefreshThing = RefreshThing;
            _RelevantId = RelevantId;
            _Count = Count;
            _MaxCount = MaxCount;
            _Interval = Interval;
            _MonsterPosition = MonsterPosition;
            _PositionRange = PositionRange;
        }
        public readonly static ReadOnlyDictionary<Int64, RefreshRule> Datas = new ReadOnlyDictionary<Int64, RefreshRule>(
            new Dictionary<Int64, RefreshRule>() {
                {5001, new RefreshRule(5001, AutoEnum.eRefreshThing.Monster, 4001, 1, 5000, 1000, new float[] {-14f,14f}, 3)},
                {5002, new RefreshRule(5002, AutoEnum.eRefreshThing.OrganType, 21001, 6, 5000, 1000, new float[] {2f,10f}, 5)},
                {5003, new RefreshRule(5003, AutoEnum.eRefreshThing.Monster, 4002, 5, 5000, 1000, new float[] {1.42f,1.6f}, 3)},
                {5004, new RefreshRule(5004, AutoEnum.eRefreshThing.Monster, 4002, 5, 5000, 1000, new float[] {12.2f,15.6f}, 4)},
                {5005, new RefreshRule(5005, AutoEnum.eRefreshThing.Monster, 4001, 6, 5000, 1000, new float[] {-1.4f,21.33f}, 4)},
                {5006, new RefreshRule(5006, AutoEnum.eRefreshThing.Monster, 4002, 7, 5000, 1000, new float[] {-8.8f,17.6f}, 4)},
                {5007, new RefreshRule(5007, AutoEnum.eRefreshThing.Monster, 4001, 7, 5000, 1000, new float[] {20.11f,12.78f}, 4)},
                {5008, new RefreshRule(5008, AutoEnum.eRefreshThing.MonsterBoss, 4001, 1, 2, 5000, new float[] {-44.78f,49.56f}, 1)},
                {5009, new RefreshRule(5009, AutoEnum.eRefreshThing.Monster, 4003, 1, 15, 1000, new float[] {-45.18f,40.56f}, 1)},
                {5010, new RefreshRule(5010, AutoEnum.eRefreshThing.Monster, 4003, 1, 5, 1000, new float[] {-43.88f,42.56f}, 1)},
                {5011, new RefreshRule(5011, AutoEnum.eRefreshThing.Monster, 4001, 1, 15, 1000, new float[] {-48.5f,38.2f}, 1)},
                {5012, new RefreshRule(5012, AutoEnum.eRefreshThing.Monster, 4001, 1, 15, 1000, new float[] {0.56f,9.95f}, 1)},
                {5013, new RefreshRule(5013, AutoEnum.eRefreshThing.Monster, 4002, 1, 1, 1000, new float[] {0.56f,9.95f}, 1)},
                {5014, new RefreshRule(5014, AutoEnum.eRefreshThing.Monster, 4003, 1, 1, 1000, new float[] {1.26f,68f}, 2)},
                {5015, new RefreshRule(5015, AutoEnum.eRefreshThing.Monster, 4001, 2, 3, 10, new float[] {1.26f,68f}, 3)},
                {5016, new RefreshRule(5016, AutoEnum.eRefreshThing.Monster, 4001, 4, 5, 10, new float[] {19.58f,57.42f}, 3)},
                {5017, new RefreshRule(5017, AutoEnum.eRefreshThing.Monster, 4001, 2, 3, 10, new float[] {9f,44.6f}, 3)},
                {5018, new RefreshRule(5018, AutoEnum.eRefreshThing.Monster, 4001, 2, 3, 10, new float[] {10.4f,10.9f}, 2)},
                {5019, new RefreshRule(5019, AutoEnum.eRefreshThing.Monster, 4001, 4, 5, 10, new float[] {56.31f,41f}, 2)},
                {5020, new RefreshRule(5020, AutoEnum.eRefreshThing.Monster, 4001, 4, 5, 10, new float[] {74f,31f}, 2)},
                {5021, new RefreshRule(5021, AutoEnum.eRefreshThing.Monster, 4001, 4, 6, 10, new float[] {38f,8.9f}, 2)},
                {5022, new RefreshRule(5022, AutoEnum.eRefreshThing.Monster, 4001, 4, 6, 10, new float[] {33.52f,3.19f}, 2)},
                {5023, new RefreshRule(5023, AutoEnum.eRefreshThing.Monster, 4001, 3, 4, 10, new float[] {-86.12f,8.86f}, 2)},
                {5024, new RefreshRule(5024, AutoEnum.eRefreshThing.Monster, 4001, 3, 4, 10, new float[] {-84f,30f}, 2)},
                {5025, new RefreshRule(5025, AutoEnum.eRefreshThing.Monster, 4001, 3, 4, 10, new float[] {-84f,30f}, 2)},
                {5026, new RefreshRule(5026, AutoEnum.eRefreshThing.Monster, 4001, 4, 12, 1000, new float[] {-41.1f,38.6f}, 2)},
                {5027, new RefreshRule(5027, AutoEnum.eRefreshThing.Monster, 4002, 1, 1, 10, new float[] {27.9f,27.6f}, 1)},
                {5028, new RefreshRule(5028, AutoEnum.eRefreshThing.Monster, 4001, 7, 7, 10, new float[] {-25.7f,0.8f}, 1)},
                {5029, new RefreshRule(5029, AutoEnum.eRefreshThing.Monster, 4003, 1, 2, 10, new float[] {32.8f,32.5f}, 1)},
                {5030, new RefreshRule(5030, AutoEnum.eRefreshThing.Monster, 4003, 1, 2, 10, new float[] {-9.9f,49.5f}, 1)},
                {5031, new RefreshRule(5031, AutoEnum.eRefreshThing.Monster, 4003, 1, 2, 10, new float[] {-86.6f,11.5f}, 1)},
            }
        );
        public static RefreshRule Get(Int64 id) {
            RefreshRule config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
