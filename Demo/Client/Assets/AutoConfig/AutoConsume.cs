using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Consume
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
        public readonly Int64[] _CharacterUse;
        public readonly Int64 _LevelLimit;
        public readonly string _ConsumeName;
        public readonly Int64 _Weight;
        public readonly string _DropIcon;
        public readonly Int64 _Quality;
        public Consume(Int64 ID, string Name, AutoEnum.eTransactionType TransactionType, AutoEnum.eSoldType SoldType, string Des, AutoEnum.eResources SoldCurrency, Int64 Price, string Icon, Int64[] CharacterUse, Int64 LevelLimit, string ConsumeName, Int64 Weight, string DropIcon, Int64 Quality)
        {
            _ID = ID;
            _Name = Name;
            _TransactionType = TransactionType;
            _SoldType = SoldType;
            _Des = Des;
            _SoldCurrency = SoldCurrency;
            _Price = Price;
            _Icon = Icon;
            _CharacterUse = CharacterUse;
            _LevelLimit = LevelLimit;
            _ConsumeName = ConsumeName;
            _Weight = Weight;
            _DropIcon = DropIcon;
            _Quality = Quality;
        }
        public readonly static ReadOnlyDictionary<Int64, Consume> Datas = new ReadOnlyDictionary<Int64, Consume>(
            new Dictionary<Int64, Consume>() {
                {5001, new Consume(5001, "血瓶", AutoEnum.eTransactionType.None, AutoEnum.eSoldType.None, "回复玩家200的生命值", AutoEnum.eResources.Coin, 100, "UI/Sprites/Bag/Consume/Hp1.png", new Int64[] {100001}, 10001, "药水", 1, "RewardItem/LootItem/LootItem/Res/IMG/TrueHead", 1001)},
            }
        );
        public static Consume Get(Int64 id) {
            Consume config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
