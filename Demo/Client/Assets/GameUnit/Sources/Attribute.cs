using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using XUtils;
using static GameUnitApi;

namespace GameUnit
{
    class Attribute : IUnitAttribute
    {
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

        Underlying _Underlying;
        string __String;
        Vector2 __Vector2;
        Vector3 __Vector3;
        Vector4 __Vector4;

        public readonly int _Index;
        public readonly eAttributeType _Type;
        public readonly string _Name;
        public readonly bool _Sync;
        public readonly bool _Visual;
        public readonly bool _Save;
        public readonly bool _Important;
        public readonly Unit _Host;


        public eAttributeType Type => _Type;
        public int Index => _Index;
        public string Name => _Name;
        public bool Sync => _Sync;
        public bool Visual => _Visual;
        public bool Save => _Save;
        public bool Important => _Important;
        public IUnit Host => _Host;

        public Attribute(Unit host, in eAttributeType type, in int index, in string name, in bool sync, in bool visual, in bool save, in bool important)
        {
            _Host = host;
            _Type = type;
            _Index = index;
            _Name = name;
            _Sync = sync;
            _Visual = visual;
            _Save = save;
            _Important = important;
        }

        public Int16 GetInt16()
        {
            if (_Type != eAttributeType.Int16)
                throw new Exception($"Attribute {_Name} get error type value");

            return _Underlying.__Int16;
        }
        public bool SetInt16(in Int16 value, in bool trigger)
        {
            if (_Type != eAttributeType.Int16)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _Int16AttributeChangeCallbackPool)
                for (int i = 0; i < _Int16AttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _Int16AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, _Underlying.__Int16, value))
                        return false;
                }

            _Underlying.__Int16 = value;

            if (trigger && null != _Int16AttributeChangeCallbackPool)
            {
                for (int i = 0; i < _Int16AttributeChangeCallbackPool.Count; i++)
                {
                    _Int16AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, _Underlying.__Int16, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);

            return true;
        }
        public UInt16 GetUInt16()
        {
            if (_Type != eAttributeType.UInt16)
                throw new Exception($"Attribute {_Name} get error type value");

            return _Underlying.__UInt16;
        }
        public bool SetUInt16(in UInt16 value, in bool trigger)
        {
            if (_Type != eAttributeType.UInt16)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _UInt16AttributeChangeCallbackPool)
                for (int i = 0; i < _UInt16AttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _UInt16AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, _Underlying.__UInt16, value))
                        return false;
                }

            _Underlying.__UInt16 = value;

            if (trigger && null != _UInt16AttributeChangeCallbackPool)
            {
                for (int i = 0; i < _UInt16AttributeChangeCallbackPool.Count; i++)
                {
                    _UInt16AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, _Underlying.__UInt16, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }

        public Int32 GetInt32()
        {
            if (_Type != eAttributeType.Int32)
                throw new Exception($"Attribute {_Name} get error type value");

            return _Underlying.__Int32;
        }
        public bool SetInt32(in Int32 value, in bool trigger)
        {
            if (_Type != eAttributeType.Int32)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _Int32AttributeChangeCallbackPool)
                for (int i = 0; i < _Int32AttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _Int32AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, _Underlying.__Int32, value))
                        return false;
                }

            _Underlying.__Int32 = value;

            if (trigger && null != _Int32AttributeChangeCallbackPool)
            {
                for (int i = 0; i < _Int32AttributeChangeCallbackPool.Count; i++)
                {
                    _Int32AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, _Underlying.__Int32, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }
        public UInt32 GetUInt32()
        {
            if (_Type != eAttributeType.UInt32)
                throw new Exception($"Attribute {_Name} get error type value");

            return _Underlying.__UInt32;
        }
        public bool SetUInt32(in UInt32 value, in bool trigger)
        {
            if (_Type != eAttributeType.UInt32)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _UInt32AttributeChangeCallbackPool)
                for (int i = 0; i < _UInt32AttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _UInt32AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, _Underlying.__UInt32, value))
                        return false;
                }

            _Underlying.__UInt32 = value;

            if (trigger && null != _UInt32AttributeChangeCallbackPool)
            {
                for (int i = 0; i < _UInt32AttributeChangeCallbackPool.Count; i++)
                {
                    _UInt32AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, _Underlying.__UInt32, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }

        public Int64 GetInt64()
        {
            if (_Type != eAttributeType.Int64)
                throw new Exception($"Attribute {_Name} get error type value");

            return _Underlying.__Int64;
        }
        public bool SetInt64(in Int64 value, in bool trigger)
        {
            if (_Type != eAttributeType.Int64)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _Int64AttributeChangeCallbackPool)
                for (int i = 0; i < _Int64AttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _Int64AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, _Underlying.__Int64, value))
                        return false;
                }

            _Underlying.__Int64 = value;

            if (trigger && null != _Int64AttributeChangeCallbackPool)
            {
                for (int i = 0; i < _Int64AttributeChangeCallbackPool.Count; i++)
                {
                    _Int64AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, _Underlying.__Int64, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }
        public UInt64 GetUInt64()
        {
            if (_Type != eAttributeType.UInt64)
                throw new Exception($"Attribute {_Name} get error type value");

            return _Underlying.__UInt64;
        }
        public bool SetUInt64(in UInt64 value, in bool trigger)
        {
            if (_Type != eAttributeType.UInt64)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _UInt64AttributeChangeCallbackPool)
                for (int i = 0; i < _UInt64AttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _UInt64AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, _Underlying.__UInt64, value))
                        return false;
                }

            _Underlying.__UInt64 = value;

            if (trigger && null != _UInt64AttributeChangeCallbackPool)
            {
                for (int i = 0; i < _UInt64AttributeChangeCallbackPool.Count; i++)
                {
                    _UInt64AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, _Underlying.__UInt64, value);
                }

            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }
        public float GetFloat()
        {
            if (_Type != eAttributeType.Float)
                throw new Exception($"Attribute {_Name} get error type value");

            return _Underlying.__Float;
        }
        public bool SetFloat(in float value, in bool trigger)
        {
            if (_Type != eAttributeType.Float)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _FloatAttributeChangeCallbackPool)
                for (int i = 0; i < _FloatAttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _FloatAttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, _Underlying.__Float, value))
                        return false;
                }

            _Underlying.__Float = value;

            if (trigger && null != _FloatAttributeChangeCallbackPool)
            {
                for (int i = 0; i < _FloatAttributeChangeCallbackPool.Count; i++)
                {
                    _FloatAttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, _Underlying.__Float, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }
        public string GetString()
        {
            if (_Type != eAttributeType.String)
                throw new Exception($"Attribute {_Name} get error type value");

            return __String;
        }
        public bool SetString(in string value, in bool trigger)
        {
            if (_Type != eAttributeType.String)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _StringAttributeChangeCallbackPool)
                for (int i = 0; i < _StringAttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _StringAttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, __String, value))
                        return false;
                }

            __String = value;

            if (trigger && null != _StringAttributeChangeCallbackPool)
            {
                for (int i = 0; i < _StringAttributeChangeCallbackPool.Count; i++)
                {
                    _StringAttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, __String, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }
        public bool GetBool()
        {
            if (_Type != eAttributeType.Bool)
                throw new Exception($"Attribute {_Name} get error type value");

            return _Underlying.__Bool;
        }
        public bool SetBool(in bool value, in bool trigger)
        {
            if (_Type != eAttributeType.Bool)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _BoolAttributeChangeCallbackPool)
                for (int i = 0; i < _BoolAttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _BoolAttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, _Underlying.__Bool, value))
                        return false;
                }

            _Underlying.__Bool = value;

            if (trigger && null != _BoolAttributeChangeCallbackPool)
            {
                for (int i = 0; i < _BoolAttributeChangeCallbackPool.Count; i++)
                {
                    _BoolAttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, _Underlying.__Bool, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }
        public Vector2 GetVector2()
        {
            if (_Type != eAttributeType.Vector2)
                throw new Exception($"Attribute {_Name} get error type value");

            return __Vector2;
        }
        public bool SetVector2(in Vector2 value, in bool trigger)
        {
            if (_Type != eAttributeType.Vector2)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _Vector2AttributeChangeCallbackPool)
                for (int i = 0; i < _Vector2AttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _Vector2AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, __Vector2, value))
                        return false;
                }

            __Vector2 = value;

            if (trigger && null != _Vector2AttributeChangeCallbackPool)
            {
                for (int i = 0; i < _Vector2AttributeChangeCallbackPool.Count; i++)
                {
                    _Vector2AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, __Vector2, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }
        public Vector3 GetVector3()
        {
            if (_Type != eAttributeType.Vector3)
                throw new Exception($"Attribute {_Name} get error type value");

            return __Vector3;
        }
        public bool SetVector3(in Vector3 value, in bool trigger)
        {
            if (_Type != eAttributeType.Vector3)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _Vector3AttributeChangeCallbackPool)
                for (int i = 0; i < _Vector3AttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _Vector3AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, __Vector3, value))
                        return false;
                }

            __Vector3 = value;

            if (trigger && null != _Vector3AttributeChangeCallbackPool)
            {
                for (int i = 0; i < _Vector3AttributeChangeCallbackPool.Count; i++)
                {
                    _Vector3AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, __Vector3, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }
        public Vector4 GetVector4()
        {
            if (_Type != eAttributeType.Vector4)
                throw new Exception($"Attribute {_Name} get error type value");

            return __Vector4;
        }
        public bool SetVector4(in Vector4 value, in bool trigger)
        {
            if (_Type != eAttributeType.Vector4)
                throw new Exception($"Attribute {_Name} set error type value");

            if (trigger && null != _Vector4AttributeChangeCallbackPool)
                for (int i = 0; i < _Vector4AttributeChangeCallbackPool.Count; i++)
                {
                    if (false == _Vector4AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changing, this._Host, this._Index, __Vector4, value))
                        return false;
                }

            __Vector4 = value;

            if (trigger && null != _Vector4AttributeChangeCallbackPool)
            {
                for (int i = 0; i < _Vector4AttributeChangeCallbackPool.Count; i++)
                {
                    _Vector4AttributeChangeCallbackPool[i]?.Invoke(eUnitAttributeEvent.Changed, this._Host, this._Index, __Vector4, value);
                }
            }

            if (trigger)
                _Host.TriggerAnyChanged(_Index, _Sync, _Visual, _Save, _Important);
            return true;
        }

        List<fUInt16AttributeChange> _UInt16AttributeChangeCallbackPool;
        List<fInt16AttributeChange> _Int16AttributeChangeCallbackPool;

        List<fUInt32AttributeChange> _UInt32AttributeChangeCallbackPool;
        List<fInt32AttributeChange> _Int32AttributeChangeCallbackPool;

        List<fUInt64AttributeChange> _UInt64AttributeChangeCallbackPool;
        List<fInt64AttributeChange> _Int64AttributeChangeCallbackPool;

        List<fFloatAttributeChange> _FloatAttributeChangeCallbackPool;
        List<fStringAttributeChange> _StringAttributeChangeCallbackPool;
        List<fBoolAttributeChange> _BoolAttributeChangeCallbackPool;
        List<fVector2AttributeChange> _Vector2AttributeChangeCallbackPool;
        List<fVector3AttributeChange> _Vector3AttributeChangeCallbackPool;
        List<fVector4AttributeChange> _Vector4AttributeChangeCallbackPool;

        public void RegistUInt16AttributeChange(in eUnitAttributeEvent ev, in fUInt16AttributeChange callback)
        {
            if (_Type != eAttributeType.UInt16)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _UInt16AttributeChangeCallbackPool)
                _UInt16AttributeChangeCallbackPool = new List<fUInt16AttributeChange>();

            _UInt16AttributeChangeCallbackPool.Add(callback);
        }

        public void RegistInt16AttributeChange(in eUnitAttributeEvent ev, in fInt16AttributeChange callback)
        {
            if (_Type != eAttributeType.Int16)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _Int16AttributeChangeCallbackPool)
                _Int16AttributeChangeCallbackPool = new List<fInt16AttributeChange>();

            _Int16AttributeChangeCallbackPool.Add(callback);
        }

        public void RegistUInt32AttributeChange(in eUnitAttributeEvent ev, in fUInt32AttributeChange callback)
        {
            if (_Type != eAttributeType.UInt32)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _UInt32AttributeChangeCallbackPool)
                _UInt32AttributeChangeCallbackPool = new List<fUInt32AttributeChange>();

            _UInt32AttributeChangeCallbackPool.Add(callback);
        }

        public void RegistInt32AttributeChange(in eUnitAttributeEvent ev, in fInt32AttributeChange callback)
        {
            if (_Type != eAttributeType.Int32)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _Int32AttributeChangeCallbackPool)
                _Int32AttributeChangeCallbackPool = new List<fInt32AttributeChange>();

            _Int32AttributeChangeCallbackPool.Add(callback);
        }

        public void RegistUInt64AttributeChange(in eUnitAttributeEvent ev, in fUInt64AttributeChange callback)
        {
            if (_Type != eAttributeType.UInt64)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _UInt64AttributeChangeCallbackPool)
                _UInt64AttributeChangeCallbackPool = new List<fUInt64AttributeChange>();

            _UInt64AttributeChangeCallbackPool.Add(callback);
        }

        public void RegistInt64AttributeChange(in eUnitAttributeEvent ev, in fInt64AttributeChange callback)
        {
            if (_Type != eAttributeType.Int64)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _Int64AttributeChangeCallbackPool)
                _Int64AttributeChangeCallbackPool = new List<fInt64AttributeChange>();

            _Int64AttributeChangeCallbackPool.Add(callback);
        }

        public void RegistFloatAttributeChange(in eUnitAttributeEvent ev, in fFloatAttributeChange callback)
        {
            if (_Type != eAttributeType.Float)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _FloatAttributeChangeCallbackPool)
                _FloatAttributeChangeCallbackPool = new List<fFloatAttributeChange>();

            _FloatAttributeChangeCallbackPool.Add(callback);
        }

        public void RegistStringAttributeChange(in eUnitAttributeEvent ev, in fStringAttributeChange callback)
        {
            if (_Type != eAttributeType.String)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _StringAttributeChangeCallbackPool)
                _StringAttributeChangeCallbackPool = new List<fStringAttributeChange>();

            _StringAttributeChangeCallbackPool.Add(callback);
        }

        public void RegistBoolAttributeChange(in eUnitAttributeEvent ev, in fBoolAttributeChange callback)
        {
            if (_Type != eAttributeType.Bool)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _BoolAttributeChangeCallbackPool)
                _BoolAttributeChangeCallbackPool = new List<fBoolAttributeChange>();

            _BoolAttributeChangeCallbackPool.Add(callback);
        }

        public void RegistVector2AttributeChange(in eUnitAttributeEvent ev, in fVector2AttributeChange callback)
        {
            if (_Type != eAttributeType.Vector2)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _Vector2AttributeChangeCallbackPool)
                _Vector2AttributeChangeCallbackPool = new List<fVector2AttributeChange>();

            _Vector2AttributeChangeCallbackPool.Add(callback);
        }

        public void RegistVector3AttributeChange(in eUnitAttributeEvent ev, in fVector3AttributeChange callback)
        {
            if (_Type != eAttributeType.Vector3)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _Vector3AttributeChangeCallbackPool)
                _Vector3AttributeChangeCallbackPool = new List<fVector3AttributeChange>();

            _Vector3AttributeChangeCallbackPool.Add(callback);
        }

        public void RegistVector4AttributeChange(in eUnitAttributeEvent ev, in fVector4AttributeChange callback)
        {
            if (_Type != eAttributeType.Vector4)
                throw new Exception($"RegistUInt16AttributeChange {_Name} type value");

            if (null == _Vector4AttributeChangeCallbackPool)
                _Vector4AttributeChangeCallbackPool = new List<fVector4AttributeChange>();

            _Vector4AttributeChangeCallbackPool.Add(callback);
        }
    }
}
