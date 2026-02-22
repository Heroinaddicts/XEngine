using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Equipment
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Name;
        public readonly AutoEnum.eEquipmentType _EquipmentType;
        public readonly AutoEnum.eTransactionType _TransactionType;
        public readonly AutoEnum.eSoldType _SoldType;
        public readonly AutoEnum.eResources _SoldCurrency;
        public readonly Int64 _Price;
        public readonly string _Des;
        public readonly Int64 _LevelLimit;
        public readonly Int64 _Occupy;
        public readonly Int64 _Patience;
        public readonly Int64 _WeaponQuality;
        public readonly float _Weight;
        public readonly float _Atk;
        public readonly float _Speed;
        public readonly float _Hp;
        public readonly float _Critical;
        public readonly float _CriticalDamage;
        public readonly float _Defend;
        public readonly float _MagicDefend;
        public readonly float _Blood;
        public readonly float _MagicAttack;
        public readonly float _PhysicalPierce;
        public readonly float _MagicPierce;
        public readonly float _Dodge;
        public readonly float _Resilience;
        public readonly Int64 _Dynamic;
        public readonly Int64 _Skill;
        public readonly string _Icon;
        public readonly Int64 _EquipmentStrengthen;
        public readonly bool _EquipmentAppearType;
        public readonly string _AppearPrefab;
        public readonly AutoEnum.eWeaponType _WeaponType;
        public readonly string _EquipmentTypeName;
        public readonly Int64[] _CharacterUse;
        public readonly float _InitialStrengthen;
        public readonly float _PhysicalPierceRate;
        public readonly float _MagicPierceRate;
        public readonly string _DropIcon;
        public readonly Int64 _Quality;
        public Equipment(Int64 ID, string Name, AutoEnum.eEquipmentType EquipmentType, AutoEnum.eTransactionType TransactionType, AutoEnum.eSoldType SoldType, AutoEnum.eResources SoldCurrency, Int64 Price, string Des, Int64 LevelLimit, Int64 Occupy, Int64 Patience, Int64 WeaponQuality, float Weight, float Atk, float Speed, float Hp, float Critical, float CriticalDamage, float Defend, float MagicDefend, float Blood, float MagicAttack, float PhysicalPierce, float MagicPierce, float Dodge, float Resilience, Int64 Dynamic, Int64 Skill, string Icon, Int64 EquipmentStrengthen, bool EquipmentAppearType, string AppearPrefab, AutoEnum.eWeaponType WeaponType, string EquipmentTypeName, Int64[] CharacterUse, float InitialStrengthen, float PhysicalPierceRate, float MagicPierceRate, string DropIcon, Int64 Quality)
        {
            _ID = ID;
            _Name = Name;
            _EquipmentType = EquipmentType;
            _TransactionType = TransactionType;
            _SoldType = SoldType;
            _SoldCurrency = SoldCurrency;
            _Price = Price;
            _Des = Des;
            _LevelLimit = LevelLimit;
            _Occupy = Occupy;
            _Patience = Patience;
            _WeaponQuality = WeaponQuality;
            _Weight = Weight;
            _Atk = Atk;
            _Speed = Speed;
            _Hp = Hp;
            _Critical = Critical;
            _CriticalDamage = CriticalDamage;
            _Defend = Defend;
            _MagicDefend = MagicDefend;
            _Blood = Blood;
            _MagicAttack = MagicAttack;
            _PhysicalPierce = PhysicalPierce;
            _MagicPierce = MagicPierce;
            _Dodge = Dodge;
            _Resilience = Resilience;
            _Dynamic = Dynamic;
            _Skill = Skill;
            _Icon = Icon;
            _EquipmentStrengthen = EquipmentStrengthen;
            _EquipmentAppearType = EquipmentAppearType;
            _AppearPrefab = AppearPrefab;
            _WeaponType = WeaponType;
            _EquipmentTypeName = EquipmentTypeName;
            _CharacterUse = CharacterUse;
            _InitialStrengthen = InitialStrengthen;
            _PhysicalPierceRate = PhysicalPierceRate;
            _MagicPierceRate = MagicPierceRate;
            _DropIcon = DropIcon;
            _Quality = Quality;
        }
        public readonly static ReadOnlyDictionary<Int64, Equipment> Datas = new ReadOnlyDictionary<Int64, Equipment>(
            new Dictionary<Int64, Equipment>() {
                {2001, new Equipment(2001, "鞋子", AutoEnum.eEquipmentType.Shose, AutoEnum.eTransactionType.None, AutoEnum.eSoldType.Sold, AutoEnum.eResources.Coin, 100, "xxxx", 5, 1, 30, 1001, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1f, 1001, 5001, "xxx.png", 4001, false, "Weapon/Prefabs/t2", AutoEnum.eWeaponType.None, "鞋子", new Int64[] {100001}, 1f, 0.1f, 0.1f, "RewardItem/LootItem/LootItem/Res/IMG/TrueHead", 1001)},
                {2002, new Equipment(2002, "武器", AutoEnum.eEquipmentType.Weapon, AutoEnum.eTransactionType.None, AutoEnum.eSoldType.Sold, AutoEnum.eResources.Coin, 100, "xxxx", 5, 1, 30, 1001, 1f, 2f, 2f, 2f, 2f, 2f, 2f, 2f, 2f, 1f, 1f, 1f, 1f, 1f, 1001, 5001, "xxx.png", 4001, true, "Weapon/Prefabs/t1", AutoEnum.eWeaponType.Sword, "弓箭", new Int64[] {100002}, 1f, 0.2f, 0.2f, "RewardItem/LootItem/LootItem/Res/IMG/TrueHead", 1001)},
                {2003, new Equipment(2003, "裤子", AutoEnum.eEquipmentType.Pants, AutoEnum.eTransactionType.None, AutoEnum.eSoldType.Sold, AutoEnum.eResources.Coin, 100, "xxxx", 5, 1, 30, 1001, 1f, 3f, 3f, 3f, 3f, 3f, 3f, 3f, 3f, 1f, 1f, 1f, 1f, 1f, 1001, 5001, "xxx.png", 4001, false, "xxx", AutoEnum.eWeaponType.None, "裤子", new Int64[] {100003}, 1f, 0.3f, 0.3f, "RewardItem/LootItem/LootItem/Res/IMG/TrueHead", 1001)},
                {2004, new Equipment(2004, "衣服", AutoEnum.eEquipmentType.Cloths, AutoEnum.eTransactionType.None, AutoEnum.eSoldType.Sold, AutoEnum.eResources.Coin, 100, "xxxx", 5, 1, 30, 1001, 1f, 4f, 4f, 4f, 4f, 4f, 4f, 4f, 4f, 1f, 1f, 1f, 1f, 1f, 1001, 5001, "xxx.png", 4001, false, "xxx", AutoEnum.eWeaponType.None, "上衣", new Int64[] {100004}, 1f, 0.4f, 0.4f, "RewardItem/LootItem/LootItem/Res/IMG/TrueHead", 1001)},
                {2005, new Equipment(2005, "腰带", AutoEnum.eEquipmentType.Belt, AutoEnum.eTransactionType.None, AutoEnum.eSoldType.Sold, AutoEnum.eResources.Coin, 100, "xxxx", 5, 1, 30, 1001, 1f, 5f, 5f, 5f, 5f, 5f, 5f, 5f, 5f, 1f, 1f, 1f, 1f, 1f, 1001, 5001, "xxx.png", 4001, false, "xxx", AutoEnum.eWeaponType.None, "腰带", new Int64[] {100005}, 1f, 0.5f, 0.5f, "RewardItem/LootItem/LootItem/Res/IMG/TrueHead", 1001)},
                {2006, new Equipment(2006, "武器1", AutoEnum.eEquipmentType.Weapon, AutoEnum.eTransactionType.None, AutoEnum.eSoldType.Sold, AutoEnum.eResources.Coin, 100, "xxxx", 5, 1, 30, 1001, 1f, 6f, 6f, 6f, 6f, 6f, 6f, 6f, 6f, 1f, 1f, 1f, 1f, 1f, 1001, 5001, "xxx.png", 4001, true, "Weapon/Prefabs/t3", AutoEnum.eWeaponType.Sword, "弓箭", new Int64[] {100002}, 1f, 0.6f, 0.6f, "RewardItem/LootItem/LootItem/Res/IMG/TrueHead", 1001)},
            }
        );
        public static Equipment Get(Int64 id) {
            Equipment config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
