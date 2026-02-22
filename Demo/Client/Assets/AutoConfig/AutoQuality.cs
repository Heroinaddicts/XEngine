using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Quality
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _QualityName;
        public readonly float[] _QualityColor;
        public readonly string _QualityPicture;
        public Quality(Int64 ID, string QualityName, float[] QualityColor, string QualityPicture)
        {
            _ID = ID;
            _QualityName = QualityName;
            _QualityColor = QualityColor;
            _QualityPicture = QualityPicture;
        }
        public readonly static ReadOnlyDictionary<Int64, Quality> Datas = new ReadOnlyDictionary<Int64, Quality>(
            new Dictionary<Int64, Quality>() {
                {1001, new Quality(1001, "杀死xxx野怪", new float[] {1f,1f,1f}, "xxx")},
            }
        );
        public static Quality Get(Int64 id) {
            Quality config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
