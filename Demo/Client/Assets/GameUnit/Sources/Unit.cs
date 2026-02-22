using System;
using System.Collections.Generic;
using UnityEngine;
using static GameUnitApi;

namespace GameUnit
{

    abstract class UnitMiddleWare : MonoBehaviour
    {
        protected virtual void Awake() { }
        protected virtual void OnDestroy() { }
        protected virtual void OnTriggerEnter2D(Collider2D other) { }
        protected virtual void OnTriggerExit2D(Collider2D other) { }
        protected virtual void OnCollisionEnter2D(Collision2D collision) { }
        protected virtual void OnCollisionExit2D(Collision2D collision) { }
    }

    abstract class Unit : UnitMiddleWare, IUnit
    {
        protected UInt64 _Guid;
        protected eUnitType _Type;
        protected Int64 _ConfigID;
        protected string _Name;
        protected float _Angle;
        public string Name { get { return _Name; } }


        public T GetComponent<T>() where T : Component
        {
            return gameObject.GetComponent<T>();
        }

        public T AddComponent<T>() where T : Component
        {
            return gameObject.AddComponent<T>();
        }

        public IUnitAttribute[] Attributes => _Attributes;
        protected Attribute[] _Attributes;
        public UInt64 Guid
        {
            get { return _Guid; }
            set { _Guid = value; }
        }

        public Int64 ConfigID
        {
            get { return _ConfigID; }
            set { _ConfigID = value; }
        }

        public Vector2 Position
        {
            get
            {
                return transform.position;
            }

            set
            {
                transform.position = value;
            }
        }

        public eUnitType Type => _Type;

        public float Angle { get { return _Angle; } set { _Angle = value; } }

        public abstract bool Is(in eUnitType type);

        public abstract void OnInitialize(in UInt64 guid, in Int64 configId, in string file, in int line);
        public abstract void OnCreate(in string file, in int line);
        public abstract void OnRelease(in string file, in int line);

        public UInt16 GetUInt16(in int index)
        {
            return _Attributes[index].GetUInt16();
        }

        public bool SetUInt16(in int index, in UInt16 value, in bool trigger = true)
        {
            return _Attributes[index].SetUInt16(value, trigger);
        }

        public Int16 GetInt16(in int index)
        {
            return _Attributes[index].GetInt16();
        }

        public bool SetInt16(in int index, in Int16 value, in bool trigger = true)
        {
            return _Attributes[index].SetInt16(value, trigger);
        }

        public UInt32 GetUInt32(in int index)
        {
            return _Attributes[index].GetUInt32();
        }

        public bool SetUInt32(in int index, in UInt32 value, in bool trigger = true)
        {
            return _Attributes[index].SetUInt32(value, trigger);
        }

        public Int32 GetInt32(in int index)
        {
            return _Attributes[index].GetInt32();
        }

        public bool SetInt32(in int index, in Int32 value, in bool trigger = true)
        {
            return _Attributes[index].SetInt32(value, trigger);
        }

        public UInt64 GetUInt64(in int index)
        {
            return _Attributes[index].GetUInt64();
        }

        public bool SetUInt64(in int index, in UInt64 value, in bool trigger = true)
        {
            return _Attributes[index].SetUInt64(value, trigger);
        }

        public Int64 GetInt64(in int index)
        {
            return _Attributes[index].GetInt64();
        }

        public bool SetInt64(in int index, in Int64 value, in bool trigger = true)
        {
            return _Attributes[index].SetInt64(value, trigger);
        }

        public float GetFloat(in int index)
        {
            return _Attributes[index].GetFloat();
        }

        public bool SetFloat(in int index, in float value, in bool trigger = true)
        {
            return _Attributes[index].SetFloat(value, trigger);
        }

        public string GetString(in int index)
        {
            return _Attributes[index].GetString();
        }

        public bool SetString(in int index, in string value, in bool trigger = true)
        {
            return _Attributes[index].SetString(value, trigger);
        }

        public bool GetBool(in int index)
        {
            return _Attributes[index].GetBool();
        }

        public bool SetBool(in int index, in bool value, in bool trigger = true)
        {
            return _Attributes[index].SetBool(value, trigger);
        }

        public Vector2 GetVector2(in int index)
        {
            return _Attributes[index].GetVector2();
        }

        public bool SetVector2(in int index, in Vector2 value, in bool trigger = true)
        {
            return _Attributes[index].SetVector2(value, trigger);
        }

        public Vector3 GetVector3(in int index)
        {
            return _Attributes[index].GetVector3();
        }

        public bool SetVector3(in int index, in Vector3 value, in bool trigger = true)
        {
            return _Attributes[index].SetVector3(value, trigger);
        }

        public Vector4 GetVector4(in int index)
        {
            return _Attributes[index].GetVector4();
        }

        public bool SetVector4(in int index, in Vector4 value, in bool trigger = true)
        {
            return _Attributes[index].SetVector4(value, trigger);
        }

        HashSet<GameUnitApi.fTrigger2D> _TriggerEnters2D, _TriggerExits2D;
        List<GameUnitApi.fTrigger2D> _TriggerEnter2DRecycleBin, _TriggerExit2DRecycleBin;
        HashSet<GameUnitApi.fCollision2D> _CollisionEnters2D, _CollisionExits2D;
        List<GameUnitApi.fCollision2D> _CollisionEnter2DRecycleBin, _CollisionExit2DRecycleBin;

        public void RegistTriggerEnter2D(fTrigger2D trigger)
        {
            if (null == _TriggerEnters2D)
                _TriggerEnters2D = new HashSet<GameUnitApi.fTrigger2D>();
            _TriggerEnters2D.Add(trigger);
        }
        public void CancelRegistTriggerEnter2D(fTrigger2D trigger)
        {
            if (null == _TriggerEnter2DRecycleBin)
                _TriggerEnter2DRecycleBin = new List<fTrigger2D>();
            _TriggerEnter2DRecycleBin.Add(trigger);
        }
        public void RegistTriggerExit2D(fTrigger2D trigger)
        {
            if (null == _TriggerExits2D)
                _TriggerExits2D = new HashSet<GameUnitApi.fTrigger2D>();
            _TriggerExits2D.Add(trigger);
        }
        public void CancelRegistTriggerExit2D(fTrigger2D trigger)
        {
            if (null == _TriggerExit2DRecycleBin)
                _TriggerExit2DRecycleBin = new List<fTrigger2D>();
            _TriggerExit2DRecycleBin.Add(trigger);
        }
        public void RegistCollisionEnter2D(fCollision2D collision)
        {
            if (null == _CollisionEnters2D)
                _CollisionEnters2D = new HashSet<fCollision2D>();
            _CollisionEnters2D.Add(collision);
        }
        public void CancelRegistCollisionEnter2D(fCollision2D collision)
        {
            if (null == _CollisionEnter2DRecycleBin)
                _CollisionEnter2DRecycleBin = new List<fCollision2D>();
            _CollisionEnter2DRecycleBin.Add(collision);
        }
        public void RegistCollisionExit2D(fCollision2D collision)
        {
            if (null == _CollisionExits2D)
                _CollisionExits2D = new HashSet<GameUnitApi.fCollision2D>();
            _CollisionExits2D.Add(collision);
        }
        public void CancelRegistCollisionExit2D(fCollision2D collision)
        {
            if (null == _CollisionExit2DRecycleBin)
                _CollisionExit2DRecycleBin = new List<fCollision2D>();
            _CollisionExit2DRecycleBin.Add(collision);
        }

        protected override sealed void OnTriggerEnter2D(Collider2D collider)
        {
            if (null != _TriggerEnters2D)
                foreach (var f in _TriggerEnters2D)
                    f?.Invoke(this, collider.gameObject, collider);

            if (null != _TriggerEnter2DRecycleBin)
                for (int i = 0; i < _TriggerEnter2DRecycleBin.Count; i++)
                    _TriggerEnters2D.Remove(_TriggerEnter2DRecycleBin[i]);
            _TriggerEnter2DRecycleBin?.Clear();
        }
        protected override sealed void OnTriggerExit2D(Collider2D collider)
        {
            if (null != _TriggerExits2D)
                foreach (var f in _TriggerExits2D)
                    f?.Invoke(this, collider.gameObject, collider);

            if (null != _TriggerExit2DRecycleBin)
                for (int i = 0; i < _TriggerExit2DRecycleBin.Count; i++)
                    _TriggerExits2D.Remove(_TriggerExit2DRecycleBin[i]);
            _TriggerExit2DRecycleBin?.Clear();
        }
        protected override sealed void OnCollisionEnter2D(Collision2D collision)
        {
            if (null != _CollisionEnters2D)
                foreach (var f in _CollisionEnters2D)
                    f?.Invoke(this, collision.gameObject, collision);

            if (null != _CollisionEnter2DRecycleBin)
                for (int i = 0; i < _CollisionEnter2DRecycleBin.Count; i++)
                    _CollisionEnters2D.Remove(_CollisionEnter2DRecycleBin[i]);
            _CollisionEnter2DRecycleBin?.Clear();
        }
        protected override sealed void OnCollisionExit2D(Collision2D collision)
        {
            if (null != _CollisionExits2D)
                foreach (var f in _CollisionExits2D)
                    f?.Invoke(this, collision.gameObject, collision);

            if (null != _CollisionExit2DRecycleBin)
                for (int i = 0; i < _CollisionExit2DRecycleBin.Count; i++)
                    _CollisionExits2D.Remove(_CollisionExit2DRecycleBin[i]);
            _CollisionExit2DRecycleBin?.Clear();
        }

        HashSet<Action<IUnit>> _Updates, _FixedUpdates, _LateUpdates;
        List<Action<IUnit>> _UpdatesRecycleBin, _FixedUpdatesRecycleBin, _LateUpdatesRecycleBin;
        void Update()
        {
            if (null != _Updates)
                foreach (var update in _Updates)
                    update?.Invoke(this);

            if (null != _UpdatesRecycleBin && null != _Updates)
                for (int i = 0; i < _UpdatesRecycleBin.Count; i++)
                    _Updates.Remove(_UpdatesRecycleBin[i]);

            _UpdatesRecycleBin?.Clear();
        }

        void FixedUpdate()
        {
            if (null != _FixedUpdates)
                foreach (var update in _FixedUpdates)
                    update?.Invoke(this);

            if (null != _FixedUpdatesRecycleBin && null != _FixedUpdates)
                for (int i = 0; i < _FixedUpdatesRecycleBin.Count; i++)
                    _FixedUpdates.Remove(_FixedUpdatesRecycleBin[i]);

            _FixedUpdatesRecycleBin?.Clear();
        }

        void LateUpdate()
        {
            if (null != _LateUpdates)
                foreach (var update in _LateUpdates)
                    update?.Invoke(this);

            if (null != _LateUpdatesRecycleBin && null != _LateUpdates)
                for (int i = 0; i < _LateUpdatesRecycleBin.Count; i++)
                    _LateUpdates.Remove(_LateUpdatesRecycleBin[i]);

            _LateUpdatesRecycleBin?.Clear();
        }

        public void RegistUpdate(Action<IUnit> update)
        {
            if (null == _Updates)
            {
                _Updates = new HashSet<Action<IUnit>>();
            }

            _Updates.Add(update);
        }

        public void CancelRegistUpdate(Action<IUnit> update)
        {
            if (null == _UpdatesRecycleBin)
            {
                _UpdatesRecycleBin = new List<Action<IUnit>>();
            }
            _UpdatesRecycleBin.Add(update);
        }

        public void RegistFixedUpdate(Action<IUnit> fixedUpdate)
        {
            if (null == _FixedUpdates)
            {
                _FixedUpdates = new HashSet<Action<IUnit>>();
            }

            _FixedUpdates.Add(fixedUpdate);
        }

        public void CancelRegistFixedUpdate(Action<IUnit> fixedUpdate)
        {
            if (null == _FixedUpdatesRecycleBin)
            {
                _FixedUpdatesRecycleBin = new List<Action<IUnit>>();
            }
            _FixedUpdatesRecycleBin.Add(fixedUpdate);
        }

        public void RegistLateUpdate(Action<IUnit> lateUpdate)
        {
            if (null == _LateUpdates)
            {
                _LateUpdates = new HashSet<Action<IUnit>>();
            }

            _LateUpdates.Add(lateUpdate);
        }

        public void CancelRegistLateUpdate(Action<IUnit> lateUpdate)
        {
            if (null == _LateUpdatesRecycleBin)
            {
                _LateUpdatesRecycleBin = new List<Action<IUnit>>();
            }
            _LateUpdatesRecycleBin.Add(lateUpdate);
        }

        HashSet<fAnyAttributeChanged> _AnyChanged = new HashSet<fAnyAttributeChanged>();
        public void RegistAnyAttributeChanged(in fAnyAttributeChanged callback)
        {
            _AnyChanged.Add(callback);
        }

        public void TriggerAnyChanged(in int index, in bool sync, in bool visual, in bool save, in bool important)
        {
            foreach (var callback in _AnyChanged)
            {
                callback(this, index, sync, visual, save, important);
            }
        }

        public void RegistUInt16AttributeChange(in int index, in eUnitAttributeEvent ev, in fUInt16AttributeChange callback)
        {
            _Attributes[index].RegistUInt16AttributeChange(ev, callback);
        }

        public void RegistInt16AttributeChange(in int index, in eUnitAttributeEvent ev, in fInt16AttributeChange callback)
        {
            _Attributes[index].RegistInt16AttributeChange(ev, callback);
        }

        public void RegistUInt32AttributeChange(in int index, in eUnitAttributeEvent ev, in fUInt32AttributeChange callback)
        {
            _Attributes[index].RegistUInt32AttributeChange(ev, callback);
        }

        public void RegistInt32AttributeChange(in int index, in eUnitAttributeEvent ev, in fInt32AttributeChange callback)
        {
            _Attributes[index].RegistInt32AttributeChange(ev, callback);
        }

        public void RegistUInt64AttributeChange(in int index, in eUnitAttributeEvent ev, in fUInt64AttributeChange callback)
        {
            _Attributes[index].RegistUInt64AttributeChange(ev, callback);
        }

        public void RegistInt64AttributeChange(in int index, in eUnitAttributeEvent ev, in fInt64AttributeChange callback)
        {
            _Attributes[index].RegistInt64AttributeChange(ev, callback);
        }

        public void RegistFloatAttributeChange(in int index, in eUnitAttributeEvent ev, in fFloatAttributeChange callback)
        {
            _Attributes[index].RegistFloatAttributeChange(ev, callback);
        }

        public void RegistStringAttributeChange(in int index, in eUnitAttributeEvent ev, in fStringAttributeChange callback)
        {
            _Attributes[index].RegistStringAttributeChange(ev, callback);
        }

        public void RegistBoolAttributeChange(in int index, in eUnitAttributeEvent ev, in fBoolAttributeChange callback)
        {
            _Attributes[index].RegistBoolAttributeChange(ev, callback);
        }

        public void RegistVector2AttributeChange(in int index, in eUnitAttributeEvent ev, in fVector2AttributeChange callback)
        {
            _Attributes[index].RegistVector2AttributeChange(ev, callback);
        }

        public void RegistVector3AttributeChange(in int index, in eUnitAttributeEvent ev, in fVector3AttributeChange callback)
        {
            _Attributes[index].RegistVector3AttributeChange(ev, callback);
        }

        public void RegistVector4AttributeChange(in int index, in eUnitAttributeEvent ev, in fVector4AttributeChange callback)
        {
            _Attributes[index].RegistVector4AttributeChange(ev, callback);
        }
    }
}
