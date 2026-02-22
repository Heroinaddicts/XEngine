using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Tests
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly Int64 _TestInt;
        public readonly float _TestFloat;
        public readonly string _TestString;
        public readonly bool _TestBool;
        public readonly AutoEnum.eTransactionType _TestEnum;
        public readonly Int64[] _TestIntArray;
        public readonly float[] _TestFloatArray;
        public readonly string[] _TestStringArray;
        public readonly bool[] _TestBoolArray;
        public readonly AutoEnum.eTransactionType[] _TestEnumArray;
        public Tests(Int64 ID, Int64 TestInt, float TestFloat, string TestString, bool TestBool, AutoEnum.eTransactionType TestEnum, Int64[] TestIntArray, float[] TestFloatArray, string[] TestStringArray, bool[] TestBoolArray, AutoEnum.eTransactionType[] TestEnumArray)
        {
            _ID = ID;
            _TestInt = TestInt;
            _TestFloat = TestFloat;
            _TestString = TestString;
            _TestBool = TestBool;
            _TestEnum = TestEnum;
            _TestIntArray = TestIntArray;
            _TestFloatArray = TestFloatArray;
            _TestStringArray = TestStringArray;
            _TestBoolArray = TestBoolArray;
            _TestEnumArray = TestEnumArray;
        }
        public readonly static ReadOnlyDictionary<Int64, Tests> Datas = new ReadOnlyDictionary<Int64, Tests>(
            new Dictionary<Int64, Tests>() {
                {100001, new Tests(100001, 112388, 10.012312f, "Hello Test String1", false, AutoEnum.eTransactionType.Tradable, new Int64[] {100,12333,9987}, new float[] {1.678f,8.9756f}, new string[] {"HelloWorld1","Helloword2","Hellosowed1"}, new bool[] {}, new AutoEnum.eTransactionType[] {AutoEnum.eTransactionType.Tradable,AutoEnum.eTransactionType.None,AutoEnum.eTransactionType.Tradable})},
                {100002, new Tests(100002, 1123123, 120.065123f, "Hello Test String2", false, AutoEnum.eTransactionType.Tradable, new Int64[] {9987}, new float[] {8.9756f}, new string[] {}, new bool[] {false,true,false}, new AutoEnum.eTransactionType[] {})},
                {100003, new Tests(100003, 567, 3510.0123f, "Hello Test String3", false, AutoEnum.eTransactionType.Tradable, new Int64[] {}, new float[] {}, new string[] {"HelloWorld1"}, new bool[] {false}, new AutoEnum.eTransactionType[] {AutoEnum.eTransactionType.Tradable})},
            }
        );
        public static Tests Get(Int64 id) {
            Tests config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
