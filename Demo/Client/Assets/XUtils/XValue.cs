using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace XUtils
{
    public enum eValueType
    {
        UInt16 = 0,
        Int16 = 1,
        UInt32 = 2,
        Int32 = 3,
        UInt64 = 4,
        Int64 = 5,
        Float = 6,
        String = 7,
        Bool = 8,
        Vector2 = 9,
        Vector3 = 10,
        Vector4 = 11,

        __Unknown__
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct Underlying
    {
        [FieldOffset(0)] public Int16 __Int16;
        [FieldOffset(0)] public UInt16 __UInt16;
        [FieldOffset(0)] public Int32 __Int32;
        [FieldOffset(0)] public UInt32 __UInt32;
        [FieldOffset(0)] public Int64 __Int64;
        [FieldOffset(0)] public UInt64 __UInt64;
        [FieldOffset(0)] public float __Float;
        [FieldOffset(0)] public bool __Bool;
    }
    public class XValue
    {
        public readonly eValueType _Type;

        public Underlying _Underlying;
        public string __String;
        public Vector2 __Vector2;
        public Vector3 __Vector3;
        public Vector4 __Vector4;

        public XValue(in eValueType type)
        {
            _Type = type;
        }

        public XValue(in Int16 value)
        {
            _Type = eValueType.Int16;
            _Underlying.__Int16 = value;
        }

        public XValue(in UInt16 value)
        {
            _Type = eValueType.UInt16;
            _Underlying.__UInt16 = value;
        }

        public XValue(in Int32 value)
        {
            _Type = eValueType.Int32;
            _Underlying.__Int32 = value;
        }

        public XValue(in UInt32 value)
        {
            _Type = eValueType.UInt32;
            _Underlying.__UInt32 = value;
        }

        public XValue(in Int64 value)
        {
            _Type = eValueType.Int64;
            _Underlying.__Int64 = value;
        }

        public XValue(in UInt64 value)
        {
            _Type = eValueType.UInt64;
            _Underlying.__UInt64 = value;
        }

        public XValue(in float value)
        {
            _Type = eValueType.Float;
            _Underlying.__Float = value;
        }

        public XValue(in string value)
        {
            _Type = eValueType.String;
            __String = value;
        }

        public XValue(in bool value)
        {
            _Type = eValueType.Bool;
            _Underlying.__Bool = value;
        }

        public XValue(in Vector2 value)
        {
            _Type = eValueType.Vector2;
            __Vector2 = value;
        }

        public XValue(in Vector3 value)
        {
            _Type = eValueType.Vector3;
            __Vector3 = value;
        }

        public XValue(in Vector4 value)
        {
            _Type = eValueType.Vector4;
            __Vector4 = value;
        }
    }
}
