using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Resource
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly AutoEnum.eResources _ResourceType;
        public readonly Int64 _RelevantID;
        public readonly Int64 _Count;
        public readonly string _Des;
        public readonly string _Icon;
        public Resource(Int64 ID, AutoEnum.eResources ResourceType, Int64 RelevantID, Int64 Count, string Des, string Icon)
        {
            _ID = ID;
            _ResourceType = ResourceType;
            _RelevantID = RelevantID;
            _Count = Count;
            _Des = Des;
            _Icon = Icon;
        }
        public readonly static ReadOnlyDictionary<Int64, Resource> Datas = new ReadOnlyDictionary<Int64, Resource>(
            new Dictionary<Int64, Resource>() {
                {4001, new Resource(4001, AutoEnum.eResources.Special, 9001, 1, "xx", "UI/Sprites/Icon/Hp1")},
                {4002, new Resource(4002, AutoEnum.eResources.Material, 5001, 1, "xx", "UI/Sprites/Icon/Hp1")},
                {4003, new Resource(4003, AutoEnum.eResources.FatigueValue, -1, 1, "xx", "UI/Sprites/Icon/Hp1")},
                {4004, new Resource(4004, AutoEnum.eResources.Equipment, 2002, 1, "xx", "UI/Sprites/Icon/Hp1")},
                {4005, new Resource(4005, AutoEnum.eResources.Special, 9001, 1, "xx", "UI/Sprites/Icon/Hp1")},
            }
        );
        public static Resource Get(Int64 id) {
            Resource config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
