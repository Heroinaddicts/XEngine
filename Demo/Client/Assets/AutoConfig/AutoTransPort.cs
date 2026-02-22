using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TransPort
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Des;
        public readonly Int64 _Scene;
        public readonly float[] _Position;
        public readonly AutoEnum.eTransPortType _TransPortType;
        public readonly Int64 _WhenEffect;
        public readonly Int64 _EffectNumber;
        public readonly Int64 _EffecTime;
        public readonly Int64 _TransPortRange;
        public readonly string _Effect;
        public readonly bool _IsLoop;
        public readonly string _WhereLoop;
        public TransPort(Int64 ID, string Des, Int64 Scene, float[] Position, AutoEnum.eTransPortType TransPortType, Int64 WhenEffect, Int64 EffectNumber, Int64 EffecTime, Int64 TransPortRange, string Effect, bool IsLoop, string WhereLoop)
        {
            _ID = ID;
            _Des = Des;
            _Scene = Scene;
            _Position = Position;
            _TransPortType = TransPortType;
            _WhenEffect = WhenEffect;
            _EffectNumber = EffectNumber;
            _EffecTime = EffecTime;
            _TransPortRange = TransPortRange;
            _Effect = Effect;
            _IsLoop = IsLoop;
            _WhereLoop = WhereLoop;
        }
        public readonly static ReadOnlyDictionary<Int64, TransPort> Datas = new ReadOnlyDictionary<Int64, TransPort>(
            new Dictionary<Int64, TransPort>() {
                {10001, new TransPort(10001, "潘家园主城传送到副本", 20002, new float[] {54f,36f}, AutoEnum.eTransPortType.Normal, 10, 0, 1000, 2, "Effect/Prefabs/Environment/Fire/Cartoon/SpinPortalGold", true, "blood_3")},
                {10002, new TransPort(10002, "战斗地点撤离点传送回到潘家园", 20001, new float[] {-2.32f,51.93f}, AutoEnum.eTransPortType.Evacuate, 900000, 4, 15000, 2, "Effect/Prefabs/Environment/Fire/Cartoon/SpinPortalGold2", true, "blood_3")},
            }
        );
        public static TransPort Get(Int64 id) {
            TransPort config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
