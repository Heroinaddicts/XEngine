using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Monsterkill
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _Id;
        public readonly Int64 _Monster;
        public readonly Int64 _Count;
        public Monsterkill(Int64 Id, Int64 Monster, Int64 Count)
        {
            _Id = Id;
            _Monster = Monster;
            _Count = Count;
        }
        public readonly static ReadOnlyDictionary<Int64, Monsterkill> Datas = new ReadOnlyDictionary<Int64, Monsterkill>(
            new Dictionary<Int64, Monsterkill>() {
                {61001, new Monsterkill(61001, 4001, 10)},
                {61002, new Monsterkill(61002, 4001, 20)},
            }
        );
        public static Monsterkill Get(Int64 id) {
            Monsterkill config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
