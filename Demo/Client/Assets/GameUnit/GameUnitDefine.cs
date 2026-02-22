using System;
using UnityEngine;
using XUtils;
using static GameUnitApi;


namespace GameUnit
{
    public enum eUnitEvent
    {
        Create = 0,
        Released = 1
    }

    public delegate void fOnUnitEvent(in eUnitEvent ev, IUnit unit, in bool isParentType, in string file, in int line);

    public enum eUnitAttributeEvent
    {
        Changing, //当注册了Changing事件的回调中有一个返回了false，则代表该回调不允许修改该值，则修改IUnit属性失败
        Changed
    }

    public delegate void fAnyAttributeChanged(in IUnit unit, in int index, in bool sync, in bool visual, in bool save, in bool important);
    public delegate bool fUInt16AttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in UInt16 oldValue, in UInt16 newValue);
    public delegate bool fInt16AttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in Int16 oldValue, in Int16 newValue);
    public delegate bool fUInt32AttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in UInt32 oldValue, in UInt32 newValue);
    public delegate bool fInt32AttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in Int32 oldValue, in Int32 newValue);
    public delegate bool fUInt64AttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in UInt64 oldValue, in UInt64 newValue);
    public delegate bool fInt64AttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in Int64 oldValue, in Int64 newValue);
    public delegate bool fFloatAttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue);
    public delegate bool fBoolAttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in bool oldValue, in bool newValue);
    public delegate bool fStringAttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in string oldValue, in string newValue);
    public delegate bool fVector2AttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in Vector2 oldValue, in Vector2 newValue);
    public delegate bool fVector3AttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in Vector3 oldValue, in Vector3 newValue);
    public delegate bool fVector4AttributeChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in Vector4 oldValue, in Vector4 newValue);
}
