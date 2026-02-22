using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Runtime.CompilerServices;
using UnityEngine;
using static GameUnitApi;

namespace GameUnit
{
    class UnitFactory : IUnitFactory
    {
        Func<UInt64> _GuidGenerater;
        public UInt64 GenerateGuid()
        {
            if (null == _GuidGenerater)
            {
                return 0;
            }

            return _GuidGenerater();
        }

        Dictionary<eUnitType, List<fOnUnitEvent>> _UnitEventPool;
        Dictionary<UInt64, Unit> _UnitMap = new Dictionary<ulong, Unit>();
        Dictionary<eUnitType, Dictionary<UInt64, IUnit>> _TypeMap = new Dictionary<eUnitType, Dictionary<ulong, IUnit>>();

        public void RegistUnitEvent(in eUnitType type, fOnUnitEvent callback)
        {
            if (null == _UnitEventPool)
            {
                _UnitEventPool = new Dictionary<eUnitType, List<fOnUnitEvent>>();
            }

            List<fOnUnitEvent> pool;
            if (!_UnitEventPool.TryGetValue(type, out pool))
            {
                pool = new List<fOnUnitEvent>();
                _UnitEventPool.Add(type, pool);
            }

            _UnitEventPool[type].Add(callback);
        }

        public void TriggerUnitEvent(in eUnitEvent ev, in eUnitType type, in IUnit unit, in bool parentType, in string file, in int line)
        {
            if (null != _UnitEventPool)
            {
                List<fOnUnitEvent> list;
                if (_UnitEventPool.TryGetValue(type, out list))
                {
                    for (int i = 0; i < list.Count; i++)
                    {
                        list[i]?.Invoke(ev, unit, parentType, file, line);
                    }
                }
            }
        }

        public void SetGuidGenerater(in Func<UInt64> generater)
        {
            _GuidGenerater = generater;
        }

        private static UnitFactory s_Instance;
        public static UnitFactory GetInstance()
        {
            if (null == s_Instance)
            {
                s_Instance = new UnitFactory();
            }

            return s_Instance;
        }

        private UnitFactory() { }


        public IUnit QueryUnit(in UInt64 guid)
        {
            Unit unit = null;
            if (_UnitMap.TryGetValue(guid, out unit))
            {
                return unit;
            }

            return null;
        }

        public ReadOnlyDictionary<UInt64, IUnit> GetUnitsByType(in eUnitType type)
        {
            Dictionary<UInt64, IUnit> map;

            if (!_TypeMap.TryGetValue(type, out map))
            {
                return null;
            }

            return new ReadOnlyDictionary<UInt64, IUnit>(map);
        }

        public IUnit CreateUnit(in UInt64 guid, in Int64 configId, in eUnitType type, in fCreateGameObject goFun, Action<IUnit> initFun, in string file, in int line)
        {
            if (_UnitMap.ContainsKey(guid))
            {
                GameUnitSetting.Log($"Unit {type} Guid {guid} already exits");
                return null;
            }

            Unit unit = UnityCreator.CreateUnit(guid, configId, type, goFun(guid, configId, type));
            unit.OnInitialize(guid, configId, file, line);
            _UnitMap.Add(guid, unit);

            Dictionary<UInt64, IUnit> typeMap;
            if (!_TypeMap.TryGetValue(type, out typeMap))
            {
                typeMap = new Dictionary<UInt64, IUnit>();
                _TypeMap.Add(type, typeMap);
            }

            typeMap.Add(guid, unit);

            initFun?.Invoke(unit);
            unit.OnCreate(file, line);
            return unit;
        }

        public void ReleaseUnit(in IUnit unit, in string file, in int line)
        {
            if (unit is Unit u)
            {
                u.OnRelease(file, line);
                _UnitMap.Remove(unit.Guid);
                _TypeMap[unit.Type].Remove(unit.Guid);
                u.Guid = GameUnitApi.INVALID_UNIT_GUID;
                GameObject.Destroy(unit.gameObject);
            }
            else
            {
                GameUnitSetting.Log($"ReleaseUnit error {unit}");
            }
        }
    }
}
