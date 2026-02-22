using System;
using System.Collections.Generic;
using AutoConfig;
using GameUnit;
using NUnit.Framework.Internal;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;
using static GameUnitApi;

public interface IUILootItemLab : IModule
{
    public void InitAndAddClickUI(IUnit unit);//添加物品Item
    public void ReleaseItem(IUnit unit);//移除物品Item
}

class UILootItemLab : IUILootItemLab
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IUILootItemLab, UILootItemLab>();
    }
    ICharacterManager _CharacterManager;
    IClient _Client;

    XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();
    Dictionary<UInt64, GameObject> _LootItems = new Dictionary<UInt64, GameObject>();
    bool _IsShow = false;
    int _LootItemCount = 0;

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _Client = Launcher.GetModule<IClient>();
        return true;
    }

    public void Release()
    {
        _LootItemCount = 0;
        _LootItems.Clear();
    }

    public void InitAndAddClickUI(IUnit unit)//初始化点击UI和显示内容
    {
        GameObject itemPre = Resources.Load<GameObject>("UI/Prefabs/LootLab/LootItem");
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        Transform itemLab = canvas.transform.Find("LootLab/Viewport/Content").gameObject.transform;
        if (_IsShow == true)
        {
            if (!_LootItems.ContainsKey(unit.Guid))
            {
                canvas.transform.Find("LootLab").gameObject.SetActive(true);
                GameObject instantiatedItem = GameObject.Instantiate(itemPre);
                GameObject item = InitItem(unit,instantiatedItem);
                item.transform.SetParent(itemLab, false);
                item.transform.localScale = Vector3.one;

                _LootItems[unit.Guid] = item;
                _LootItemCount++;
            }

        }
        else
        {
            _IsShow = true;
            for (int i = 0; i < itemLab.childCount; i++)
            {
                GameObject.Destroy(itemLab.GetChild(i).gameObject);
            }
            _LootItemCount = 0;
            GameObject instantiatedItem = GameObject.Instantiate(itemPre);
            GameObject item = InitItem(unit,instantiatedItem);
            item.transform.SetParent(itemLab, false);
            item.transform.localScale = Vector3.one;
            canvas.transform.Find("LootLab").gameObject.SetActive(true);


            _LootItems[unit.Guid] = item;
            _LootItemCount++;
        }
    }

    GameObject InitItem(IUnit unit, GameObject item)//初始化物品Item
    {
        Int64 resID = AutoConfig.Resource.Get(unit.ConfigID)._RelevantID;
        AutoEnum.eResources resType = AutoConfig.Resource.Get(unit.ConfigID)._ResourceType;
        string picBG = "";
        string name = "";
        Int64 quality = 0;
        string img = "";
        switch (resType)
        {
            case AutoEnum.eResources.Material://素材
                picBG = AutoConfig.Quality.Get(AutoConfig.Material.Get(resID)._Quality)._QualityPicture;
                name = AutoConfig.Material.Get(resID)._Name;
                quality = AutoConfig.Material.Get(resID)._Quality;
                img = AutoConfig.Material.Get(resID)._Icon;
                break;
            case AutoEnum.eResources.Special://特殊
                picBG = AutoConfig.Quality.Get(AutoConfig.Special.Get(resID)._Quality)._QualityPicture;
                name = AutoConfig.Special.Get(resID)._Name;
                quality = AutoConfig.Special.Get(resID)._Quality;
                img = AutoConfig.Special.Get(resID)._Icon;
                break;
            case AutoEnum.eResources.Brass:// brass
                break;
            case AutoEnum.eResources.Coin:// 金币
                break;
            case AutoEnum.eResources.Consume:// 消耗品
                picBG = AutoConfig.Quality.Get(AutoConfig.Consume.Get(resID)._Quality)._QualityPicture;
                name = AutoConfig.Consume.Get(resID)._Name;
                quality = AutoConfig.Consume.Get(resID)._Quality;
                img = AutoConfig.Consume.Get(resID)._Icon;
                break;
            case AutoEnum.eResources.Equipment:// 装备
                picBG = AutoConfig.Quality.Get(AutoConfig.Equipment.Get(resID)._Quality)._QualityPicture;
                name = AutoConfig.Equipment.Get(resID)._Name;
                quality = AutoConfig.Equipment.Get(resID)._Quality;
                img = AutoConfig.Equipment.Get(resID)._Icon;
                break;
            case AutoEnum.eResources.FatigueValue:// 疲劳值
                break;
        }
        item.transform.Find("BG").GetComponent<Image>().sprite = Resources.Load<Sprite>(picBG);
        item.transform.Find("Name").GetComponent<TextMeshProUGUI>().text = name + " " + unit.GetInt32(eItemAttr.Count_Int32).ToString();
        float[] color = AutoConfig.Quality.Get(quality)._QualityColor;
        item.transform.Find("Name").GetComponent<TextMeshProUGUI>().color = new Color(color[0], color[1], color[2]);
        //string img = AutoConfig.Material.Get(AutoConfig.Resource.Get(unit.ConfigID)._RelevantID)._Icon;
        item.transform.Find("Icon").GetComponent<Image>().sprite = Resources.Load<Sprite>(img);

        item.GetComponent<Button>().onClick.RemoveAllListeners();
        item.GetComponent<Button>().onClick.AddListener(() =>//点击拾取
        {
            GetItem(unit);
        });

        return item;
    }

    private void GetItem(IUnit unit)
    {
        Debug.LogError("拾取物品" + unit.ConfigID + "数量" + unit.GetInt32(eItemAttr.Count_Int32));
        IUnit check = GameUnitApi.QueryUnit(unit.Guid);
        if (check == null)
        {
            return;
        }
        oItemPickUpReq req = new oItemPickUpReq();
        req.ItemGuid = unit.Guid;
        _Client.SendProtobuf(eClientID.ItemPickUpReq, req);
        ReleaseItem(unit);
    }

    public void ReleaseItem(IUnit unit)//移除物品Item
    {
        if (_LootItems.ContainsKey(unit.Guid))
        {
            GameObject itemgo = _LootItems[unit.Guid];
            if (itemgo != null)
            {
                Debug.LogError("移除列表内的物品" + unit.Guid);
                GameObject.Destroy(itemgo);
                //GameObject.DestroyImmediate(itemgo, true);
            }
            _LootItems.Remove(unit.Guid);
            _LootItemCount--;
        }
        if (_LootItemCount <= 0)
        {
            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
            canvas.transform.Find("LootLab").gameObject.SetActive(false);
            _IsShow = false;
            _LootItemCount = 0;
        }
    }

    public void HideLootUI()
    {
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        Transform itemLab = canvas.transform.Find("LootLab").gameObject.transform;
        itemLab.gameObject.SetActive(false);
        _IsShow = false;
        _LootItems.Clear();
        _LootItemCount = 0;
    }

}
