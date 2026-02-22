using Google.Protobuf;
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;
using static GameUnitApi;

public interface IScreenSelecter : IModule
{
    public enum eEvent
    {
        Select,
        UnSelect
    }

    public void Regist(in eEvent e, in Action<IUnit> fun);

    GameObject GetSelect();
}


class ScreenSelecter : IScreenSelecter
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IScreenSelecter, ScreenSelecter>();
    }

    GameObject _Select;
    XEventPool<IScreenSelecter.eEvent> _EventPool = new XEventPool<IScreenSelecter.eEvent>();

    public bool Initialize()
    {
        _Select = null;
        return true;
    }

    public bool Launch()
    {
        return true;
    }

    public void Release()
    {
    }
    public void Regist(in IScreenSelecter.eEvent e, in Action<IUnit> fun)
    {
        _EventPool.Regist(e, fun);
    }

    public void Update()
    {
        if (!Input.GetMouseButtonDown(0))
            return;

        if (EventSystem.current != null && EventSystem.current.IsPointerOverGameObject())
            return;

        Vector2 worldPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        Collider2D hit = Physics2D.OverlapPoint(worldPos);

        if (hit == null || hit.GetComponent<IUnit>() == null)
        {
            if (_Select != null)
            {
                _EventPool.Trigger(IScreenSelecter.eEvent.UnSelect, _Select.GetComponent<IUnit>());
                _Select = null;
            }
            return;
        }

        //_EventPool.Trigger(IScreenSelecter.eEvent.UnSelect, _Select.GetComponent<IUnit>());
        if (_Select != null)
        {
            _EventPool.Trigger(IScreenSelecter.eEvent.UnSelect, _Select.GetComponent<IUnit>());
        }
        _Select = hit.gameObject;
        //Debug.LogError($"点击到了物体 {hit.name} ，点击位置是{worldPos}，点击单位类型是{hit.GetComponent<IUnit>().Type}，位置是{hit.GetComponent<IUnit>().Position}");
        _EventPool.Trigger(IScreenSelecter.eEvent.Select, _Select.GetComponent<IUnit>());
    }

    public GameObject GetSelect()
    {
        return _Select;
    }
}
