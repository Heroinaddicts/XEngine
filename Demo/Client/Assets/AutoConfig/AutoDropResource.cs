using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class DropResource
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly AutoEnum.eResources _ResourceType;
        public readonly Int64 _RelevantID;
        public readonly Int64 _Count;
        public readonly string _Des;
        public readonly float _DropRate;
        public DropResource(Int64 ID, AutoEnum.eResources ResourceType, Int64 RelevantID, Int64 Count, string Des, float DropRate)
        {
            _ID = ID;
            _ResourceType = ResourceType;
            _RelevantID = RelevantID;
            _Count = Count;
            _Des = Des;
            _DropRate = DropRate;
        }
        public readonly static ReadOnlyDictionary<Int64, DropResource> Datas = new ReadOnlyDictionary<Int64, DropResource>(
            new Dictionary<Int64, DropResource>() {
                {4001, new DropResource(4001, AutoEnum.eResources.Special, 9001, 1, "xx", 0.2f)},
                {4002, new DropResource(4002, AutoEnum.eResources.Material, 5001, 1, "xx", 0.2f)},
                {4003, new DropResource(4003, AutoEnum.eResources.FatigueValue, -1, 1, "xx", 0.2f)},
                {4004, new DropResource(4004, AutoEnum.eResources.Equipment, 2002, 1, "xx", 0.2f)},
                {4005, new DropResource(4005, AutoEnum.eResources.Special, 9001, 1, "xx", 0.2f)},
            }
        );
        public static DropResource Get(Int64 id) {
            DropResource config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
