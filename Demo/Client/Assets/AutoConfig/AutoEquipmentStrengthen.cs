using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class EquipmentStrengthen
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly float _AtkMutiple;
        public readonly float _SpeedMutiple;
        public readonly float _HpMutiple;
        public readonly float _CriticalMutiple;
        public readonly float _CriticalDamageMutiple;
        public readonly float _DefendMutiple;
        public readonly float _MagicDefendMutiple;
        public readonly float _BloodMutiple;
        public readonly float _MagicAttackMutiple;
        public readonly float _PhysicalPierceMutiple;
        public readonly float _MagicPierceMutiple;
        public readonly float _DodgeMutiple;
        public readonly float _HitRate;
        public EquipmentStrengthen(Int64 ID, float AtkMutiple, float SpeedMutiple, float HpMutiple, float CriticalMutiple, float CriticalDamageMutiple, float DefendMutiple, float MagicDefendMutiple, float BloodMutiple, float MagicAttackMutiple, float PhysicalPierceMutiple, float MagicPierceMutiple, float DodgeMutiple, float HitRate)
        {
            _ID = ID;
            _AtkMutiple = AtkMutiple;
            _SpeedMutiple = SpeedMutiple;
            _HpMutiple = HpMutiple;
            _CriticalMutiple = CriticalMutiple;
            _CriticalDamageMutiple = CriticalDamageMutiple;
            _DefendMutiple = DefendMutiple;
            _MagicDefendMutiple = MagicDefendMutiple;
            _BloodMutiple = BloodMutiple;
            _MagicAttackMutiple = MagicAttackMutiple;
            _PhysicalPierceMutiple = PhysicalPierceMutiple;
            _MagicPierceMutiple = MagicPierceMutiple;
            _DodgeMutiple = DodgeMutiple;
            _HitRate = HitRate;
        }
        public readonly static ReadOnlyDictionary<Int64, EquipmentStrengthen> Datas = new ReadOnlyDictionary<Int64, EquipmentStrengthen>(
            new Dictionary<Int64, EquipmentStrengthen>() {
                {1001, new EquipmentStrengthen(1001, 0.25f, 0.26f, 0.27f, 0.28f, 0.29f, 0.30f, 0.31f, 0.32f, 0.33f, 0.34f, 0.35f, 0.36f, 0.37f)},
            }
        );
        public static EquipmentStrengthen Get(Int64 id) {
            EquipmentStrengthen config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
