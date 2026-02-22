using System;
using UnityEngine;
using XGame;
using UnityEngine.UI;
using GameUnit;
using XEngine;
using XUtils;
using static GameUnitApi;
using Spine.Unity;
using AutoConfig;

public interface ILootItemManager : IModule
{
    void LootItemAddIMG(GameObject gameObject, long configId);
}

class LootItemManager : ILootItemManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<ILootItemManager, LootItemManager>();
    }

    ICharacterManager _CharacterManager;
    IGameObjectSync _GameObjectSync;
    XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _GameObjectSync = Launcher.GetModule<IGameObjectSync>();
        _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncCreate, (IUnit goUnit) =>
        {
            if (goUnit.Type == GameUnit.eUnitType.Item)
            {
                OnLootItemCreate(goUnit);
            }
        });
        _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncRelease, (IUnit goUnit) =>
        {
            if (goUnit.Type == GameUnit.eUnitType.Item)
            {
                OnLootItemRelease(goUnit);
            }
        });
        return true;
    }

    public void Release()
    {

    }

    private void OnLootItemCreate(IUnit goUnit)//掉落物品创建
    {
        bool firstcreate = goUnit.GetBool(eItemAttr.IsFirstCreate_Bool);

        if (firstcreate == true)//第一次创建
        {
            goUnit.SetBool(eItemAttr.IsFirstCreate_Bool, false);
            Vector2 startPos = goUnit.GetVector2(eItemAttr.StartPos_Vector2);
            Vector2 endPos = goUnit.GetVector2(eItemAttr.EndPos_Vector2);
            Debug.LogError("开始物品掉落,起点坐标是" + startPos+",终点坐标是"+endPos);
            goUnit.RegistUpdate(UpdateMoveItem);

            goUnit.SetInt64(eItemAttr.StartTime_Int64, SafeSystem.Time.GetMillisecondTick());

        }
        else//非第一次创建
        {
            Vector2 startPos = goUnit.GetVector2(eItemAttr.StartPos_Vector2);
            Vector2 endPos = goUnit.GetVector2(eItemAttr.EndPos_Vector2);
            // 检查物品是否已经到达终点或不需要移动
            Int64 startTime = goUnit.GetInt64(eItemAttr.StartTime_Int64);
            Int64 totalTime = AutoConst.GameSetting.DropTime;
            Int64 currentTime = SafeSystem.Time.GetMillisecondTick();
            Int64 elapsed = currentTime - startTime;

            if (elapsed < totalTime)
            {
                goUnit.RegistUpdate(UpdateMoveItem);
            }
            else
            {
                goUnit.Position = new Vector3(endPos.x, endPos.y);
                goUnit.CancelRegistUpdate(UpdateMoveItem);
            }
        }
        if (goUnit.Type == eUnitType.Item)
        {
            CircleCollider2D collider = goUnit.GetComponent<CircleCollider2D>();
            if (collider != null)
            {
                collider.radius = AutoConst.GameSetting.ThingArea;
            }
            
        }
    }

    private void UpdateMoveItem(IUnit unit)
    {
        Debug.LogError("开始物品掉落,起点坐标是" + unit.Position+",终点坐标是"+unit.GetVector2(eItemAttr.EndPos_Vector2));
        Int64 totalTime = AutoConst.GameSetting.DropTime;
        Int64 startTime = unit.GetInt64(eItemAttr.StartTime_Int64);
        Int64 currentTime = SafeSystem.Time.GetMillisecondTick();
        Int64 elapsed = currentTime - startTime;//已用的时间
        Vector2 endpos = unit.GetVector2(eItemAttr.EndPos_Vector2);
        Vector2 startpos = unit.GetVector2(eItemAttr.StartPos_Vector2);

        if (elapsed >= totalTime)
        {

            unit.Position = new Vector3(endpos.x, endpos.y);
            unit.CancelRegistUpdate(UpdateMoveItem);
            return;
        }
        float progress = Mathf.Clamp01(elapsed / totalTime);

        Vector2 controlpos = unit.GetVector2(eItemAttr.CotrolPoint_Vector2);
        Vector2 currentPosition = CalculateQuadraticBezierPoint(progress, startpos, controlpos, endpos);
        unit.Position = new Vector3(currentPosition.x, currentPosition.y);
    }

    private Vector2 CalculateQuadraticBezierPoint(float t, Vector2 p0, Vector2 p1, Vector2 p2)
    {
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float ut2 = 2 * u * t;

        Vector2 point = uu * p0 + ut2 * p1 + tt * p2;
        return point;
    }

    private void OnLootItemRelease(IUnit goUnit)//掉落物品释放
    {
        if (goUnit == null)
            return;

        goUnit.CancelRegistUpdate(UpdateMoveItem);
    }

    public void LootItemAddIMG(GameObject gameObject, long configId)
    {
        AutoEnum.eResources resourceType = AutoConfig.Resource.Get(configId)._ResourceType;
        switch (resourceType)
        {
            case AutoEnum.eResources.Material://素材
                string img = AutoConfig.Material.Get(configId)._DropIcon;
                Sprite sprite = Resources.Load<Sprite>(img);
                gameObject.GetComponent<Image>().sprite = sprite;
                break;
            case AutoEnum.eResources.Special://特殊
                img = AutoConfig.Special.Get(configId)._DropIcon;
                sprite = Resources.Load<Sprite>(img);
                gameObject.GetComponent<Image>().sprite = sprite;
                break;
            case AutoEnum.eResources.Equipment://装备
                img = AutoConfig.Equipment.Get(configId)._Icon;
                sprite = Resources.Load<Sprite>(img);
                gameObject.GetComponent<Image>().sprite = sprite;
                break;
            case AutoEnum.eResources.Coin://金币
                img = AutoConst.GameSetting.CoinIcon;
                sprite = Resources.Load<Sprite>(img);
                gameObject.GetComponent<Image>().sprite = sprite;
                break;
            case AutoEnum.eResources.Consume://消耗品
                img = AutoConfig.Consume.Get(configId)._Icon;
                sprite = Resources.Load<Sprite>(img);
                gameObject.GetComponent<Image>().sprite = sprite;
                break;
            case AutoEnum.eResources.Brass://陨铜币
                img = AutoConst.GameSetting.BrassIcon;
                sprite = Resources.Load<Sprite>(img);
                gameObject.GetComponent<Image>().sprite = sprite;
                break;
        }
    }
}

