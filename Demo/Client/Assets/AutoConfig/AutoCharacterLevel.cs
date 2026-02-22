using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class CharacterLevel
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _LevelDes;
        public readonly Int64 _UpExp;
        public readonly Int64 _SkillPoint;
        public readonly Int64 _SpecialEffect;
        public readonly Int64 _Level;
        public CharacterLevel(Int64 ID, string LevelDes, Int64 UpExp, Int64 SkillPoint, Int64 SpecialEffect, Int64 Level)
        {
            _ID = ID;
            _LevelDes = LevelDes;
            _UpExp = UpExp;
            _SkillPoint = SkillPoint;
            _SpecialEffect = SpecialEffect;
            _Level = Level;
        }
        public readonly static ReadOnlyDictionary<Int64, CharacterLevel> Datas = new ReadOnlyDictionary<Int64, CharacterLevel>(
            new Dictionary<Int64, CharacterLevel>() {
                {1, new CharacterLevel(1, "Lv1", 100, 0, 1027, 1)},
                {2, new CharacterLevel(2, "Lv1", 150, 5, 1027, 1)},
                {3, new CharacterLevel(3, "Lv1", 200, 5, 1027, 1)},
            }
        );
        public static CharacterLevel Get(Int64 id) {
            CharacterLevel config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
