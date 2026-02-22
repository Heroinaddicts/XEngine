using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Material
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Name;
        public readonly AutoEnum.eTransactionType _TransactionType;
        public readonly AutoEnum.eSoldType _SoldType;
        public readonly string _Des;
        public readonly AutoEnum.eResources _SoldCurrency;
        public readonly Int64 _Price;
        public readonly string _Icon;
        public readonly string _MaterialTypeName;
        public readonly Int64 _Weight;
        public readonly string _DropIcon;
        public readonly Int64 _Quality;
        public Material(Int64 ID, string Name, AutoEnum.eTransactionType TransactionType, AutoEnum.eSoldType SoldType, string Des, AutoEnum.eResources SoldCurrency, Int64 Price, string Icon, string MaterialTypeName, Int64 Weight, string DropIcon, Int64 Quality)
        {
            _ID = ID;
            _Name = Name;
            _TransactionType = TransactionType;
            _SoldType = SoldType;
            _Des = Des;
            _SoldCurrency = SoldCurrency;
            _Price = Price;
            _Icon = Icon;
            _MaterialTypeName = MaterialTypeName;
            _Weight = Weight;
            _DropIcon = DropIcon;
            _Quality = Quality;
        }
        public readonly static ReadOnlyDictionary<Int64, Material> Datas = new ReadOnlyDictionary<Int64, Material>(
            new Dictionary<Int64, Material>() {
                {5001, new Material(5001, "", AutoEnum.eTransactionType.None, AutoEnum.eSoldType.None, "战国帛书拓本是进入鲁王公墓的特别物件", AutoEnum.eResources.Coin, 100, "RewardItem/LootItem/LootItem/Res/IMG/TrueHead", "晶石", 1, "RewardItem/LootItem/LootItem/Res/IMG/TrueHead", 1001)},
            }
        );
        public static Material Get(Int64 id) {
            Material config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
