using System;
using System.Collections.Generic;
using UnityEngine;

namespace XUtils
{
    public class XParameters
    {
        List<XValue> _Values;
        public void Push(in Int16 value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }

        public void Push(in UInt16 value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }

        public void Push(in Int32 value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }
        public void Push(in UInt32 value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }

        public void Push(in UInt64 value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }
        public void Push(in Int64 value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }

        public void Push(in string value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }
        public void Push(in float value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }
        public void Push(in bool value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }
        public void Push(in Vector2 value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }
        public void Push(in Vector3 value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }
        public void Push(in Vector4 value)
        {
            if (null == _Values)
                _Values = new List<XValue>();

            _Values.Add(new XValue(value));
        }

        public Int16 GetInt16(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.Int16)
                throw new Exception($"XParmeters _Values[{index}] type not GetInt16, but {_Values[index]._Type}");

            return _Values[index]._Underlying.__Int16;
        }
        public UInt16 GetUInt16(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.UInt16)
                throw new Exception($"XParmeters _Values[{index}] type not GetUInt16, but {_Values[index]._Type}");

            return _Values[index]._Underlying.__UInt16;
        }

        public Int32 GetInt32(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.Int32)
                throw new Exception($"XParmeters _Values[{index}] type not GetInt32, but {_Values[index]._Type}");

            return _Values[index]._Underlying.__Int32;
        }
        public UInt32 GetUInt32(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.UInt32)
                throw new Exception($"XParmeters _Values[{index}] type not GetUInt32, but {_Values[index]._Type}");

            return _Values[index]._Underlying.__UInt32;
        }

        public Int64 GetInt64(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.Int64)
                throw new Exception($"XParmeters _Values[{index}] type not GetInt64, but {_Values[index]._Type}");

            return _Values[index]._Underlying.__Int64;
        }
        public UInt64 GetUInt64(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.UInt64)
                throw new Exception($"XParmeters _Values[{index}] type not GetUInt64, but {_Values[index]._Type}");

            return _Values[index]._Underlying.__UInt64;
        }

        public float GetFloat(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.Float)
                throw new Exception($"XParmeters _Values[{index}] type not GetInt64, but {_Values[index]._Type}");

            return _Values[index]._Underlying.__Float;
        }

        public string GetString(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.String)
                throw new Exception($"XParmeters _Values[{index}] type not GetInt64, but {_Values[index]._Type}");

            return _Values[index].__String;
        }

        public bool GetBool(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.Bool)
                throw new Exception($"XParmeters _Values[{index}] type not GetInt64, but {_Values[index]._Type}");

            return _Values[index]._Underlying.__Bool;
        }

        public Vector2 GetVector2(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.Vector2)
                throw new Exception($"XParmeters _Values[{index}] type not GetInt64, but {_Values[index]._Type}");

            return _Values[index].__Vector2;
        }
        public Vector3 GetVector3(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.Vector3)
                throw new Exception($"XParmeters _Values[{index}] type not GetInt64, but {_Values[index]._Type}");

            return _Values[index].__Vector3;
        }
        public Vector4 GetVector4(in int index)
        {
            if (_Values.Count <= index)
                throw new IndexOutOfRangeException();

            if (_Values[index]._Type != eValueType.Vector4)
                throw new Exception($"XParmeters _Values[{index}] type not GetInt64, but {_Values[index]._Type}");

            return _Values[index].__Vector4;
        }
    }
}
