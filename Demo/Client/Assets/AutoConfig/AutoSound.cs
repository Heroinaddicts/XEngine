using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Sound
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly AutoEnum.eSoundType _eSoundType;
        public readonly string _AudioMaterial;
        public readonly bool _Loop;
        public Sound(Int64 ID, string Des, AutoEnum.eSoundType eSoundType, string AudioMaterial, bool Loop)
        {
            _ID = ID;
            _Des = Des;
            _eSoundType = eSoundType;
            _AudioMaterial = AudioMaterial;
            _Loop = Loop;
        }
        public readonly static ReadOnlyDictionary<Int64, Sound> Datas = new ReadOnlyDictionary<Int64, Sound>(
            new Dictionary<Int64, Sound>() {
                {4001, new Sound(4001, "小哥普攻1", AutoEnum.eSoundType.EffectAll, "Sound/Effect/xiaoge_atk1", false)},
                {4002, new Sound(4002, "小哥普攻2", AutoEnum.eSoundType.EffectAll, "Sound/Effect/xiaoge_atk2", false)},
                {4003, new Sound(4003, "小哥普攻3", AutoEnum.eSoundType.EffectAll, "Sound/Effect/xiaoge_atk3", false)},
                {4004, new Sound(4004, "小哥移动", AutoEnum.eSoundType.EffectAll, "Sound/Effect/yidong", false)},
                {4005, new Sound(4005, "小哥血阵", AutoEnum.eSoundType.EffectAll, "Sound/Effect/Blood", false)},
                {4006, new Sound(4006, "小哥声波", AutoEnum.eSoundType.EffectAll, "Sound/Effect/Wistle", false)},
                {4007, new Sound(4007, "小哥踢腿", AutoEnum.eSoundType.EffectAll, "Sound/Effect/xiaoge_atk3", false)},
                {4008, new Sound(4008, "小哥位移", AutoEnum.eSoundType.EffectAll, "Sound/Effect/Move", false)},
                {4009, new Sound(4009, "背景音乐", AutoEnum.eSoundType.EffectSelf, "Sound/Bgm/dating", true)},
                {4010, new Sound(4010, "战斗场景音乐", AutoEnum.eSoundType.EffectSelf, "Sound/Bgm/fuben", true)},
                {4011, new Sound(4011, "交互", AutoEnum.eSoundType.EffectSelf, "Sound/Bgm/jiaohu", false)},
                {4012, new Sound(4012, "普通怪物挨打", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/Hurt", false)},
                {4013, new Sound(4013, "普通怪物死亡", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/Die", false)},
                {4014, new Sound(4014, "九头蛇柏普攻", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/Snake_Atk", false)},
                {4015, new Sound(4015, "九头蛇柏飞出头", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/Snake_Fly", false)},
                {4016, new Sound(4016, "藤曼飞出来", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/Snake_Tengman", false)},
                {4017, new Sound(4017, "九头蛇柏地刺", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/Snake_Dici", false)},
                {4018, new Sound(4018, "小哥死亡", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/XiaogeDie", false)},
                {4019, new Sound(4019, "蛇柏插入地下", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/Snake_Insert", false)},
                {4020, new Sound(4020, "潘子普攻", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/PanAtk", false)},
                {4021, new Sound(4021, "潘子火箭", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/PanHuoJian", false)},
                {4022, new Sound(4022, "潘子剑雨", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/PanJianyu", false)},
                {4023, new Sound(4023, "潘子后撤", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/PanHouche", false)},
                {4024, new Sound(4024, "潘子buff", AutoEnum.eSoundType.EffectSelf, "Sound/Effect/PanBuff", false)},
            }
        );
        public static Sound Get(Int64 id) {
            Sound config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
