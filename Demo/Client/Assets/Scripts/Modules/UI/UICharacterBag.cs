// using Unity.VisualScripting;
using System;
using System.Collections.Generic;
using System.Linq;
using AutoConfig;
using GameUnit;
using Spine.Unity;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using static GameUnitApi;

public interface IUIBag : IModule
{

}

public class UIBag : IUIBag
{

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IUIBag, UIBag>();
    }

    IUIMain _UIMain;
    ICharacterManager _CharacterManager;

    GameObject _UIBag;
    IUnit _CharacterUnit;

    IClient _Client;
    IUICharacterInfo _UICharacterInfo;
    IWeaponManager _WeaponManager;
    oBackpackItemPush _BackpackData;
    List<oGameUnit> _CurrentTabItems;

    Dictionary<GameObject, oGameUnit> _ItemToGameUnitMap = new Dictionary<GameObject, oGameUnit>();

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _UICharacterInfo = Launcher.GetModule<IUICharacterInfo>();
        _Client.RegistProtobuf<oBackpackItemPush>(eServerID.BackpackItemPush, OnBackpackData);
        _Client.RegistProtobuf<oEquipAsw>(eServerID.EquipAsw, OnEquipAsw);
        _Client.RegistProtobuf<oEquipAsw>(eServerID.UnEquipAsw, OnUnEquipAsw);
        _WeaponManager = Launcher.GetModule<IWeaponManager>();

        _UIMain = Launcher.GetModule<IUIMain>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _UIMain.RegistEvent(IUIMain.eEvent.OpenCharacterBag, OpenBag);
        return true;
    }

    private void OnBackpackData(oBackpackItemPush body)
    {
        if (body == null)
        {
            XApi.Debug("OnBackpackData body is null.");
            return;
        }
        _BackpackData = body;

        _CharacterUnit = _CharacterManager.GetMainCharacterUnit();
        ulong weaponID = _CharacterUnit.GetUInt64(eCharacterAttr.SlotWeapon_UInt64);

        oGameUnit weapon = null;
        foreach (var item in _BackpackData.BackpackItem)
        {
            if (item.Guid == weaponID)
            {
                weapon = item;
                break;
            }
        }
        if (weaponID != 0)
        {
            _WeaponManager.CharacterChangeWeapon(weapon.ConfigId, _CharacterUnit);
        }
    }

    private void OnBackpackItemUpdate(List<oGameUnit> updatedItems)//背包物品修改
    {
        if (_BackpackData == null || updatedItems == null || updatedItems.Count <= 0)
            return;
        bool needRefreshUI = false;
        // 查找GUID物品
        foreach (var updatedItem in updatedItems)
        {
            var existingItem = _BackpackData.BackpackItem.FirstOrDefault(item => item.Guid == updatedItem.Guid);
            if (existingItem != null)
            {
                if (updatedItem.Attributes.FirstOrDefault(attr => attr.Index == eItemAttr.Count_Int32) != null &&
                    updatedItem.Attributes.FirstOrDefault(attr => attr.Index == eItemAttr.Count_Int32).Value.Int <= 0)
                {
                    _BackpackData.BackpackItem.Remove(existingItem);
                    needRefreshUI = true;
                }
                else
                {
                    needRefreshUI = true;
                }
            }
            else
            {
                _BackpackData.BackpackItem.Add(updatedItem);
                needRefreshUI = true;
            }
        }
        if (_UIBag != null && _UIBag.activeSelf && needRefreshUI)//如果是背包打开着的时候数据更新
        {
            InitBagUI();
        }
    }

    // 根据指定 itemType 获取物品列表
    public List<oGameUnit> GetItemsByItemtype(AutoConfig.AutoEnum.eResources itemtype)
    {
        if (_BackpackData == null) return new List<oGameUnit>();
        int itemTypeValue = (int)itemtype;
        return _BackpackData.BackpackItem.Where(item =>
        item.Attributes.Any(attr =>
            attr.Index == (int)eItemAttr.ItemType_Int32 &&
            attr.Value.Int == itemTypeValue)).ToList();
    }

    void OpenBag()
    {
        _UIMain.UnRegistEvent(IUIMain.eEvent.OpenCharacterBag, OpenBag);
        if (_UIBag == null)
        {
            GameObject uiRes = Resources.Load<GameObject>("UI/Prefabs/BagUI/UIBag");
            _UIBag = GameObject.Instantiate(uiRes);
            _UIBag.transform.parent = GameObject.FindFirstObjectByType<Canvas>().gameObject.transform;
            _UIBag.GetComponent<RectTransform>().offsetMax = new Vector2(0, 0);
            _UIBag.GetComponent<RectTransform>().offsetMin = new Vector2(0, 0);
            _UIBag.transform.localScale = Vector3.one;

            InitBagUI();
        }
        else
        {
            _UIBag.SetActive(true);
            InitBagUI();
        }
    }

    void InitBagUI()//初始化背包UI
    {
        ListenerBtnClickEvent();

        //_CharacterUnit = _CharacterManager.GetMainCharacterUnit();
        if (_CharacterUnit == null)
        {
            XApi.Debug("CharacterUnit is null.");
            return;
        }
        TextMeshProUGUI characterAfford = _UIBag.transform.Find("BagUI/ChracterDetail/Afford/TextCount").GetComponent<TextMeshProUGUI>();//负重
        if (characterAfford == null)
        {
            XApi.Debug("characterAafford is null.");
            return;
        }
        characterAfford.text = _BackpackData.TotalWeight.ToString() + "/" + _CharacterUnit.GetFloat(eCharacterAttr.Afford_Float).ToString();

        TextMeshProUGUI characterLV = _UIBag.transform.Find("BagUI/ChracterDetail/TextLevel").GetComponent<TextMeshProUGUI>();//等级
        if (characterLV == null)
        {
            XApi.Debug("characterLV is null.");
            return;
        }
        characterLV.text = "LV " + _CharacterUnit.GetInt64(eCharacterAttr.Level_Int64).ToString();

        Transform bagItemList = _UIBag.transform.Find("BagUI/Resource/BagView/Viewport/Content");//获得背包lab
        Transform mask = _UIBag.transform.Find("EquipmentOperate/Mask");
        if (mask != null)
        {
            Button maskButton = mask.GetComponent<Button>();
            if (maskButton == null)
            {
                maskButton = mask.gameObject.AddComponent<Button>();
            }

            maskButton.onClick.RemoveAllListeners();
            maskButton.onClick.AddListener(() =>
            {
                mask.gameObject.SetActive(false);
                _UIBag.transform.Find("EquipmentOperate/EmptyOperater").gameObject.SetActive(false);
                _UIBag.transform.Find("EquipmentOperate/MiniOperater").gameObject.SetActive(false);
                _UIBag.transform.Find("EquipmentOperate/UseOperater").gameObject.SetActive(false);
            });
        }
        _UIBag.transform.Find("EquipmentOperate/EmptyOperater").gameObject.SetActive(false);
        _UIBag.transform.Find("EquipmentOperate/MiniOperater").gameObject.SetActive(false);
        _UIBag.transform.Find("EquipmentOperate/UseOperater").gameObject.SetActive(false);
        mask.gameObject.SetActive(false);
        InitCharacterSpine();
        CheckPage(AutoConfig.AutoEnum.eResources.Equipment);
    }

    void OnItemClick(oGameUnit item)//物品点击事件
    {
        GameObject mask = _UIBag.transform.Find("EquipmentOperate/Mask").gameObject;
        mask.SetActive(true);

        AutoConfig.AutoEnum.eResources itemtype = (AutoConfig.AutoEnum.eResources)item.Attributes[eItemAttr.ItemType_Int32].Value.Int;
        switch (itemtype)
        {
            case AutoConfig.AutoEnum.eResources.Equipment://武器装备
                _UIBag.transform.Find("EquipmentOperate/EmptyOperater").gameObject.SetActive(true);
                InitWeaponItem(item);
                break;
            case AutoConfig.AutoEnum.eResources.Consume://消耗品
                _UIBag.transform.Find("EquipmentOperate/UseOperater").gameObject.SetActive(true);
                InitUseItem(item);
                break;
            case AutoConfig.AutoEnum.eResources.Material://材料
                _UIBag.transform.Find("EquipmentOperate/UseOperater").gameObject.SetActive(true);
                InitUseItem(item);
                break;
            case AutoConfig.AutoEnum.eResources.Special://特殊
                _UIBag.transform.Find("EquipmentOperate/UseOperater").gameObject.SetActive(true);
                InitUseItem(item);
                break;
        }
    }

    void InitWeaponItem(oGameUnit item)
    {
        TextMeshProUGUI name = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/TextEquipmentName").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI type = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/TextTypeName").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI whoCanUes = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/WhoUse").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI inUse = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/TextEquipStatement").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI lvControl = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/LevelDistinct").GetComponent<TextMeshProUGUI>();
        Image pic = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/WeaponShow").GetComponent<Image>();
        TextMeshProUGUI tolerancCount = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/TolerancText/TextCount").GetComponent<TextMeshProUGUI>();//耐久度
        Image skillpic = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/SkillWeapon").GetComponent<Image>();
        TextMeshProUGUI skillDes = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/SkillWeapon/TextDes").GetComponent<TextMeshProUGUI>();
        GameObject equpiBtn = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/BthEquip").gameObject;
        GameObject unloadBtn = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/BthUnload").gameObject;

        TextMeshProUGUI miniName = _UIBag.transform.Find("EquipmentOperate/MiniOperater/Operater/TextEquipmentName").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI minitype = _UIBag.transform.Find("EquipmentOperate/MiniOperater/Operater/TextTypeName").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI miniWhoCanUes = _UIBag.transform.Find("EquipmentOperate/MiniOperater/Operater/WhoUse").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI miniInUse = _UIBag.transform.Find("EquipmentOperate/MiniOperater/Operater/TextEquipStatement").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI minilvcount = _UIBag.transform.Find("EquipmentOperate/MiniOperater/Operater/LevelDistinct").GetComponent<TextMeshProUGUI>();
        Image miniPic = _UIBag.transform.Find("EquipmentOperate/MiniOperater/Operater/WeaponShow").GetComponent<Image>();
        TextMeshProUGUI miniTolerancCount = _UIBag.transform.Find("EquipmentOperate/MiniOperater/Operater/TolerancText/TextCount").GetComponent<TextMeshProUGUI>();//耐久度
        Image miniSkillpic = _UIBag.transform.Find("EquipmentOperate/MiniOperater/Operater/SkillWeapon").GetComponent<Image>();
        TextMeshProUGUI miniSkillDes = _UIBag.transform.Find("EquipmentOperate/MiniOperater/Operater/SkillWeapon/TextDes").GetComponent<TextMeshProUGUI>();

        AutoEnum.eEquipmentType equipType = (AutoEnum.eEquipmentType)item.Attributes[eEquipAttr.EquipType_Int32].Value.Int;
        switch (equipType)
        {
            case AutoEnum.eEquipmentType.Belt:

                break;
            case AutoEnum.eEquipmentType.Cloths:
                break;
            case AutoEnum.eEquipmentType.Jewelry:
                break;
            case AutoEnum.eEquipmentType.Pants:
                break;
            case AutoEnum.eEquipmentType.Shose:
                break;
            case AutoEnum.eEquipmentType.Weapon:
                break;
            default:
                break;
        }

        name.text = AutoConfig.Equipment.Get(item.ConfigId)._Name;
        type.text = AutoConfig.Equipment.Get(item.ConfigId)._EquipmentTypeName;
        long[] useLab = AutoConfig.Equipment.Get(item.ConfigId)._CharacterUse;
        string characterNames = "可使用：";
        for (int i = 0; i < useLab.Length; i++)
        {
            string characterName = AutoConfig.Character.Get(useLab[i])._Name;
            if (i == 0)
            {
                characterNames = characterName;
            }
            else
            {
                characterNames += ", " + characterName;
            }
        }
        whoCanUes.text = characterNames;
        UInt64 currentWeapon = _CharacterUnit.GetUInt64(eCharacterAttr.SlotWeapon_UInt64);
        if (currentWeapon == item.Guid)
        {
            inUse.text = "（穿戴中）";
        }
        else
        {
            inUse.text = "（未穿戴）";
        }
        lvControl.text = "等级限制：" + "LV" + AutoConfig.Equipment.Get(item.ConfigId)._LevelLimit;
        string equipment = AutoConfig.Equipment.Get(item.ConfigId)._Icon;
        if (!string.IsNullOrEmpty(equipment))
        {
            Sprite iconSprite = Resources.Load<Sprite>(equipment);
            if (iconSprite != null)
            {
                pic.sprite = iconSprite;
            }
            else
            {
                XApi.Debug("加载武器图标失败：" + equipment);
            }
        }
        equpiBtn.GetComponent<Button>().onClick.RemoveAllListeners();
        unloadBtn.GetComponent<Button>().onClick.RemoveAllListeners();
        equpiBtn.GetComponent<Button>().onClick.AddListener(() =>//装备
        {
            _CharacterManager.WearEquip(item.Guid);
            _UIBag.transform.Find("EquipmentOperate/EmptyOperater").gameObject.SetActive(false);
            _UIBag.transform.Find("EquipmentOperate/MiniOperater").gameObject.SetActive(false);
            _UIBag.transform.Find("EquipmentOperate/UseOperater").gameObject.SetActive(false);
            _UIBag.transform.Find("EquipmentOperate/Mask").gameObject.SetActive(false);
        });

        unloadBtn.GetComponent<Button>().onClick.AddListener(() =>//卸下
        {
            _CharacterManager.UnWearEquip(item.Guid);
            _UIBag.transform.Find("EquipmentOperate/EmptyOperater").gameObject.SetActive(false);
            _UIBag.transform.Find("EquipmentOperate/MiniOperater").gameObject.SetActive(false);
            _UIBag.transform.Find("EquipmentOperate/UseOperater").gameObject.SetActive(false);
            _UIBag.transform.Find("EquipmentOperate/Mask").gameObject.SetActive(false);
        });

        if (_CharacterUnit.GetUInt64(eCharacterAttr.SlotWeapon_UInt64) == item.Guid)
        {
            equpiBtn.SetActive(false);
            unloadBtn.SetActive(true);
        }
        else
        {
            equpiBtn.SetActive(true);
            unloadBtn.SetActive(false);
        }

        switch (AutoConfig.Equipment.Get(item.ConfigId)._EquipmentType)
        {
            case AutoConfig.AutoEnum.eEquipmentType.Belt:

                break;
            case AutoConfig.AutoEnum.eEquipmentType.Cloths:

                break;
            case AutoConfig.AutoEnum.eEquipmentType.Jewelry:

                break;
            case AutoConfig.AutoEnum.eEquipmentType.Pants:

                break;
            case AutoConfig.AutoEnum.eEquipmentType.Shose:

                break;
            case AutoConfig.AutoEnum.eEquipmentType.Weapon:

                break;
        }
    }

    void InitUseItem(oGameUnit item)
    {
        TextMeshProUGUI name = _UIBag.transform.Find("EquipmentOperate/UseOperater/Operater/DescribeName").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI type = _UIBag.transform.Find("EquipmentOperate/UseOperater/Operater/TextTypeName").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI whoCanUes = _UIBag.transform.Find("EquipmentOperate/UseOperater/Operater/WhoUse").GetComponent<TextMeshProUGUI>();
        TextMeshProUGUI lvControl = _UIBag.transform.Find("EquipmentOperate/UseOperater/Operater/LevelDistinct").GetComponent<TextMeshProUGUI>();
        Image pic = _UIBag.transform.Find("EquipmentOperate/UseOperater/Operater/ItemShow").GetComponent<Image>();
        TextMeshProUGUI Effect = _UIBag.transform.Find("EquipmentOperate/UseOperater/Operater/DescribeItem").GetComponent<TextMeshProUGUI>();
        //消耗品在consume，材料在metral，特殊在special
        switch ((AutoConfig.AutoEnum.eResources)item.Type)
        {
            case AutoConfig.AutoEnum.eResources.Consume:
                name.text = AutoConfig.Consume.Get(item.ConfigId)._Name;
                type.text = AutoConfig.Consume.Get(item.ConfigId)._ConsumeName;
                long[] useLab = AutoConfig.Consume.Get(item.ConfigId)._CharacterUse;
                string characterNames = "";
                for (int i = 0; i < useLab.Length; i++)
                {
                    string characterName = AutoConfig.Character.Get(useLab[i])._Name;
                    if (i == 0)
                    {
                        characterNames = characterName;
                    }
                    else
                    {
                        characterNames += ", " + characterName;
                    }
                }
                whoCanUes.text = characterNames;
                lvControl.text = "LV" + AutoConfig.CharacterLevel.Get(AutoConfig.Consume.Get(item.ConfigId)._LevelLimit)._ID;
                pic.sprite = Resources.Load<Sprite>(AutoConfig.Consume.Get(item.ConfigId)._Icon);
                Effect.text = AutoConfig.Consume.Get(item.ConfigId)._Des;
                break;
            case AutoConfig.AutoEnum.eResources.Material:
                name.text = AutoConfig.Material.Get(item.ConfigId)._Name;
                type.text = AutoConfig.Material.Get(item.ConfigId)._MaterialTypeName;
                whoCanUes.text = "";
                lvControl.text = "";
                pic.sprite = Resources.Load<Sprite>(AutoConfig.Material.Get(item.ConfigId)._Icon);
                Effect.text = AutoConfig.Material.Get(item.ConfigId)._Des;
                break;
            case AutoConfig.AutoEnum.eResources.Special:
                name.text = AutoConfig.Special.Get(item.ConfigId)._Name;
                type.text = AutoConfig.Special.Get(item.ConfigId)._SpecialTypeName;
                whoCanUes.text = "";
                lvControl.text = "";
                pic.sprite = Resources.Load<Sprite>(AutoConfig.Special.Get(item.ConfigId)._Icon);
                Effect.text = AutoConfig.Special.Get(item.ConfigId)._Des;
                break;
        }
    }
    void ListenerBtnClickEvent()//监听按钮点击事件
    {
        Button closeBtn = _UIBag.transform.Find("BagUI/Close").GetComponent<Button>();
        closeBtn.onClick.RemoveAllListeners();
        closeBtn.onClick.AddListener(() =>
        {
            CloseBagUI();
        });

        Button characterBtn = _UIBag.transform.Find("BagUI/ChracterDetail/ButtonCharacter").GetComponent<Button>();
        characterBtn.onClick.RemoveAllListeners();
        characterBtn.onClick.AddListener(() =>
        {
            _UICharacterInfo.OpenCharacterInfo();
        });

        Button equipmentTabBtn = _UIBag.transform.Find("BagUI/Tab/TabEquipmentselected").GetComponent<Button>();
        equipmentTabBtn.onClick.RemoveAllListeners();
        equipmentTabBtn.onClick.AddListener(() =>//武器
        {
            CheckPage(AutoConfig.AutoEnum.eResources.Equipment);
        });

        Button consumeTabBtn = _UIBag.transform.Find("BagUI/Tab/TabConsumeselected").GetComponent<Button>();
        consumeTabBtn.onClick.RemoveAllListeners();
        consumeTabBtn.onClick.AddListener(() =>//消耗品
        {
            CheckPage(AutoConfig.AutoEnum.eResources.Consume);
        });

        Button materialTabBtn = _UIBag.transform.Find("BagUI/Tab/TabRawmaterialselected").GetComponent<Button>();
        materialTabBtn.onClick.RemoveAllListeners();
        materialTabBtn.onClick.AddListener(() =>//材料
        {
            CheckPage(AutoConfig.AutoEnum.eResources.Material);
        });

        Button specialTabBtn = _UIBag.transform.Find("BagUI/Tab/TabSpecialselected").GetComponent<Button>();
        specialTabBtn.onClick.RemoveAllListeners();
        specialTabBtn.onClick.AddListener(() =>//特殊
        {
            CheckPage(AutoConfig.AutoEnum.eResources.Special);
        });

        Button BattleBagBtn = _UIBag.transform.Find("BagUI/BattleBagBtn").GetComponent<Button>();
        BattleBagBtn.onClick.RemoveAllListeners();
        BattleBagBtn.onClick.AddListener(() =>//战斗背包
        {
            OpenBattleBag();
        });

    }

    private void OpenBattleBag()
    {
        _UIBag.transform.Find("BagUI/BattleBag").gameObject.SetActive(true);
        Button closebtn = _UIBag.transform.Find("BagUI/BattleBag/Close").GetComponent<Button>();
        closebtn.onClick.RemoveAllListeners();
        closebtn.onClick.AddListener(() =>//关闭战斗背包
        {
            _UIBag.transform.Find("BagUI/BattleBag").gameObject.SetActive(false);
            RestoreNormalBagItemEvents();//恢复背包物品点击事件

        });
        CancelAllItemEvents();//取消背包物品点击事件
        RegisterBattleBagItemEvents();//注册战斗背包物品点击事件
    }

    private void RestoreNormalBagItemEvents()//恢复背包物品点击事件
    {
        Transform bagItemList = _UIBag.transform.Find("BagUI/Resource/BagView/Viewport/Content");
        for (int i = 0; i < bagItemList.childCount; i++)
        {
            Transform child = bagItemList.GetChild(i);
            Button itembtn = child.GetComponent<Button>();
            if (itembtn != null && _ItemToGameUnitMap.ContainsKey(child.gameObject))
            {
                oGameUnit item = _ItemToGameUnitMap[child.gameObject];

                itembtn.onClick.RemoveAllListeners();
                itembtn.onClick.AddListener(() =>
                {
                    OnItemClick(item);
                });
            }
        }
    }

    private void CancelAllItemEvents()
    {
        Transform bagItemList = _UIBag.transform.Find("BagUI/Resource/BagView/Viewport/Content");
        for(int i = 0; i < bagItemList.childCount; i++)
        {
            Transform child = bagItemList.GetChild(i);
            Button itemButton = child.GetComponent<Button>();
            if(itemButton != null)
            {
                itemButton.onClick.RemoveAllListeners();
            }
        }
    }

    private void  RegisterBattleBagItemEvents()//注册战斗背包物品点击事件
    {
        Transform bagItemList = _UIBag.transform.Find("BagUI/Resource/BagView/Viewport/Content");
        for(int i = 0; i < bagItemList.childCount; i++)
        {
            Transform child = bagItemList.GetChild(i);
            Button itemButton = child.GetComponent<Button>();
            if(itemButton!= null && _ItemToGameUnitMap.ContainsKey(child.gameObject))
            {
                oGameUnit item = _ItemToGameUnitMap[child.gameObject];

                itemButton.onClick.RemoveAllListeners();
                itemButton.onClick.AddListener(() =>
                {
                   OnBattleBagItemClick(item);
                });
            }
        }
    }

    private void OnBattleBagItemClick(oGameUnit item)
    {
        Debug.LogError("点击了战斗背包物品" + item);
    }

    void CheckPage(AutoConfig.AutoEnum.eResources itemtype)
    {
        Transform bagItemList = _UIBag.transform.Find("BagUI/Resource/BagView/Viewport/Content");
        for (int i = bagItemList.childCount - 1; i >= 0; i--)
        {
            GameObject.Destroy(bagItemList.GetChild(i).gameObject);
        }

        switch (itemtype)
        {
            case AutoConfig.AutoEnum.eResources.Equipment:
                _UIBag.transform.Find("BagUI/Tab/TabEquipmentselected/Light").gameObject.SetActive(true);
                _UIBag.transform.Find("BagUI/Tab/TabConsumeselected/Light").gameObject.SetActive(false);
                _UIBag.transform.Find("BagUI/Tab/TabRawmaterialselected/Light").gameObject.SetActive(false);
                _UIBag.transform.Find("BagUI/Tab/TabSpecialselected/Light").gameObject.SetActive(false);
                CreateItemsForTab(AutoConfig.AutoEnum.eResources.Equipment, bagItemList);
                break;
            case AutoConfig.AutoEnum.eResources.Consume:
                _UIBag.transform.Find("BagUI/Tab/TabConsumeselected/Light").gameObject.SetActive(true);
                _UIBag.transform.Find("BagUI/Tab/TabEquipmentselected/Light").gameObject.SetActive(false);
                _UIBag.transform.Find("BagUI/Tab/TabRawmaterialselected/Light").gameObject.SetActive(false);
                _UIBag.transform.Find("BagUI/Tab/TabSpecialselected/Light").gameObject.SetActive(false);
                CreateItemsForTab(AutoConfig.AutoEnum.eResources.Consume, bagItemList);
                break;
            case AutoConfig.AutoEnum.eResources.Material:
                _UIBag.transform.Find("BagUI/Tab/TabRawmaterialselected/Light").gameObject.SetActive(true);
                _UIBag.transform.Find("BagUI/Tab/TabEquipmentselected/Light").gameObject.SetActive(false);
                _UIBag.transform.Find("BagUI/Tab/TabConsumeselected/Light").gameObject.SetActive(false);
                _UIBag.transform.Find("BagUI/Tab/TabSpecialselected/Light").gameObject.SetActive(false);
                CreateItemsForTab(AutoConfig.AutoEnum.eResources.Material, bagItemList);
                break;
            case AutoConfig.AutoEnum.eResources.Special:
                _UIBag.transform.Find("BagUI/Tab/TabSpecialselected/Light").gameObject.SetActive(true);
                _UIBag.transform.Find("BagUI/Tab/TabEquipmentselected/Light").gameObject.SetActive(false);
                _UIBag.transform.Find("BagUI/Tab/TabConsumeselected/Light").gameObject.SetActive(false);
                _UIBag.transform.Find("BagUI/Tab/TabRawmaterialselected/Light").gameObject.SetActive(false);
                CreateItemsForTab(AutoConfig.AutoEnum.eResources.Special, bagItemList);
                break;
        }

    }
    void CreateItemsForTab(AutoConfig.AutoEnum.eResources itemType, Transform parent)
    {
        _CurrentTabItems = GetItemsByItemtype(itemType);
        _ItemToGameUnitMap.Clear();//清空

        foreach (oGameUnit item in _CurrentTabItems)
        {
            GameObject itemPrefab = Resources.Load<GameObject>("UI/Prefabs/BagUI/UIBagItem");

            if (itemPrefab != null)
            {
                bool isEquipped = item.Attributes[eEquipAttr.Equiped_Bool].Value.Bool;
                if (isEquipped == false)
                {
                    GameObject itemGO = GameObject.Instantiate(itemPrefab, parent);
                    //设置相关属性
                    Button itemButton = itemGO.GetComponent<Button>();
                    oGameUnit currentItem = item;
                    itemButton.onClick.RemoveAllListeners();
                    itemButton.onClick.AddListener(() =>
                    {
                        OnItemClick(currentItem);
                    });
                    _ItemToGameUnitMap.Add(itemGO, currentItem);//添加到字典中记录关系
                }
                else
                {
                    AutoEnum.eEquipmentType equipType = (AutoEnum.eEquipmentType)item.Attributes[eEquipAttr.EquipType_Int32].Value.Int;
                    switch (equipType)
                    {
                        case AutoEnum.eEquipmentType.Shose:
                            Transform shoseparent = _UIBag.transform.Find("BagUI/ChracterDetail/ShooseBottom/ShooseBG");
                            GameObject itemshose = GameObject.Instantiate(itemPrefab, shoseparent);
                            itemshose.transform.localPosition = Vector3.zero;
                            Button shoseitemButton = itemshose.GetComponent<Button>();
                            oGameUnit shoseItem = item;
                            shoseitemButton.onClick.RemoveAllListeners();
                            shoseitemButton.onClick.AddListener(() =>
                            {
                                OnItemClick(shoseItem);
                            });
                            _ItemToGameUnitMap.Add(itemshose, shoseItem);//添加到字典中记录关系
                            break;
                        case AutoEnum.eEquipmentType.Weapon:
                            Transform weaponparent = _UIBag.transform.Find("BagUI/ChracterDetail/WeaponBottom/WeaponBG");
                            GameObject itemweapon = GameObject.Instantiate(itemPrefab, weaponparent);
                            itemweapon.transform.localPosition = Vector3.zero;
                            Button weaponitemButton = itemweapon.GetComponent<Button>();
                            oGameUnit weaponItem = item;
                            weaponitemButton.onClick.RemoveAllListeners();
                            weaponitemButton.onClick.AddListener(() =>
                            {
                                OnItemClick(weaponItem);
                            });
                            _ItemToGameUnitMap.Add(itemweapon, weaponItem);//添加到字典中记录关系
                            break;
                        case AutoEnum.eEquipmentType.Pants:
                            Transform pantsparent = _UIBag.transform.Find("BagUI/ChracterDetail/PantsBottom/PantsBG");
                            GameObject itempants = GameObject.Instantiate(itemPrefab, pantsparent);
                            itempants.transform.localPosition = Vector3.zero;
                            Button pantsitemButton = itempants.GetComponent<Button>();
                            oGameUnit pantsItem = item;
                            pantsitemButton.onClick.RemoveAllListeners();
                            pantsitemButton.onClick.AddListener(() =>
                            {
                                OnItemClick(pantsItem);
                            });
                            _ItemToGameUnitMap.Add(itempants, pantsItem);//添加到字典中记录关系
                            break;
                        case AutoEnum.eEquipmentType.Cloths:
                            Transform clothsparent = _UIBag.transform.Find("BagUI/ChracterDetail/ClothsBottom/ClothsBG");
                            GameObject itemcloths = GameObject.Instantiate(itemPrefab, clothsparent);
                            itemcloths.transform.localPosition = Vector3.zero;
                            Button clothsitemButton = itemcloths.GetComponent<Button>();
                            oGameUnit clothsItem = item;
                            clothsitemButton.onClick.RemoveAllListeners();
                            clothsitemButton.onClick.AddListener(() =>
                            {
                                OnItemClick(clothsItem);
                            });
                            _ItemToGameUnitMap.Add(itemcloths, clothsItem);//添加到字典中记录关系
                            break;
                        case AutoEnum.eEquipmentType.Belt:
                            Transform beltparent = _UIBag.transform.Find("BagUI/ChracterDetail/BeltBottom/BeltBG");
                            GameObject itembelt = GameObject.Instantiate(itemPrefab, beltparent);
                            itembelt.transform.localPosition = Vector3.zero;
                            Button beltitemButton = itembelt.GetComponent<Button>();
                            oGameUnit beltItem = item;
                            beltitemButton.onClick.RemoveAllListeners();
                            beltitemButton.onClick.AddListener(() =>
                            {
                                OnItemClick(beltItem);
                            });
                            _ItemToGameUnitMap.Add(itembelt, beltItem);//添加到字典中记录关系
                            break;
                        case AutoEnum.eEquipmentType.Jewelry:
                            Transform jewelryparent = _UIBag.transform.Find("BagUI/ChracterDetail/JewelryBottom/JewelryBG");
                            GameObject itemjewelry = GameObject.Instantiate(itemPrefab, jewelryparent);
                            itemjewelry.transform.localPosition = Vector3.zero;
                            Button jewelryitemButton = itemjewelry.GetComponent<Button>();
                            oGameUnit jewelryItem = item;
                            jewelryitemButton.onClick.RemoveAllListeners();
                            jewelryitemButton.onClick.AddListener(() =>
                            {
                                OnItemClick(jewelryItem);
                            });
                            _ItemToGameUnitMap.Add(itemjewelry, jewelryItem);//添加到字典中记录关系
                            break;
                    }
                }
            }
        }
    }

    void InitCharacterSpine()
    {
        Transform charachterSpine = _UIBag.transform.Find("BagUI/ChracterDetail/Character/CharacterSpine");
        foreach (Transform child in charachterSpine)
        {
            GameObject.Destroy(child.gameObject);
        }
        long characterConfigID = _CharacterUnit.ConfigID;

        string spineRes = AutoConfig.Character.Get(characterConfigID)._PrefabShow;
        SkeletonDataAsset skeletonDataAsset = Resources.Load<SkeletonDataAsset>(spineRes);
        if (skeletonDataAsset == null)
        {
            XApi.Debug($"未找到角色{characterConfigID}的Spine资源{spineRes}");
            return;
        }
        GameObject skeletonGraphicItem = new GameObject("CharacterSpine");
        skeletonGraphicItem.transform.SetParent(charachterSpine, false);
        SkeletonGraphic sg = _UIBag.transform.Find("BagUI/ChracterDetail/Character/CharacterSpine").GetComponent<SkeletonGraphic>();
        sg.skeletonDataAsset = skeletonDataAsset;
        sg.startingAnimation = "idle";
        sg.startingLoop = true;
        sg.Initialize(true);
    }

    void CloseBagUI()
    {
        _UIMain.RegistEvent(IUIMain.eEvent.OpenCharacterBag, OpenBag);
        _UIBag.SetActive(false);
    }

    void OnEquipAsw(oEquipAsw body)
    {
        //_CharacterUnit = _CharacterManager.GetMainCharacterUnit();
        GameObject equpiBtn = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/BthEquip").gameObject;
        GameObject unloadBtn = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/BthUnload").gameObject;

        equpiBtn.SetActive(false);
        unloadBtn.SetActive(true);


        var item = _BackpackData.BackpackItem.FirstOrDefault(i => i.Guid == body.EquipGuid);
        if (item != null)
        {
            var equipAttr = item.Attributes.FirstOrDefault(attr => attr.Index == (int)eEquipAttr.Equiped_Bool);
            if (equipAttr != null)
            {
                AutoConfig.AutoEnum.eEquipmentType oldtype = (AutoConfig.AutoEnum.eEquipmentType)item.Attributes[eEquipAttr.EquipType_Int32].Value.Int;
                UInt64 currentEquippedItemGuid = 0;
                switch (oldtype)
                {
                    case AutoConfig.AutoEnum.eEquipmentType.Belt:
                        currentEquippedItemGuid = _CharacterUnit.GetUInt64(eCharacterAttr.SlotBelt_UInt64);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Jewelry:
                        currentEquippedItemGuid = _CharacterUnit.GetUInt64(eCharacterAttr.SlotJewelry_UInt64);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Weapon:
                        currentEquippedItemGuid = _CharacterUnit.GetUInt64(eCharacterAttr.SlotWeapon_UInt64);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Cloths:
                        currentEquippedItemGuid = _CharacterUnit.GetUInt64(eCharacterAttr.SlotCloths_UInt64);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Pants:
                        currentEquippedItemGuid = _CharacterUnit.GetUInt64(eCharacterAttr.SlotPants_UInt64);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Shose:
                        currentEquippedItemGuid = _CharacterUnit.GetUInt64(eCharacterAttr.SlotShoes_UInt64);
                        break;
                }

                if (currentEquippedItemGuid != 0)
                {
                    var currentItem = _BackpackData.BackpackItem.FirstOrDefault(i => i.Guid == currentEquippedItemGuid);
                    if (currentItem != null)
                    {
                        var currentEquipAttr = currentItem.Attributes.FirstOrDefault(attr => attr.Index == (int)eEquipAttr.Equiped_Bool);
                        if (currentEquipAttr != null)
                        {
                            currentEquipAttr.Value.Bool = false;
                        }
                    }
                }

                equipAttr.Value.Bool = true;
                AutoConfig.AutoEnum.eEquipmentType type = (AutoConfig.AutoEnum.eEquipmentType)item.Attributes[eEquipAttr.EquipType_Int32].Value.Int;
                switch (type)
                {
                    case AutoConfig.AutoEnum.eEquipmentType.Belt:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotBelt_UInt64, item.Guid);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Jewelry:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotJewelry_UInt64, item.Guid);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Weapon:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotWeapon_UInt64, item.Guid);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Cloths:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotCloths_UInt64, item.Guid);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Pants:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotPants_UInt64, item.Guid);
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Shose:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotShoes_UInt64, item.Guid);
                        break;
                }
            }
            if (_UIBag != null && _UIBag.activeSelf)
            {
                CheckPage(AutoConfig.AutoEnum.eResources.Equipment);
            }
            _WeaponManager.CharacterChangeWeapon(item.ConfigId, _CharacterManager.GetMainCharacterUnit());
        }

    }

    void OnUnEquipAsw(oEquipAsw body)
    {
        //_CharacterUnit = _CharacterManager.GetMainCharacterUnit();
        GameObject equpiBtn = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/BthEquip").gameObject;
        GameObject unloadBtn = _UIBag.transform.Find("EquipmentOperate/EmptyOperater/Operater/BthUnload").gameObject;

        equpiBtn.SetActive(true);
        unloadBtn.SetActive(false);
        var item = _BackpackData.BackpackItem.FirstOrDefault(i => i.Guid == body.EquipGuid);
        if (item != null)
        {
            var equipAttr = item.Attributes.FirstOrDefault(attr => attr.Index == (int)eEquipAttr.Equiped_Bool);
            if (equipAttr != null)
            {
                equipAttr.Value.Bool = false;
                AutoConfig.AutoEnum.eEquipmentType type = (AutoConfig.AutoEnum.eEquipmentType)item.Attributes[eEquipAttr.EquipType_Int32].Value.Int;
                switch (type)
                {
                    case AutoConfig.AutoEnum.eEquipmentType.Belt:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotBelt_UInt64, 0);
                        Transform beltparent = _UIBag.transform.Find("BagUI/ChracterDetail/BeltBottom/BeltBG");
                        foreach (Transform child in beltparent)
                        {
                            GameObject.Destroy(child.gameObject);
                        }

                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Jewelry:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotJewelry_UInt64, 0);
                        Transform jewelryparent = _UIBag.transform.Find("BagUI/ChracterDetail/JewelryBottom/JewelryBG");
                        foreach (Transform child in jewelryparent)
                        {
                            GameObject.Destroy(child.gameObject);
                        }
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Weapon:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotWeapon_UInt64, 0);
                        Transform weaponparent = _UIBag.transform.Find("BagUI/ChracterDetail/WeaponBottom/WeaponBG");
                        foreach (Transform child in weaponparent)
                        {
                            GameObject.Destroy(child.gameObject);
                        }
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Cloths:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotCloths_UInt64, 0);
                        Transform clothsparent = _UIBag.transform.Find("BagUI/ChracterDetail/ClothsBottom/ClothsBG");
                        foreach (Transform child in clothsparent)
                        {
                            GameObject.Destroy(child.gameObject);
                        }
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Pants:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotPants_UInt64, 0);
                        Transform pantsparent = _UIBag.transform.Find("BagUI/ChracterDetail/PantsBottom/PantsBG");
                        foreach (Transform child in pantsparent)
                        {
                            GameObject.Destroy(child.gameObject);
                        }
                        break;
                    case AutoConfig.AutoEnum.eEquipmentType.Shose:
                        _CharacterUnit.SetUInt64(eCharacterAttr.SlotShoes_UInt64, 0);
                        Transform shoseparent = _UIBag.transform.Find("BagUI/ChracterDetail/ShooseBottom/ShooseBG");
                        foreach (Transform child in shoseparent)
                        {
                            GameObject.Destroy(child.gameObject);
                        }
                        break;
                }
                if (_UIBag != null && _UIBag.activeSelf)
                {
                    CheckPage(AutoConfig.AutoEnum.eResources.Equipment);
                    _WeaponManager.CharacterChangeWeapon(-1, _CharacterManager.GetMainCharacterUnit());
                }
            }
        }
    }
    public void Release()
    {

    }
}

