using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Dynamic
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly float[] _Atk;
        public readonly float[] _Speed;
        public readonly float[] _Hp;
        public readonly float[] _Critical;
        public readonly float[] _CriticalDamage;
        public readonly float[] _Defend;
        public readonly float[] _MagicDefend;
        public readonly float[] _Blood;
        public readonly float[] _MagicAttack;
        public readonly float[] _PhysicalPierce;
        public readonly float[] _MagicPierce;
        public readonly float[] _Dodge;
        public Dynamic(Int64 ID, float[] Atk, float[] Speed, float[] Hp, float[] Critical, float[] CriticalDamage, float[] Defend, float[] MagicDefend, float[] Blood, float[] MagicAttack, float[] PhysicalPierce, float[] MagicPierce, float[] Dodge)
        {
            _ID = ID;
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
        }
        public readonly static ReadOnlyDictionary<Int64, Dynamic> Datas = new ReadOnlyDictionary<Int64, Dynamic>(
            new Dictionary<Int64, Dynamic>() {
                {1001, new Dynamic(1001, new float[] {1f,3f}, new float[] {1f,4f}, new float[] {1f,5f}, new float[] {1f,6f}, new float[] {1f,7f}, new float[] {1f,8f}, new float[] {1f,9f}, new float[] {1f,10f}, new float[] {1f,11f}, new float[] {1f,12f}, new float[] {1f,13f}, new float[] {1f,14f})},
            }
        );
        public static Dynamic Get(Int64 id) {
            Dynamic config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
