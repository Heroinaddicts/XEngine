using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class MonsterQuality
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _Id;
        public readonly string _Monster;
        public readonly float[] _Color;
        public readonly AutoEnum.eDamagePriority _DamagePriority;
        public MonsterQuality(Int64 Id, string Monster, float[] Color, AutoEnum.eDamagePriority DamagePriority)
        {
            _Id = Id;
            _Monster = Monster;
            _Color = Color;
            _DamagePriority = DamagePriority;
        }
        public readonly static ReadOnlyDictionary<Int64, MonsterQuality> Datas = new ReadOnlyDictionary<Int64, MonsterQuality>(
            new Dictionary<Int64, MonsterQuality>() {
                {91001, new MonsterQuality(91001, "普通", new float[] {0.87f,1f,0.8f}, AutoEnum.eDamagePriority.Third)},
                {91002, new MonsterQuality(91002, "精英", new float[] {0.35f,0.92f,1f}, AutoEnum.eDamagePriority.Third)},
                {91003, new MonsterQuality(91003, "Boss", new float[] {0f,0f,2f}, AutoEnum.eDamagePriority.First)},
            }
        );
        public static MonsterQuality Get(Int64 id) {
            MonsterQuality config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
