using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class ResourceQuality
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Name;
        public readonly string _Icon;
        public readonly string _Icon2;
        public ResourceQuality(Int64 ID, string Name, string Icon, string Icon2)
        {
            _ID = ID;
            _Name = Name;
            _Icon = Icon;
            _Icon2 = Icon2;
        }
        public readonly static ReadOnlyDictionary<Int64, ResourceQuality> Datas = new ReadOnlyDictionary<Int64, ResourceQuality>(
            new Dictionary<Int64, ResourceQuality>() {
                {4001, new ResourceQuality(4001, "白", "xxx", "xxx")},
                {4002, new ResourceQuality(4002, "篮", "xxx", "xxx")},
                {4003, new ResourceQuality(4003, "紫", "xxx", "xxx")},
                {4004, new ResourceQuality(4004, "金", "xxx", "xxx")},
                {4005, new ResourceQuality(4005, "红", "xxx", "xxx")},
                {4006, new ResourceQuality(4006, "彩", "xxx", "xxx")},
            }
        );
        public static ResourceQuality Get(Int64 id) {
            ResourceQuality config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
