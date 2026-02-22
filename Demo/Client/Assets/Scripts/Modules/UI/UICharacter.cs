using AutoConfig;
using GameUnit;
using System;
using System.Collections.Generic;
using System.Xml;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using static GameUnitApi;

public interface IUICharacter : IModule
{

}

class UICharacter : IUICharacter
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IUICharacter, UICharacter>();
    }
    // IClientBattleCopy _ClientBattleCopy;


    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        return true;
    }

    public void Release()
    {

    }

    Dictionary<UInt64, GameObject> _PlayerCharacterTitileUIs = new Dictionary<UInt64, GameObject>();
    //bool _lastBattleState = false;
    void CreatePlayerCharacterTitleUI(IUnit playerCharacterUnit)
    {
        if (_PlayerCharacterTitileUIs.ContainsKey(playerCharacterUnit.Guid))//去重，不然会重复创建UI
        {
            return;
        }

        Transform UITop = null;
        Queue<Transform> transformQueue = new Queue<Transform>();
        transformQueue.Enqueue(playerCharacterUnit.transform);

        while (transformQueue.Count > 0)
        {
            Transform current = transformQueue.Dequeue();
            if (current.name == "TOP")
            {
                UITop = current;
                break;
            }

            foreach (Transform child in current)
            {
                transformQueue.Enqueue(child);
            }
        }
        if (UITop == null)
        {
            return;
        }
        Transform canvasTransform = GameObject.Find("Canvas")?.transform;
        if (canvasTransform == null)
        {
            XApi.Error("找不到主Canvas，无法创建头顶UI");
            return;
        }
        GameObject UITopPre = Resources.Load<GameObject>("UI/Prefabs/UITop/TopUI");
        if (UITopPre == null)
        {
            XApi.Error("TopUI.prefab 加载失败，返回空对象");
            return;
        }
        GameObject titleUI = GameObject.Instantiate(UITopPre);
        if (_PlayerCharacterTitileUIs.ContainsKey(playerCharacterUnit.Guid))
        {
            GameObject.Destroy(titleUI);
            return;
        }

        titleUI.transform.SetParent(canvasTransform, false);
        titleUI.transform.SetSiblingIndex(0);
        RectTransform titleRect = titleUI.GetComponent<RectTransform>();
    }

    void UpdateCharacterTitleUI(IUnit playerCharacterUnit)
    {
        if (_PlayerCharacterTitileUIs.TryGetValue(playerCharacterUnit.Guid, out GameObject titleUI))
        {
            Camera mainCamera = Camera.main;
            if (mainCamera == null) return;

            Transform UITop = null;
            Queue<Transform> transformQueue = new Queue<Transform>();
            transformQueue.Enqueue(playerCharacterUnit.transform);
            while (transformQueue.Count > 0)
            {
                Transform current = transformQueue.Dequeue();
                if (current.name == "TOP")
                {
                    UITop = current;
                    break;
                }
                foreach (Transform child in current)
                {
                    transformQueue.Enqueue(child);
                }
            }
            if (UITop == null) return;


            // 转换为屏幕空间坐标
            Vector3 screenPos = mainCamera.WorldToScreenPoint(UITop.position);

            bool isVisible = screenPos.z > 0 &&
                             screenPos.x >= 0 && screenPos.x <= Screen.width &&
                             screenPos.y >= 0 && screenPos.y <= Screen.height;
            titleUI.SetActive(isVisible);

            if (isVisible)
            {
                RectTransform titleRect = titleUI.GetComponent<RectTransform>();
                RectTransform canvasRect = titleUI.transform.parent.GetComponent<RectTransform>();

                if (RectTransformUtility.ScreenPointToLocalPointInRectangle(canvasRect, screenPos, null, out Vector2 localPos))
                {
                    localPos.y += 30f;

                    titleRect.anchoredPosition = localPos;
                }

            }

        }
    }
    void ReleasePlayerCharacterTitleUI(IUnit playerCharacterUnit)
    {
        if (_PlayerCharacterTitileUIs.TryGetValue(playerCharacterUnit.Guid, out GameObject titleUI))
        {
            // 从字典移除并销毁
            _PlayerCharacterTitileUIs.Remove(playerCharacterUnit.Guid);
            GameObject.Destroy(titleUI);
        }
    }

    void OnPlayerCharacterEvent(in eUnitEvent ev, IUnit unit, in bool isParentType, in string file, in int line)
    {
        switch (ev)
        {
            case eUnitEvent.Create:
                {
                    CreatePlayerCharacterTitleUI(unit);
                    unit.RegistLateUpdate(UpdateCharacterTitleUI);
                    break;
                }
            case eUnitEvent.Released:
                {
                    ReleasePlayerCharacterTitleUI(unit);
                    break;
                }
        }
    }
}

