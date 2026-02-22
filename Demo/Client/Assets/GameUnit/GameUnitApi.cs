using GameUnit;
using System;
using System.Collections.ObjectModel;
using System.Runtime.CompilerServices;
using UnityEngine;
using XUtils;
using static GameUnitApi;

public static class GameUnitApi
{
    public interface IUnitAttribute
    {
        eAttributeType Type { get; }
        int Index { get; }
        string Name { get; }
        bool Sync { get; }
        bool Visual { get; }
        bool Save { get; }
        bool Important { get; }
        IUnit Host { get; }
    }

    public delegate void fTrigger2D(IUnit self, GameObject other, Collider2D collider);
    public delegate void fCollision2D(IUnit self, GameObject other, Collision2D collider);
    public interface IUnit
    {
        GameObject gameObject { get; }
        Transform transform { get; }
        public T GetComponent<T>() where T : Component;
        public T AddComponent<T>() where T : Component;

        Vector2 Position { get; set; }
        float Angle { get; set; }

        UInt64 Guid { get; }
        Int64 ConfigID { get; set; }
        eUnitType Type { get; }

        bool Is(in eUnitType type);

        string ToString();

        IUnitAttribute[] Attributes { get; }
        UInt16 GetUInt16(in int index);
        bool SetUInt16(in int index, in UInt16 value, in bool trigger = true);

        Int16 GetInt16(in int index);
        bool SetInt16(in int index, in Int16 value, in bool trigger = true);

        UInt32 GetUInt32(in int index);
        bool SetUInt32(in int index, in UInt32 value, in bool trigger = true);

        Int32 GetInt32(in int index);
        bool SetInt32(in int index, in Int32 value, in bool trigger = true);

        UInt64 GetUInt64(in int index);
        bool SetUInt64(in int index, in UInt64 value, in bool trigger = true);

        Int64 GetInt64(in int index);
        bool SetInt64(in int index, in Int64 value, in bool trigger = true);

        float GetFloat(in int index);
        bool SetFloat(in int index, in float value, in bool trigger = true);

        string GetString(in int index);
        bool SetString(in int index, in string value, in bool trigger = true);

        bool GetBool(in int index);
        bool SetBool(in int index, in bool value, in bool trigger = true);

        Vector2 GetVector2(in int index);
        bool SetVector2(in int index, in Vector2 value, in bool trigger = true);
        Vector3 GetVector3(in int index);
        bool SetVector3(in int index, in Vector3 value, in bool trigger = true);
        Vector4 GetVector4(in int index);
        bool SetVector4(in int index, in Vector4 value, in bool trigger = true);
        public void RegistTriggerEnter2D(fTrigger2D trigger);
        public void CancelRegistTriggerEnter2D(fTrigger2D trigger);
        public void RegistTriggerExit2D(fTrigger2D trigger);
        public void CancelRegistTriggerExit2D(fTrigger2D trigger);
        public void RegistCollisionEnter2D(fCollision2D collision);
        public void CancelRegistCollisionEnter2D(fCollision2D collision);
        public void RegistCollisionExit2D(fCollision2D collision);
        public void CancelRegistCollisionExit2D(fCollision2D collision);

        void RegistUpdate(Action<IUnit> update);
        void CancelRegistUpdate(Action<IUnit> update);
        void RegistFixedUpdate(Action<IUnit> update);
        void CancelRegistFixedUpdate(Action<IUnit> update);
        void RegistLateUpdate(Action<IUnit> update);
        void CancelRegistLateUpdate(Action<IUnit> update);

        void RegistAnyAttributeChanged(in fAnyAttributeChanged callback);
        void RegistUInt16AttributeChange(in int index, in eUnitAttributeEvent ev, in fUInt16AttributeChange callback);
        void RegistInt16AttributeChange(in int index, in eUnitAttributeEvent ev, in fInt16AttributeChange callback);
        void RegistUInt32AttributeChange(in int index, in eUnitAttributeEvent ev, in fUInt32AttributeChange callback);
        void RegistInt32AttributeChange(in int index, in eUnitAttributeEvent ev, in fInt32AttributeChange callback);
        void RegistUInt64AttributeChange(in int index, in eUnitAttributeEvent ev, in fUInt64AttributeChange callback);
        void RegistInt64AttributeChange(in int index, in eUnitAttributeEvent ev, in fInt64AttributeChange callback);
        void RegistFloatAttributeChange(in int index, in eUnitAttributeEvent ev, in fFloatAttributeChange callback);
        void RegistStringAttributeChange(in int index, in eUnitAttributeEvent ev, in fStringAttributeChange callback);
        void RegistBoolAttributeChange(in int index, in eUnitAttributeEvent ev, in fBoolAttributeChange callback);
        void RegistVector2AttributeChange(in int index, in eUnitAttributeEvent ev, in fVector2AttributeChange callback);
        void RegistVector3AttributeChange(in int index, in eUnitAttributeEvent ev, in fVector3AttributeChange callback);
        void RegistVector4AttributeChange(in int index, in eUnitAttributeEvent ev, in fVector4AttributeChange callback);
        
    }


    public delegate GameObject fCreateGameObject(in UInt64 guid, in Int64 configId, in eUnitType type);
    public interface IUnitFactory
    {
        void SetGuidGenerater(in Func<UInt64> generater);
        void RegistUnitEvent(in eUnitType type, fOnUnitEvent callback);
        IUnit QueryUnit(in UInt64 guid);
        IUnit CreateUnit(in UInt64 guid, in Int64 configId, in eUnitType type, in fCreateGameObject goFun, Action<IUnit> initFun = null, [CallerFilePath] in string file = "", [CallerLineNumber] in int line = 0);

        ReadOnlyDictionary<UInt64, IUnit> GetUnitsByType(in eUnitType type);

        void ReleaseUnit(in IUnit unit, [CallerFilePath] in string file = "", [CallerLineNumber] in int line = 0);
    }


    public static class GameUnitSetting
    {
        static Action<string> _LogFunction;

        public static void SetLogFunction(in Action<string> fun)
        {
            _LogFunction = fun;
        }

        public static void Log(in string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0)
        {
            _LogFunction?.Invoke($"{file}:{line} >> {log}");
        }
    }

    public static void SetGuidGenerater(in Func<UInt64> generater)
    {
        UnitFactory.GetInstance().SetGuidGenerater(generater);
    }
    public static void RegistUnitEvent(in eUnitType type, fOnUnitEvent callback)
    {
        UnitFactory.GetInstance().RegistUnitEvent(type, callback);
    }

    public static IUnit QueryUnit(in UInt64 guid)
    {
        return UnitFactory.GetInstance().QueryUnit(guid);
    }

    public static ReadOnlyDictionary<UInt64, IUnit> GetUnitsByType(in eUnitType type)
    {
        return UnitFactory.GetInstance().GetUnitsByType(type);
    }

    public static IUnit CreateUnit(in UInt64 guid, in Int64 configId, in eUnitType type, in fCreateGameObject goFun, Action<IUnit> initFun = null, [CallerFilePath] in string file = "", [CallerLineNumber] in int line = 0)
    {
        return UnitFactory.GetInstance().CreateUnit(guid, configId, type, goFun, initFun, file, line);
    }
    public static void ReleaseUnit(in IUnit unit, [CallerFilePath] in string file = "", [CallerLineNumber] in int line = 0)
    {
        UnitFactory.GetInstance().ReleaseUnit(unit, file, line);
    }

    public const UInt64 INVALID_UNIT_GUID = 0;
}
