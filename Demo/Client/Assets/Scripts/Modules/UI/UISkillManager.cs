using AutoConfig;
using GameUnit;
using Spine;
using System;
using System.Collections.Generic;
using TMPro;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;
using static AutoConfig.AutoConst;
using static AutoConfig.AutoEnum;
using static GameUnitApi;

public interface IUISkillManager : IModule, IDragHandler, IBeginDragHandler, IEndDragHandler
{
    AutoConfig.Skill GetSkillConfig(in string slot);

    public Int64 GetSkillConfigID(string slot);//获取技能SkillConfigID
    public void OnSkillButtonPress(string slot);
    public void OnSkillButtonRelease(string slot);
    public void ClickSkillBtn(string slot);
    public void CancelSkillRelease();//取消技能释放    
    public void ChangeAtkInfo(UInt64 playerGuid, Int64 configID, Int64 timetick);//改变攻击信息

}

class UISkillManager : IUISkillManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IUISkillManager, UISkillManager>();
    }
    enum SkillReleaseMode
    {
        None,//无技能
        TargetSelection,//选择技能目标
        DragPlacement,//技能拖放
        Instant//技能立即释放
    }
    SkillReleaseMode _CurrentSkillReleaseMode = SkillReleaseMode.None;
    public class ActiveSkillInfo
    {
        public string slot;//技能槽
        public Int64 skillConfigID;//技能配置ID
        public UInt64 skillGuid;//技能GUID
        public AutoConfig.Skill skillconfig;//技能配置
    }

    ActiveSkillInfo _ActiveSkillInfo = null;
    SkillVisual.ISkillVisual _SkillPreviewVisual = null;

    Vector2 _SkillStartPosition;
    bool _IsDragSkill = false;
    float _DragSkillDeadZone = 0f;//拖动技能的死区
    bool _IsLongPress = false;
    private EventTrigger _SkillSlotEventTrigger;
    float _SkillDragAngle = 0f;//技能拖动角度

    const string ConstSkillSlot1 = "UISkillSlot1";
    const string ConstSkillSlot2 = "UISkillSlot2";
    const string ConstSkillSlot3 = "UISkillSlot3";
    const string ConstSkillSlot4 = "UISkillSlot4";

    GameObject _UICharacterCenter;
    GameObject _UICharacterSkillParentNode;
    // GameObject _UICharacterIcon;
    // GameObject _UIScrollViewCharacterIconContent;
    Int64 _LastClickSkillConfig;

    ISkillManager _SkillManager;
    IDamageManager _DamageManager;
    ICharacterManager _CharacterManager;
    IClient _Client;
    IGameSceneManager _GameSceneManager;
    IGameObjectSync _GameObjectSync;

    IUIMain _UIMain;
    IUICharacterInfo _UICharacterInfo;
    IOptimizationOfOperationalFeel _OptimizationOfOperationalFeel;


    GameObject _SelectedSkillIcon = null;
    bool _IsSkillReleasing = false;

    Int64 _SelectedSkillConfigID = AutoConfig.Skill.INVALID_CONFIG_ID;
    UInt64 _SelectCharacterID = INVALID_UNIT_GUID;

    UInt64 _SelectSkillGuid = INVALID_UNIT_GUID;

    private float _skillDragMaxRange = 5f;
    // 当前技能释放的原点（比如主角位置）
    private Vector3 _skillOriginPos;

    class SkillInfo
    {
        public readonly Int64 _ConfigID;
        public string _Slot;
        public UInt64 _LastCastTick = 0;
        public UInt64 _SkillGuid;

        public SkillInfo(Int64 configID, string slot, UInt64 skillGuid)
        {
            _ConfigID = configID;
            _Slot = slot;
            _SkillGuid = skillGuid;
        }
    }

    GameObject _UISkillName;
    GameObject _UISkillDesc;
    GameObject _UISkillUpgradeDesc;
    GameObject _UISkillUpgradeDesc1;
    GameObject _UISkillUpgradeDesc2;
    GameObject _UIBtnUpgradeSkill;

    Vector2 _FinalCastPos; // 最终施法世界坐标

    private Dictionary<UInt64, Int64> _CharacterLastAttackConfigID = new Dictionary<UInt64, Int64>();//角色最后一次攻击的技能配置ID
    private Dictionary<UInt64, Int64> _CharacterLastAttackTime = new Dictionary<UInt64, Int64>();//角色最后一次攻击的时间


    class CharacterSkills
    {
        public Dictionary<Int64, SkillInfo> _ConfigSkillMap = new Dictionary<long, SkillInfo>();//技能ID对应的技能信息字典
        public Dictionary<string, Int64> _SlotSkillMap = new Dictionary<string, Int64>();//技能被装备在哪个槽的字典
    }
    Dictionary<UInt64, CharacterSkills> _MainCharacterSkills = new Dictionary<UInt64, CharacterSkills>();//存每个角色的技能信息

    class SkillSlot
    {
        public GameObject _MainUISlot;
        public GameObject _SkillUISlot;
        public GameObject _SlotUnload;
    }
    Dictionary<string, SkillSlot> _SlotSkillInfo = new Dictionary<string, SkillSlot>();//技能槽的UI字典
    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _Client.RegistProtobuf<oSkillInfoPush>(eServerID.SkillInfoPush, OnSkillInfoPush);
        _SkillManager = Launcher.GetModule<ISkillManager>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _DamageManager = Launcher.GetModule<IDamageManager>();
        _UICharacterInfo = Launcher.GetModule<IUICharacterInfo>();
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
        _OptimizationOfOperationalFeel = Launcher.GetModule<IOptimizationOfOperationalFeel>();
        _GameObjectSync = Launcher.GetModule<IGameObjectSync>();
        _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.GameMapLoaded, ChangeCancelSkillRelease);


        _UICharacterInfo.RegistEvent(IUICharacterInfo.eEvent.RegistSkillUI, (GameObject parent, UInt64 characterId) =>
        {
            _UISkillName = parent.transform.Find("UISkillInfo/UISkillName").gameObject;
            _UISkillDesc = parent.transform.Find("UISkillInfo/UISkillDesc/TextContentView/Viewport/Content/TextTask").gameObject;
            _UISkillUpgradeDesc = parent.transform.Find("UISkillInfo/UISkillUpgradeDesc").gameObject;
            _UISkillUpgradeDesc1 = parent.transform.Find("UISkillInfo/UISkillUpgradeDesc/Condition1").gameObject;
            _UISkillUpgradeDesc2 = parent.transform.Find("UISkillInfo/UISkillUpgradeDesc/Condition2").gameObject;
            //_UIBtnUpgradeSkill = parent.transform.Find("Canvas/UICharacterCenter/UIBtnUpgradeSkill");

            _SelectedSkillConfigID = AutoConfig.Skill.INVALID_CONFIG_ID;
            _SelectCharacterID = INVALID_UNIT_GUID;

            _UICharacterSkillParentNode = parent;

            SkillSlot slot1 = _SlotSkillInfo["UISkillSlot1"];
            slot1._SkillUISlot = parent.transform.Find("UISkillSlotConfig/UISkillSlot1").gameObject;
            slot1._SlotUnload = parent.transform.Find("UISkillSlotConfig/UISkillSlot1/Unload").gameObject;
            slot1._SkillUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);
            slot1._SkillUISlot.transform.Find("Unload").gameObject.SetActive(false);


            slot1._SkillUISlot.GetComponent<Button>().onClick.RemoveAllListeners();
            slot1._SkillUISlot.GetComponent<Button>().onClick.AddListener(() =>
            {
                if (_SelectedSkillConfigID != AutoConfig.Skill.INVALID_CONFIG_ID)
                {
                    EquipSkillSlot(ConstSkillSlot1, _SelectCharacterID, _SelectedSkillConfigID, _SelectSkillGuid);
                }
            });
            slot1._MainUISlot.GetComponent<Button>().onClick.RemoveAllListeners();
            slot1._MainUISlot.GetComponent<Button>().onClick.AddListener(() =>
            {
                //Debug.LogWarning("点击了技能槽1");
                //ClickSkillBtn(ConstSkillSlot1);
            });
            slot1._SlotUnload.GetComponent<Button>().onClick.RemoveAllListeners();
            slot1._SlotUnload.GetComponent<Button>().onClick.AddListener(() =>
            {
                if (_MainCharacterSkills[_SelectCharacterID]._SlotSkillMap.ContainsKey(ConstSkillSlot1)
                    && _MainCharacterSkills[_SelectCharacterID]._SlotSkillMap[ConstSkillSlot1] != AutoConfig.Skill.INVALID_CONFIG_ID)
                {
                    Int64 skillConfigId = _MainCharacterSkills[_SelectCharacterID]._SlotSkillMap[ConstSkillSlot1];
                    UnEquipSkillSlot(ConstSkillSlot1, _SelectCharacterID, skillConfigId);
                }
            });

            SkillSlot slot2 = _SlotSkillInfo["UISkillSlot2"];
            slot2._SkillUISlot = parent.transform.Find("UISkillSlotConfig/UISkillSlot2").gameObject;
            slot2._SlotUnload = parent.transform.Find("UISkillSlotConfig/UISkillSlot2/Unload").gameObject;
            slot2._SkillUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);
            slot2._SkillUISlot.transform.Find("Unload").gameObject.SetActive(false);

            slot2._SkillUISlot.GetComponent<Button>().onClick.RemoveAllListeners();
            slot2._SkillUISlot.GetComponent<Button>().onClick.AddListener(() =>
            {
                if (_SelectedSkillConfigID != AutoConfig.Skill.INVALID_CONFIG_ID)
                {
                    EquipSkillSlot(ConstSkillSlot2, _SelectCharacterID, _SelectedSkillConfigID, _SelectSkillGuid);
                }
            });
            slot2._MainUISlot.GetComponent<Button>().onClick.RemoveAllListeners();
            slot2._MainUISlot.GetComponent<Button>().onClick.AddListener(() =>
            {
                // Debug.LogWarning("点击了技能槽2");
                //ClickSkillBtn(ConstSkillSlot2);
            });
            slot2._SlotUnload.GetComponent<Button>().onClick.AddListener(() =>
            {
                if (_MainCharacterSkills[_SelectCharacterID]._SlotSkillMap.ContainsKey(ConstSkillSlot2)
                    && _MainCharacterSkills[_SelectCharacterID]._SlotSkillMap[ConstSkillSlot2] != AutoConfig.Skill.INVALID_CONFIG_ID)
                {
                    Int64 skillConfigId = _MainCharacterSkills[_SelectCharacterID]._SlotSkillMap[ConstSkillSlot2];
                    UnEquipSkillSlot(ConstSkillSlot2, _SelectCharacterID, skillConfigId);
                }
            });

            SkillSlot slot3 = _SlotSkillInfo["UISkillSlot3"];
            slot3._SkillUISlot = parent.transform.Find("UISkillSlotConfig/UISkillSlot3").gameObject;
            slot3._SlotUnload = parent.transform.Find("UISkillSlotConfig/UISkillSlot3/Unload").gameObject;
            slot3._SkillUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);
            slot3._SkillUISlot.transform.Find("Unload").gameObject.SetActive(false);

            slot3._SkillUISlot.GetComponent<Button>().onClick.RemoveAllListeners();
            slot3._SkillUISlot.GetComponent<Button>().onClick.AddListener(() =>
            {
                if (_SelectedSkillConfigID != AutoConfig.Skill.INVALID_CONFIG_ID)
                {
                    EquipSkillSlot(ConstSkillSlot3, _SelectCharacterID, _SelectedSkillConfigID, _SelectSkillGuid);
                }
            });
            slot3._MainUISlot.GetComponent<Button>().onClick.RemoveAllListeners();
            slot3._MainUISlot.GetComponent<Button>().onClick.AddListener(() =>
            {
                //Debug.LogWarning("点击了技能槽3");
                //ClickSkillBtn(ConstSkillSlot3);
            });
            slot3._SlotUnload.GetComponent<Button>().onClick.AddListener(() =>
            {
                if (_MainCharacterSkills[_SelectCharacterID]._SlotSkillMap.ContainsKey(ConstSkillSlot3)
                    && _MainCharacterSkills[_SelectCharacterID]._SlotSkillMap[ConstSkillSlot3] != AutoConfig.Skill.INVALID_CONFIG_ID)
                {
                    Int64 skillConfigId = _MainCharacterSkills[_SelectCharacterID]._SlotSkillMap[ConstSkillSlot3];
                    UnEquipSkillSlot(ConstSkillSlot3, _SelectCharacterID, skillConfigId);
                }
            });

            SkillSlot slot4 = _SlotSkillInfo["UISkillSlot4"];
            slot4._SkillUISlot = parent.transform.Find("UISkillSlotConfig/UISkillSlot4").gameObject;
            slot4._SlotUnload = parent.transform.Find("UISkillSlotConfig/UISkillSlot4/Unload").gameObject;
            slot4._SkillUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);
            slot4._SkillUISlot.transform.Find("Unload").gameObject.SetActive(false);

            slot4._SkillUISlot.GetComponent<Button>().onClick.RemoveAllListeners();
            slot4._SkillUISlot.GetComponent<Button>().onClick.AddListener(() =>
            {
                if (_SelectedSkillConfigID != AutoConfig.Skill.INVALID_CONFIG_ID)
                {
                    EquipSkillSlot(ConstSkillSlot4, _SelectCharacterID, _SelectedSkillConfigID, _SelectSkillGuid);
                }
            });
            slot4._MainUISlot.GetComponent<Button>().onClick.RemoveAllListeners();
            slot4._MainUISlot.GetComponent<Button>().onClick.AddListener(() =>
            {
                //Debug.LogWarning("点击了技能槽4");
                //ClickSkillBtn(ConstSkillSlot4);
            });
            slot4._SlotUnload.GetComponent<Button>().onClick.AddListener(() =>
            {
                if (_MainCharacterSkills[_SelectCharacterID]._SlotSkillMap.ContainsKey(ConstSkillSlot4)
                    && _MainCharacterSkills[_SelectCharacterID]._SlotSkillMap[ConstSkillSlot4] != AutoConfig.Skill.INVALID_CONFIG_ID)
                {
                    Int64 skillConfigId = _MainCharacterSkills[_SelectCharacterID]._SlotSkillMap[ConstSkillSlot4];
                    UnEquipSkillSlot(ConstSkillSlot4, _SelectCharacterID, skillConfigId);
                }
            });

            _SelectCharacterID = characterId;
            RefreshUISkillManager(_SelectCharacterID);

            if (_SelectCharacterID == _CharacterManager.GetMainCharacterUnit().Guid)
            {
                slot1._MainUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);
                slot2._MainUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);
                slot3._MainUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);
                slot4._MainUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);
            }

            CharacterSkills skillInfos = GetMainCharacterSkills(_SelectCharacterID);
            foreach (var skillMap in skillInfos._ConfigSkillMap)
            {
                if (skillMap.Value._Slot != "")
                {
                    EquipSkillSlot(skillMap.Value._Slot, _SelectCharacterID, skillMap.Value._ConfigID, skillMap.Value._SkillGuid);
                }
            }

        });

        _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, (IUnit unit) =>
        {
            _SlotSkillInfo.Clear();

            SkillSlot slot1 = new SkillSlot();
            slot1._MainUISlot = GameObjectCommon.GetGameObjectByPath("Canvas/UIGameController/UIJoyButton/UISkillSlot1");
            _SlotSkillInfo.Add("UISkillSlot1", slot1);

            SkillSlot slot2 = new SkillSlot();
            slot2._MainUISlot = GameObjectCommon.GetGameObjectByPath("Canvas/UIGameController/UIJoyButton/UISkillSlot2");
            _SlotSkillInfo.Add("UISkillSlot2", slot2);

            SkillSlot slot3 = new SkillSlot();
            slot3._MainUISlot = GameObjectCommon.GetGameObjectByPath("Canvas/UIGameController/UIJoyButton/UISkillSlot3");
            _SlotSkillInfo.Add("UISkillSlot3", slot3);

            SkillSlot slot4 = new SkillSlot();
            slot4._MainUISlot = GameObjectCommon.GetGameObjectByPath("Canvas/UIGameController/UIJoyButton/UISkillSlot4");
            _SlotSkillInfo.Add("UISkillSlot4", slot4);

            GameObject attackBtn = GameObjectCommon.GetGameObjectByPath("Canvas/UIGameController/UIJoyButton/UIAttack");
            attackBtn.GetComponent<Button>().onClick.RemoveAllListeners();
            attackBtn.GetComponent<Button>().onClick.AddListener(() =>
            {
                //ClickNormalAttackBtn();
            });
        });

        _UIMain = Launcher.GetModule<IUIMain>();

        _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, (IUnit unit) =>
        {
            if (unit != null)
            {
                unit.RegistFloatAttributeChange(eCharacterAttr.Mp_Float, eUnitAttributeEvent.Changed,
                (in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue) =>
                {    
                    UpdateAllSkillSlotsMpDisplay(unit);
                    return true;
                }
                );
            }
        });

        return true;
    }

    private void UpdateAllSkillSlotsMpDisplay(IUnit unit)
    {
        foreach (var slotInfo in _SlotSkillInfo)
        {
            UpdateSkillSlotMpDisplay(slotInfo.Key, unit);
        }
    }

    private void UpdateSkillSlotMpDisplay(string slot, IUnit unit)
    {
        if (!_MainCharacterSkills.ContainsKey(unit.Guid) || //检测有没有技能
            !_MainCharacterSkills[unit.Guid]._SlotSkillMap.ContainsKey(slot) ||
            _MainCharacterSkills[unit.Guid]._SlotSkillMap[slot] == AutoConfig.Skill.INVALID_CONFIG_ID)
        {
            //Debug.LogError($"槽位{slot}没有技能");
            return;
        }

        Int64 skillconfigID = _MainCharacterSkills[unit.Guid]._SlotSkillMap[slot];
        AutoConfig.Skill skillconfig = AutoConfig.Skill.Get(skillconfigID);

        if (skillconfig._Consume == AutoConfig.AutoEnum.eConsumeType.Mp)
        {
            if (skillconfig._ConsumeCount > 0)
            {
                float currentMp = unit.GetFloat(eCharacterAttr.Mp_Float);
                bool hasEnoughMp = currentMp >= skillconfig._ConsumeCount;

                UpdateSkillSlotNoMpDisplay(slot, hasEnoughMp);
            }
        }
    }
    private void UpdateSkillSlotNoMpDisplay(string slot, bool hasEnoughMp)
    {
        if (_SlotSkillInfo.ContainsKey(slot))
        {
            GameObject skillslot = _SlotSkillInfo[slot]._MainUISlot;
            if (skillslot != null)
            {
                Transform NoMp = skillslot.transform.Find("NoMp");
                if (NoMp != null)
                {
                    NoMp.gameObject.SetActive(!hasEnoughMp);
                }
            }
        }
    }

    private void ChangeCancelSkillRelease(UnityEngine.SceneManagement.Scene scene)
    {
        CancelSkillRelease();
    }

    public void Release()
    {

    }

    
    public void Update()
    {
        if (_CurrentSkillReleaseMode == SkillReleaseMode.TargetSelection)
        {
            bool hasInput = Input.GetMouseButtonDown(0) || (Input.touchCount > 0 && Input.GetTouch(0).phase == TouchPhase.Began);

            if (hasInput)
            {
                eTargetType[] targetType = AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._TargetType;
                List<eUnitType> clickTypeList = new List<eUnitType>();
                for (int i = 0; i < targetType.Length; i++)
                {
                    eUnitType type = ChangeClickType(targetType[i]);
                    if (type != eUnitType.Creature)
                    {
                        clickTypeList.Add(type);
                    }
                }
                eUnitType[] clickType = clickTypeList.ToArray();

                UInt64 targetGuid = _SkillManager.CheckClickOnPlayerAdvanced(clickType);
                if (targetGuid != 0)
                {
                    ExecuteSkillWithTarget(targetGuid);
                }
            }
        }
    }

    eUnitType ChangeClickType(AutoEnum.eTargetType targetType)
    {
        switch (targetType)
        {
            case eTargetType.EnemyCharacter:
                return eUnitType.Creature;//预留
            case eTargetType.Monster:
                return eUnitType.Monster;
            case eTargetType.Boss:
                return eUnitType.Boss;
            case eTargetType.PlayerSelf:
                return eUnitType.Character;
            default:
                return eUnitType.Character;//预留
        }
    }

    public AutoConfig.Skill GetSkillConfig(in string slot)
    {
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();

        if (null == mainCharacterUnit
            || !_MainCharacterSkills.ContainsKey(mainCharacterUnit.Guid)
            || !_MainCharacterSkills[_CharacterManager.GetMainCharacterUnit().Guid]._SlotSkillMap.ContainsKey(slot)
            || _MainCharacterSkills[_CharacterManager.GetMainCharacterUnit().Guid]._SlotSkillMap[slot] == AutoConfig.Skill.INVALID_CONFIG_ID
            )
        {
            return null;
        }
        return AutoConfig.Skill.Get(_MainCharacterSkills[_CharacterManager.GetMainCharacterUnit().Guid]._SlotSkillMap[slot]);
    }

    void UnEquipSkillSlot(string slot, UInt64 characterId, Int64 skillConfigId)
    {
        _MainCharacterSkills[characterId]._SlotSkillMap[slot] = AutoConfig.Skill.INVALID_CONFIG_ID;
        _MainCharacterSkills[characterId]._ConfigSkillMap[skillConfigId]._Slot = null;
        if (_SlotSkillInfo[slot]._SkillUISlot != null)
        {
            _SlotSkillInfo[slot]._SkillUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);
        }
        if (_CharacterManager.GetMainCharacterUnit().Guid == characterId)
        {
            _SlotSkillInfo[slot]._MainUISlot.transform.Find("UISkillIcon").gameObject.SetActive(false);

            IUnit unit = _CharacterManager.GetMainCharacterUnit();
            if (unit != null && unit.Guid == characterId)
            {
                UpdateSkillSlotNoMpDisplay(slot, true);//卸载后关闭技能MP显示
            }
        }
        if (_SlotSkillInfo[slot]._SkillUISlot != null)
        {
            Transform unloadTransform = _SlotSkillInfo[slot]._SkillUISlot.transform.Find("Unload");
            if (unloadTransform != null)
            {
                unloadTransform.gameObject.SetActive(false);
            }
        }

        oConfigureSkillSlot req = new oConfigureSkillSlot();
        req.CharacterId = characterId;
        req.SkillConfigId = skillConfigId;
        req.Slot = "";
        _Client.SendProtobuf(eClientID.ConfigureSkillSlot, req);
        RefreshSkillUIState(characterId, skillConfigId);
    }

    void EquipSkillSlot(string slot, UInt64 characterId, Int64 skillConfigId, UInt64 skillGuid)
    {
        AutoConfig.Skill skillConfig = AutoConfig.Skill.Get(skillConfigId);
        if (!skillConfig._CanEquip)
        {
            return;
        }

        if (_MainCharacterSkills[characterId]._ConfigSkillMap[skillConfigId]._Slot != null
            && _MainCharacterSkills[characterId]._ConfigSkillMap[skillConfigId]._Slot != "")
        {
            UnEquipSkillSlot(_MainCharacterSkills[characterId]._ConfigSkillMap[skillConfigId]._Slot, characterId, skillConfigId);
        }

        if (_MainCharacterSkills[characterId]._SlotSkillMap.ContainsKey(slot) && _MainCharacterSkills[characterId]._SlotSkillMap[slot] != AutoConfig.Skill.INVALID_CONFIG_ID)
        {
            UnEquipSkillSlot(slot, characterId, _MainCharacterSkills[characterId]._SlotSkillMap[slot]);
        }

        _MainCharacterSkills[characterId]._SlotSkillMap[slot] = skillConfigId;
        _MainCharacterSkills[characterId]._ConfigSkillMap[skillConfigId]._Slot = slot;
        _MainCharacterSkills[characterId]._ConfigSkillMap[skillConfigId]._SkillGuid = skillGuid;


        if (_SlotSkillInfo[slot]._SkillUISlot != null)
        {
            _SlotSkillInfo[slot]._SkillUISlot.transform.Find("UISkillIcon").gameObject.SetActive(true);
            _SlotSkillInfo[slot]._SkillUISlot.transform.Find("UISkillIcon").GetComponent<Image>().sprite = Resources.Load<Sprite>(AutoConfig.Skill.Get(skillConfigId)._SkillIcon);
            Transform unloadTransform = _SlotSkillInfo[slot]._SkillUISlot.transform.Find("Unload");
            if (unloadTransform != null)
            {
                unloadTransform.gameObject.SetActive(true);
            }
        }


        if (_CharacterManager.GetMainCharacterUnit().Guid == characterId)
        {
            _SlotSkillInfo[slot]._MainUISlot.transform.Find("UISkillIcon").gameObject.SetActive(true);
            _SlotSkillInfo[slot]._MainUISlot.transform.Find("UISkillIcon").GetComponent<Image>().sprite = Resources.Load<Sprite>(AutoConfig.Skill.Get(skillConfigId)._SkillIcon);


            IUnit unit = _CharacterManager.GetMainCharacterUnit();
            if (unit != null && unit.Guid == characterId)
            {
                UpdateSkillSlotMpDisplay(slot, unit);
            }
        }

        oConfigureSkillSlot req = new oConfigureSkillSlot();
        req.CharacterId = characterId;
        req.SkillConfigId = skillConfigId;
        req.SkillGuid = skillGuid;
        req.Slot = slot;
        _Client.SendProtobuf(eClientID.ConfigureSkillSlot, req);

        RefreshSkillUIState(characterId, skillConfigId);
    }

    void RefreshSkillUIState(UInt64 characterId, Int64 skillConfigId)
    {
        CharacterSkills skillInfos = GetMainCharacterSkills(characterId);
        if (skillInfos == null) return;

        IUnit characterUnit = GameUnitApi.QueryUnit(characterId);
        if (characterUnit == null) return;

        AutoConfig.Character characterConfig = AutoConfig.Character.Get(characterUnit.ConfigID);

        for (int i = 0; i < characterConfig._Skill.Length; i++)
        {
            if (characterConfig._Skill[i] == skillConfigId)
            {
                AutoConfig.Skill skillConfig = AutoConfig.Skill.Get(skillConfigId);
                GameObject skillItem = GameObjectCommon.GetGameObjectByPath("Canvas/UICharacterCenter/UISkillManager/" + skillConfig._UINode);
                skillItem.transform.Find("NameBG/Name").GetComponent<TextMeshProUGUI>().text = skillConfig._Name;
                if (skillItem != null)
                {
                    bool isEquiped = false;
                    foreach (var slot in skillInfos._SlotSkillMap)
                    {
                        if (slot.Value == skillConfigId)
                        {
                            isEquiped = true;
                            break;
                        }
                    }
                    if (isEquiped)
                    {
                        GameObject stateitem = skillItem.transform.Find("State").gameObject;
                        stateitem.SetActive(true);
                        GameObject equipedIcon = stateitem.transform.Find("Equipped").gameObject;
                        equipedIcon.SetActive(true);
                        GameObject changeIcon = stateitem.transform.Find("Change").gameObject;
                        changeIcon.SetActive(false);
                        // if (skillItem.transform.Find("UISkillSelected").gameObject.activeSelf)
                        // {
                        //     equipedIcon.SetActive(false);
                        //     changeIcon.SetActive(true);
                        // }
                        //Debug.LogError($"技能{skillConfig._Name}已装备");
                    }
                    else
                    {
                        GameObject stateitem = skillItem.transform.Find("State").gameObject;
                        stateitem.SetActive(false);
                        //Debug.LogError($"技能{skillConfig._Name}未装备");
                    }

                }
                break;
            }
        }

        if (_CharacterManager.GetMainCharacterUnit() != null && _CharacterManager.GetMainCharacterUnit().Guid == characterId)
        {
            UpdateAllSkillSlotsMpDisplay(characterUnit);
        }

    }

    void RefreshUISkillManager(in UInt64 characterId)
    {
        UInt64 currentcharacterid = characterId;
        IUnit characterUnit = GameUnitApi.QueryUnit(characterId);
        if (null == characterUnit)
        {
            Debug.LogWarning($"找不到 {characterId}");
            return;
        }
        CharacterSkills skillInfos = GetMainCharacterSkills(characterId);
        if (null == skillInfos)
        {
            return;
        }

        if (null != _SelectedSkillIcon)
        {
            _SelectedSkillIcon.transform.Find("UISkillSelected").gameObject.SetActive(false);
            _SelectedSkillIcon = null;
        }

        AutoConfig.Character characterConfig = AutoConfig.Character.Get(characterUnit.ConfigID);

        //刷新
        for (int i = 0; i < 10; i++)
        {
            GameObject skillItem = _UICharacterSkillParentNode.transform.Find($"UISkillManager/UISkill{(i + 1)}").gameObject;
            skillItem.transform.Find("LockMask").gameObject.SetActive(true);
            skillItem.transform.Find("State").gameObject.SetActive(false);
            skillItem.transform.Find("UISkillSelected").gameObject.SetActive(false);
        }

        for (int i = 0; i < characterConfig._Skill.Length; i++)
        {
            AutoConfig.Skill skillConfig = AutoConfig.Skill.Get(characterConfig._Skill[i]);

            GameObject skillItem = _UICharacterSkillParentNode.transform.Find("UISkillManager/" + skillConfig._UINode).gameObject;
            if (skillItem == null)
            {
                Debug.LogWarning($"找不到技能UI元素: UISkillManager/{skillConfig._UINode}");
                continue;
            }
            GameObject skillIcon = _UICharacterSkillParentNode.transform.Find("UISkillManager/" + skillConfig._UINode + "/Icon").gameObject;
            if (skillIcon == null)
            {
                Debug.LogWarning($"找不到技能图标: {skillConfig._UINode}/Icon，此时技能{skillConfig._Name}，UI节点{skillConfig._UINode}，人物{characterUnit.ConfigID}");
                continue;
            }
            skillItem.transform.Find("NameBG/Name").GetComponent<TextMeshProUGUI>().text = skillConfig._Name;
            
            Sprite skillpng = Resources.Load<Sprite>(skillConfig._SkillIcon);
            Image imageComponent = skillIcon.GetComponent<Image>();
            skillIcon.GetComponent<Image>().sprite = skillpng;
            skillItem.GetComponent<Button>().onClick.RemoveAllListeners();



            bool isEquiped = false;
            string equipedSlot = null;
            foreach (var slot in skillInfos._SlotSkillMap)
            {
                if (slot.Value == characterConfig._Skill[i])
                {
                    isEquiped = true;
                    equipedSlot = slot.Key;
                    break;
                }
            }
            if (isEquiped)
            {
                GameObject stateitem = skillItem.transform.Find("State").gameObject;
                stateitem.SetActive(true);
                GameObject equipedIcon = stateitem.transform.Find("Equipped").gameObject;
                equipedIcon.SetActive(true);
            }

            if (skillInfos._ConfigSkillMap.ContainsKey(characterConfig._Skill[i]))
            {
                skillItem.transform.Find("LockMask").gameObject.SetActive(false);

                var currentSkillConfig = skillConfig;
                var currentSkillInfos = skillInfos;
                skillItem.GetComponent<Button>().onClick.AddListener(() =>
                {
                    _UISkillName.GetComponent<TextMeshProUGUI>().text = skillConfig._Name;
                    _UISkillDesc.GetComponent<TextMeshProUGUI>().text = skillConfig._Desc;
                    _UISkillUpgradeDesc.GetComponent<TextMeshProUGUI>().text = "测试技能升级描述";


                    if (null != _SelectedSkillIcon)
                    {
                        _SelectedSkillIcon.transform.Find("UISkillSelected").gameObject.SetActive(false);
                    }
                    _SelectedSkillConfigID = skillConfig._ID;
                    _SelectedSkillIcon = skillItem;
                    //_SelectedSkillIcon.transform.Find("UISkillSelected").gameObject.SetActive(true);
                    GameObject selected = _SelectedSkillIcon.transform.Find("UISkillSelected").gameObject;
                    selected.SetActive(true);

                    //RefreshSkillUIState(currentcharacterid, skillConfig._ID);
                });
            }
        }

        if (_CharacterManager.GetMainCharacterUnit() != null && _CharacterManager.GetMainCharacterUnit().Guid == characterId)
        {
            UpdateAllSkillSlotsMpDisplay(characterUnit);
        }
    }
    void OnSkillInfoPush(oSkillInfoPush push)
    {
        //Debug.LogError("收到技能信息推送");
        CharacterSkills skillInfos = null;
        if (!_MainCharacterSkills.TryGetValue(push.CharacterId, out skillInfos))
        {
            skillInfos = new CharacterSkills();
            _MainCharacterSkills.Add(push.CharacterId, skillInfos);
        }

        for (int i = 0; i < push.Skills.Count; i++)
        {
            AutoConfig.Skill config = AutoConfig.Skill.Get(push.Skills[i].SkillConfigId);
            UInt64 skillGuid = push.Skills[i].SkillGuid;
            skillInfos._ConfigSkillMap.Add(push.Skills[i].SkillConfigId, new SkillInfo(push.Skills[i].SkillConfigId, push.Skills[i].SkillSlot, skillGuid));
            if (push.Skills[i].SkillSlot != "" && push.Skills[i].SkillSlot != null)
            {
                skillInfos._SlotSkillMap.Add(push.Skills[i].SkillSlot, push.Skills[i].SkillConfigId);
                EquipSkillSlot(push.Skills[i].SkillSlot, push.CharacterId, push.Skills[i].SkillConfigId, skillGuid);
            }
        }
        for(int i= 0 ; i < push.Skills.Count; i++)
        {
            if(push.Skills[i].SkillSlot == null || push.Skills[i].SkillSlot == "")
            {
                string[] availableSlots = {"UISkillSlot1", "UISkillSlot2", "UISkillSlot3", "UISkillSlot4"};
                foreach(string slot in availableSlots)
                {
                    if(!skillInfos._SlotSkillMap.ContainsKey(slot)|| skillInfos._SlotSkillMap[slot] == AutoConfig.Skill.INVALID_CONFIG_ID)
                    {
                        UInt64 skillGuid =push.Skills[i].SkillGuid;
                        skillInfos._ConfigSkillMap[push.Skills[i].SkillConfigId]._Slot = slot;
                        skillInfos._SlotSkillMap[slot] = push.Skills[i].SkillConfigId;

                        EquipSkillSlot(slot, push.CharacterId, push.Skills[i].SkillConfigId, skillGuid);
                        break;
                    }
                }
            }
        }
        if (_UICharacterSkillParentNode != null)
        {
            RefreshUISkillManager(push.CharacterId);
        }
    }
    CharacterSkills GetMainCharacterSkills(UInt64 characterId)
    {
        CharacterSkills skills = null;
        if (_MainCharacterSkills.TryGetValue(characterId, out skills))
        {
            return skills;
        }
        return null;
    }

    public Int64 GetSkillConfigID(string slot)//获取技能SkillConfigID
    {
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();

        if (null == mainCharacterUnit
    || !_MainCharacterSkills.ContainsKey(mainCharacterUnit.Guid)
    || !_MainCharacterSkills[mainCharacterUnit.Guid]._SlotSkillMap.ContainsKey(slot)
    || _MainCharacterSkills[mainCharacterUnit.Guid]._SlotSkillMap[slot] == AutoConfig.Skill.INVALID_CONFIG_ID)
        {
            return 0;
        }

        return _MainCharacterSkills[mainCharacterUnit.Guid]._SlotSkillMap[slot];
        //Int64 skillConfigId = _MainCharacterSkills[mainCharacterUnit.Guid]._SlotSkillMap[slot];
        //return _MainCharacterSkills[mainCharacterUnit.Guid]._ConfigSkillMap[skillConfigId]._SkillGuid;
    }

    public void ClickSkillBtn(string slot)
    {
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        if (null == mainCharacterUnit)
        {
            Debug.LogWarning($"找不到主角色");
            return;
        }

        if (!_MainCharacterSkills.ContainsKey(mainCharacterUnit.Guid) || !_MainCharacterSkills[mainCharacterUnit.Guid]._SlotSkillMap.ContainsKey(slot) || _MainCharacterSkills[mainCharacterUnit.Guid]._SlotSkillMap[slot] == AutoConfig.Skill.INVALID_CONFIG_ID)
        {
            XApi.Debug($"技能槽 {slot} 没有装备技能或不存在");
            Debug.LogError($"技能槽 {slot} 没有装备技能或不存在");
            return;
        }
        Int64 skillConfigId = _MainCharacterSkills[mainCharacterUnit.Guid]._SlotSkillMap[slot];//得到技能configID
        UInt64 skillGuid = _MainCharacterSkills[mainCharacterUnit.Guid]._ConfigSkillMap[skillConfigId]._SkillGuid;//得到技能GUID
        AutoConfig.Skill skillConfig = AutoConfig.Skill.Get(skillConfigId);//得到技能


        StartSkillRelease(slot, skillConfigId, skillGuid, skillConfig);
    }

    private void StartSkillRelease(string slot, long skillConfigId, ulong skillGuid, AutoConfig.Skill skillConfig)//技能释放开始
    {
        if (_IsSkillReleasing) return;
        _IsSkillReleasing = true;
        if (_CurrentSkillReleaseMode != SkillReleaseMode.None)
        {
            CancelSkillRelease();
        }
        _ActiveSkillInfo = new ActiveSkillInfo
        {
            slot = slot,
            skillConfigID = skillConfigId,
            skillGuid = skillGuid,
            skillconfig = skillConfig
        };

        AutoEnum.eSelectedType currentskilltype = AutoConfig.Skill.Get(skillConfigId)._SelectedType;

        if (_IsSkillReleasing == false)
        {
            Debug.LogWarning("技能释放取消");
            return;
        }
        switch (currentskilltype)
        {
            case AutoEnum.eSelectedType.Aim://指向性（技能点击后必须选择了目标后，才能释放）
                Debug.LogWarning($"指向性技能 {slot} ");
                _CurrentSkillReleaseMode = SkillReleaseMode.TargetSelection;
                StartTargetSelectionMode(skillConfig);
                break;
            case AutoEnum.eSelectedType.Liberty://自由性（技能点击后立刻释放）
                Debug.LogWarning($"自由性技能 {slot} ");
                _CurrentSkillReleaseMode = SkillReleaseMode.Instant;
                break;
            case AutoEnum.eSelectedType.UnAim://无指向性（技能点击后，选择面片，才能释放）
                Debug.LogWarning($"无指向性技能 {slot} ");
                _CurrentSkillReleaseMode = SkillReleaseMode.DragPlacement;
                StartDragPlacementMode(skillConfig);
                break;
            default:
                break;
        }
    }

    private void StartTargetSelectionMode(AutoConfig.Skill skillConfig)//技能释放模式-选择目标
    {
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;
        if (exitBtn != null)
        {
            exitBtn.SetActive(true);
            Button exitBtnBtn = exitBtn.GetComponent<Button>();
            if (exitBtnBtn != null)
            {
                exitBtnBtn.onClick.RemoveAllListeners();
                exitBtnBtn.onClick.AddListener(() =>
                {
                    CancelSkillRelease();
                });
            }
        }

        //展示技能范围
        Int64 skillConfigId = skillConfig._ID;
        GameObject skillRange = Resources.Load<GameObject>("UI/Sprites/Skill/Ring");//加载技能范围
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        GameObject skillRangeParent = mainCharacterUnit.transform.Find("SkillRange").gameObject;//找到主角色的技能范围父物体
        if (skillRangeParent != null)
        {
            for (int i = 0; i < skillRangeParent.transform.childCount; i++)
            {
                GameObject.Destroy(skillRangeParent.transform.GetChild(i).gameObject);
            }
        }
        GameObject skillRangeInstance = GameObject.Instantiate(skillRange);
        skillRangeInstance.transform.SetParent(skillRangeParent.transform);//设置技能范围的父物体为主角色的技能范围
        skillRangeInstance.transform.localPosition = Vector3.zero;
        Vector3 scale = skillRangeInstance.transform.localScale;
        scale.x = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
        scale.y = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
        skillRangeInstance.transform.localScale = scale;

        //点击某个地方选定某个人
        //技能释放//取消
    }
    private void ExecuteSkill(AutoConfig.Skill skillConfig)//执行技能
    {
        //直接点击给后端发CONFIGID
        if (_ActiveSkillInfo != null)
        {
            IUnit maincharacter = _CharacterManager.GetMainCharacterUnit();
            if (maincharacter != null)
            {
                oVector2 targetpos = new oVector2
                {
                    X = maincharacter.Position.x,
                    Y = maincharacter.Position.y,
                };

                _SkillManager.UseSkill(_ActiveSkillInfo.skillGuid, _ActiveSkillInfo.skillConfigID, 0, targetpos, maincharacter.Guid, targetpos);
                Debug.LogWarning($"立即释放技能，GUID是 {_ActiveSkillInfo.skillGuid} ，技能ID是{_ActiveSkillInfo.skillConfigID}");
            }
        }
        //长按显示范围
        //松手释放
        //滑动到取消按钮，取消施法
    }
    private void StartDragPlacementMode(AutoConfig.Skill skillConfig)//技能释放模式-拖拽placement
    {
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        if (_IsSkillReleasing == false)
        {
            CancelSkillRelease();
            return;
        }
        IUnit testMainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        if (testMainCharacterUnit != null)
        {
            _skillOriginPos = testMainCharacterUnit.transform.position;
        }
        _skillDragMaxRange = AutoConfig.Skill.Get(skillConfig._ID)._MaxRangeRadius;

        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;
        if (exitBtn != null)
        {
            exitBtn.SetActive(true);
            Button exitBtnBtn = exitBtn.GetComponent<Button>();
            if (exitBtnBtn != null)
            {
                exitBtnBtn.onClick.RemoveAllListeners();
                exitBtnBtn.onClick.AddListener(() =>
                {
                    CancelSkillRelease();
                });
            }
        }
        Int64 skillConfigId = skillConfig._ID;
        //显示面片
        AutoEnum.eSheetsType sheetsType = AutoConfig.Skill.Get(skillConfigId)._SheetsType;
        float[] sheetsParameter = AutoConfig.Skill.Get(skillConfigId)._RangeParameter;
        switch (sheetsType)
        {
            case AutoEnum.eSheetsType.Circle://圆形，半径
                _SkillPreviewVisual = SkillVisual.Api.CreateCircleVisual(sheetsParameter[0]);
                Debug.LogWarning($"创建圆形技能预览，半径{sheetsParameter[0]}");
                if (_SkillPreviewVisual != null)
                {
                    _SkillPreviewVisual.GetComponent<MeshRenderer>().material = Resources.Load<UnityEngine.Material>("UI/Sprites/Skill/WorldPos");
                }
                break;
            case AutoEnum.eSheetsType.Rectangle://矩形，长宽
                _SkillPreviewVisual = SkillVisual.Api.CreateRectangleVisual(sheetsParameter[0], sheetsParameter[1]);
                Debug.LogWarning($"创建矩形技能预览，长{sheetsParameter[0]}，宽{sheetsParameter[1]}");
                if (_SkillPreviewVisual != null)
                {
                    MeshFilter meshFilter = _SkillPreviewVisual.GetComponent<MeshFilter>();
                    if (meshFilter != null && meshFilter.mesh != null)
                    {
                        Vector3[] vertices = meshFilter.mesh.vertices;
                        // 将所有顶点沿Z轴负方向移动一半高度，使锚点在底部边缘中心
                        // Debug.Log("原始顶点坐标：");
                        // for (int i = 0; i < vertices.Length; i++)
                        // {
                        //     Debug.Log($"顶点{i}：{vertices[i]}");
                        // }
                        // for (int i = 0; i < vertices.Length; i++)
                        // {
                        //     vertices[i].z -= sheetsParameter[0] * 0.5f;
                        // }
                        float bottomEdgeZOffset = Mathf.Abs(vertices[0].z);
                        // 所有顶点Z轴+偏移量，让底部边中心对齐原点
                        for (int i = 0; i < vertices.Length; i++)
                        {
                            vertices[i].z += bottomEdgeZOffset;
                        }
                        // for (int i = 0; i < vertices.Length; i++)
                        // {
                        //     vertices[i].z = -vertices[i].z;
                        // }
                        meshFilter.mesh.vertices = vertices;
                        meshFilter.mesh.RecalculateBounds();
                        _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);


                    }
                    if (_SkillPreviewVisual != null)
                    {
                        _SkillPreviewVisual.GetComponent<MeshRenderer>().material = Resources.Load<UnityEngine.Material>("UI/Sprites/Skill/Arrow");

                    }
                }
                break;
            case AutoEnum.eSheetsType.sector://扇形，半径和角度
                Debug.LogWarning($"创建扇形技能预览，半径{sheetsParameter[0]}，角度{sheetsParameter[1]}");
                _SkillPreviewVisual = SkillVisual.Api.CreateSectorVisual(sheetsParameter[0], sheetsParameter[1]);
                break;
        }

        if (_SkillPreviewVisual == null)
        {
            Debug.LogWarning("技能预览创建失败");
            CancelSkillRelease();
            return;
        }
        //判定是不是固定位置,是固定位置就只显示面片
        if (AutoConfig.Skill.Get(skillConfigId)._CanSeparated == false)
        {
            //IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
            GameObject parent = mainCharacterUnit.transform.Find("PlacementLab").gameObject;
            if (parent != null)
            {
                for (int i = 0; i < parent.transform.childCount; i++)
                {
                    GameObject.Destroy(parent.transform.GetChild(i).gameObject);
                }
            }
            _SkillPreviewVisual.transform.SetParent(parent.transform);//设置技能范围的父物体为主角色的技能范围
            _SkillPreviewVisual.transform.localPosition = Vector3.zero;
            _SkillPreviewVisual.transform.localScale = Vector3.one;
            _SkillPreviewVisual.transform.localRotation = Quaternion.Euler(90, 0, 0);


        }
        else
        {
            //非固定位置则显示技能范围
            GameObject skillRange = Resources.Load<GameObject>("UI/Sprites/Skill/Ring");//加载技能范围
                                                                                        //IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
            GameObject skillRangeParent = mainCharacterUnit.transform.Find("SkillRange").gameObject;//找到主角色的技能范围父物体
            if (skillRangeParent != null)
            {
                for (int i = 0; i < skillRangeParent.transform.childCount; i++)
                {
                    GameObject.Destroy(skillRangeParent.transform.GetChild(i).gameObject);
                }
            }
            GameObject skillRangeInstance = GameObject.Instantiate(skillRange);
            skillRangeInstance.transform.SetParent(skillRangeParent.transform);//设置技能范围的父物体为主角色的技能范围
            skillRangeInstance.transform.localPosition = Vector3.zero;
            Vector3 scale = skillRangeInstance.transform.localScale;
            scale.x = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
            scale.y = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
            skillRangeInstance.transform.localScale = scale;


            GameObject parent = mainCharacterUnit.transform.Find("PlacementLab").gameObject;
            _SkillPreviewVisual.transform.SetParent(parent.transform);//设置技能范围的父物体为主角色的Placement
            _SkillPreviewVisual.transform.localPosition = Vector3.zero;
            _SkillPreviewVisual.transform.localScale = Vector3.one;
            _SkillPreviewVisual.transform.localRotation = Quaternion.Euler(90, 0, 0);
            // _SkillPreviewVisual.transform.localEulerAngles = new Vector3(90, 0, 0);
            _SkillPreviewVisual.transform.Rotate(0, -90, +60, Space.Self);


        }
        MeshRenderer meshRenderer = _SkillPreviewVisual.transform.GetComponent<MeshRenderer>();
        if (meshRenderer != null)
        {
            meshRenderer.sortingOrder = 9;
        }

        GameObject skillSlot = _SlotSkillInfo[_ActiveSkillInfo.slot]._MainUISlot;
        EventTrigger trigger = skillSlot.GetComponent<EventTrigger>();
        if (trigger == null)
        {
            trigger = skillSlot.AddComponent<EventTrigger>();
        }
        if (_ActiveSkillInfo == null || _SlotSkillInfo == null) return;
        if (!_SlotSkillInfo.ContainsKey(_ActiveSkillInfo.slot)) return;

        _SkillSlotEventTrigger = trigger;
        trigger.triggers.Clear();

        AddEventTriggerListener(trigger, EventTriggerType.BeginDrag, OnSkillDragBegin);
        AddEventTriggerListener(trigger, EventTriggerType.Drag, OnSkillDrag);
        AddEventTriggerListener(trigger, EventTriggerType.EndDrag, OnSkillDragEnd);
        //拖拽更新位置
        //松手释放技能

        IUnit lastattackUnit = _DamageManager.GetLastDamageUnit();
        if (lastattackUnit != null)
        {
            float distance = Vector2.Distance(mainCharacterUnit.Position, lastattackUnit.Position);
            if (distance <= AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._MaxSkillDistacne)
            {
                if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == true)
                {
                    if (lastattackUnit.transform != null) // 检查对象是否仍然有效
                    {
                        Vector3 targetPos3D = new Vector3(lastattackUnit.Position.x, lastattackUnit.Position.y, _SkillPreviewVisual.transform.position.z);
                        _SkillPreviewVisual.transform.position = targetPos3D;
                        //Debug.LogError($"面片移动，距离：{distance}，技能设定：{AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._MaxSkillDistacne}");
                    }
                }
                else if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == false)
                {
                    if (lastattackUnit.transform != null) // 检查对象是否仍然有效
                    {
                        _SkillDragAngle = Mathf.Atan2(lastattackUnit.Position.y - mainCharacterUnit.Position.y, lastattackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                        _SkillDragAngle = (_SkillDragAngle + 360f) % 360f;
                        _SkillPreviewVisual.transform.eulerAngles = new Vector3(_SkillDragAngle, -90, -90);
                        _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
                        _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
                    }
                }
            }
            else
            {
                IUnit nowAttackUnit = GetCanAttackUint(_ActiveSkillInfo.skillConfigID);
                if (nowAttackUnit != null)
                {
                    if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == true)
                    {
                        if (nowAttackUnit.transform != null) // 检查对象是否仍然有效
                        {
                            Vector3 targetPos3D = new Vector3(nowAttackUnit.Position.x, nowAttackUnit.Position.y, _SkillPreviewVisual.transform.position.z);
                            _SkillPreviewVisual.transform.position = targetPos3D;
                        }
                    }
                    else if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == false)
                    {
                        if (nowAttackUnit.transform != null) // 检查对象是否仍然有效
                        {
                            _SkillDragAngle = Mathf.Atan2(nowAttackUnit.Position.y - mainCharacterUnit.Position.y, nowAttackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                            _SkillDragAngle = (_SkillDragAngle + 360f) % 360f;
                            _SkillPreviewVisual.transform.eulerAngles = new Vector3(_SkillDragAngle, -90, -90);
                            _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
                            _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
                        }
                    }
                }

            }
        }
        else
        {
            IUnit nowAttackUnit = GetCanAttackUint(_ActiveSkillInfo.skillConfigID);
            if (nowAttackUnit != null)
            {
                if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == true)
                {
                    if (nowAttackUnit.transform != null) // 检查对象是否仍然有效
                    {
                        Vector3 targetPos3D = new Vector3(nowAttackUnit.Position.x, nowAttackUnit.Position.y, _SkillPreviewVisual.transform.position.z);
                        _SkillPreviewVisual.transform.position = targetPos3D;
                    }
                }
                else if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == false)
                {
                    if (nowAttackUnit.transform != null) // 检查对象是否仍然有效
                    {
                        _SkillDragAngle = Mathf.Atan2(nowAttackUnit.Position.y - mainCharacterUnit.Position.y, nowAttackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                        _SkillDragAngle = (_SkillDragAngle + 360f) % 360f;
                        _SkillPreviewVisual.transform.eulerAngles = new Vector3(_SkillDragAngle, -90, -90);
                        _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
                        _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
                    }
                }
            }
        }
    }
    private void AddEventTriggerListener(EventTrigger trigger, EventTriggerType type, UnityAction<BaseEventData> action)
    {
        EventTrigger.Entry entry = new EventTrigger.Entry();
        entry.eventID = type;
        entry.callback.AddListener(action);
        trigger.triggers.Add(entry);
    }
    private void OnSkillDragBegin(BaseEventData data)
    {
        OnBeginDrag((PointerEventData)data);
    }
    private void OnSkillDrag(BaseEventData data)
    {
        OnDrag((PointerEventData)data);
    }
    private void OnSkillDragEnd(BaseEventData data)
    {
        OnEndDrag((PointerEventData)data);
    }

    public void OnBeginDrag(PointerEventData eventData)//技能拖拽开始
    {
        if (_CurrentSkillReleaseMode == SkillReleaseMode.DragPlacement && _ActiveSkillInfo != null)
        {
            _SkillStartPosition = eventData.position;
            _IsDragSkill = true;
            _IsSkillReleasing = true;
            _SkillDragAngle = 0f;
            _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
        }
    }

    public void OnDrag(PointerEventData eventData)//技能拖拽更新位置
    {

        if (!_IsDragSkill || _SkillPreviewVisual == null)
        {
            return;
        }
        _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
        Vector2 dragDistance = eventData.position - _SkillStartPosition;
        if (dragDistance.magnitude < _DragSkillDeadZone)
        {
            return;
        }

        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        float angle = Mathf.Atan2(dragDistance.y, dragDistance.x) * Mathf.Rad2Deg;
        angle = (angle + 360f) % 360f;
        _SkillDragAngle = angle;

        AutoEnum.eSheetsType sheetsType = AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._SheetsType;
        bool canSeparated = AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated;//是否固定位置
        if (!canSeparated)
        {
            //_SkillPreviewVisual.transform.eulerAngles = new Vector3(90, 0, angle);
            _SkillPreviewVisual.transform.eulerAngles = new Vector3(angle, -90, -90);
            _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
            return;
        }

        float maxRange = AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._MaxSkillDistacne;
        float changeDistance = Mathf.Min(dragDistance.magnitude / AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._SheetsMove, maxRange);

        float deltaX = Mathf.Cos(angle * Mathf.Deg2Rad) * changeDistance;
        float deltaY = Mathf.Sin(angle * Mathf.Deg2Rad) * changeDistance;

        Vector3 currentPosition = mainCharacterUnit.transform.position;
        Vector3 targetpos = new Vector3(
            currentPosition.x + deltaX,
            currentPosition.y + deltaY,
            0
        );
        if (_SkillPreviewVisual == null) return;
        _SkillPreviewVisual.transform.position = targetpos;
        _FinalCastPos = _SkillPreviewVisual.transform.position; // 缓存最终施法世界坐标
    }

    public void OnEndDrag(PointerEventData eventData)//技能拖拽结束
    {
        CancelSkillRelease();
        ClearEventTriggerListeners();
    }

    void ExecuteSkillWithTarget(UInt64 target)
    {
        if (_ActiveSkillInfo != null && target != 0)
        {
            IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
            IUnit targetUnit = GameUnitApi.QueryUnit(target);
            Vector3 direction = targetUnit.Position - mainCharacter.Position;
            if (direction.magnitude > AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._MaxRangeRadius)
            {
                XApi.Debug($"Skill {_ActiveSkillInfo.skillConfigID} out of range");
                //Debug.LogError($"目标{targetUnit.Guid}超出了技能{_ActiveSkillInfo.skillConfigID}的最大范围{AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._MaxRangeRadius}");
                return;
            }
            float angle = Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg;

            oVector2 targetpos = new oVector2
            {
                X = targetUnit.Position.x,
                Y = targetUnit.Position.y,
            };
            Vector2 characterPos = mainCharacter.Position;
            oVector2 castPos = new oVector2
            {
                X = characterPos.x,
                Y = characterPos.y
            };

            AutoEnum.eTargetType targetUnitType = SelectUnitType(targetUnit);
            AutoEnum.eTargetType[] configtargetTypes = AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._TargetType;

            if (Array.Exists(configtargetTypes, element => element == targetUnitType))
            {
                _SkillManager.UseSkill(_ActiveSkillInfo.skillGuid, _ActiveSkillInfo.skillConfigID, angle, targetpos, target, castPos);
                Debug.LogWarning($"使用技能，GUID是 {_ActiveSkillInfo.skillGuid} ，技能ID是{_ActiveSkillInfo.skillConfigID} 角度是{angle} 目标位置是{targetpos} 目标GUID是{target}");
                CancelSkillRelease();
            }
            else
            {
                XApi.Debug($"目标类型{targetUnitType}不在技能{_ActiveSkillInfo.skillConfigID}的目标类型列表中");
            }
        }

    }

    // public void CancelSkillRelease()//取消技能释放
    // {
    //     _SkillDragAngle = 0f;
    //     _IsSkillReleasing = false;
    //     XApi.TimerApi.StopTimer(OnLongPressTimer, _CharacterManager.GetMainCharacterUnit().gameObject);
    //     _IsLongPress = false;
    //     _IsDragSkill = false;
    //     IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
    //     GameObject skillRangeParent = mainCharacterUnit.transform.Find("SkillRange").gameObject;//关闭技能范围显示
    //     if (skillRangeParent != null)
    //     {
    //         for (int i = 0; i < skillRangeParent.transform.childCount; i++)
    //         {
    //             GameObject.Destroy(skillRangeParent.transform.GetChild(i).gameObject);
    //         }
    //     }
    //     _ActiveSkillInfo = null;
    //     _CurrentSkillReleaseMode = SkillReleaseMode.None;
    //     _SkillPreviewVisual = null;
    //     ClearEventTriggerListeners();
    //     Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
    //     GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;//关闭退出按钮
    //     if (exitBtn != null)
    //     {
    //         exitBtn.SetActive(false);
    //     }
    //     GameObject parent = mainCharacterUnit.transform.Find("PlacementLab").gameObject;//关闭技能面片
    //     if (parent != null)
    //     {
    //         for (int i = 0; i < parent.transform.childCount; i++)
    //         {
    //             GameObject.Destroy(parent.transform.GetChild(i).gameObject);
    //         }
    //     }
    // }
    public void CancelSkillRelease()//取消技能释放
    {
        _SkillDragAngle = 0f;
        _IsSkillReleasing = false;

        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        if (mainCharacterUnit != null)
        {
            XApi.TimerApi.StopTimer(OnLongPressTimer, mainCharacterUnit.gameObject);
            _IsLongPress = false;
            _IsDragSkill = false;

            Transform skillRangeTransform = mainCharacterUnit.transform.Find("SkillRange");
            if (skillRangeTransform != null)
            {
                GameObject skillRangeParent = skillRangeTransform.gameObject;//关闭技能范围显示
                if (skillRangeParent != null)
                {
                    for (int i = 0; i < skillRangeParent.transform.childCount; i++)
                    {
                        GameObject.Destroy(skillRangeParent.transform.GetChild(i).gameObject);
                    }
                }
            }

            GameObject parent = mainCharacterUnit.transform.Find("PlacementLab").gameObject;//关闭技能面片
            if (parent != null)
            {
                for (int i = 0; i < parent.transform.childCount; i++)
                {
                    GameObject.Destroy(parent.transform.GetChild(i).gameObject);
                }
            }
        }
        _ActiveSkillInfo = null;
        _CurrentSkillReleaseMode = SkillReleaseMode.None;
        _SkillPreviewVisual = null;
        ClearEventTriggerListeners();
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        if (canvas != null)
        {
            GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;//关闭退出按钮
            if (exitBtn != null)
            {
                exitBtn.SetActive(false);
            }
        }
    }
    private void ClearEventTriggerListeners()
    {
        if (_SkillSlotEventTrigger != null)
        {
            // 移除所有事件监听器
            _SkillSlotEventTrigger.triggers.Clear();
            _SkillSlotEventTrigger = null;
        }
    }


    AutoEnum.eTargetType SelectUnitType(IUnit unit)
    {
        eUnitType unitType = unit.Type;
        switch (unitType)
        {
            case eUnitType.Character://角色
                if (unit.Guid == _CharacterManager.GetMainCharacterUnit().Guid)
                {
                    return AutoEnum.eTargetType.PlayerSelf;
                }
                else
                {
                    return AutoEnum.eTargetType.PlayerCharacter;
                }

            case eUnitType.Monster://生物
                return AutoEnum.eTargetType.Monster;

        }
        return AutoEnum.eTargetType.All;
    }

    public void OnSkillButtonPress(string slot)
    {
        _IsLongPress = false;//重置长按
        if (_CurrentSkillReleaseMode == SkillReleaseMode.TargetSelection)
        {
            IUnit lastDamageUnit = _DamageManager.GetLastDamageUnit();
            IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
            if (lastDamageUnit != null)
            {
                Vector3 direction = lastDamageUnit.Position - mainCharacter.Position;
                if (direction.magnitude <= AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._MaxRangeRadius)
                {
                    ExecuteSkillWithTarget(lastDamageUnit.Guid);
                }
                else
                {
                    XApi.Debug($"Skill {_ActiveSkillInfo.skillConfigID} out of range");
                    Debug.LogError($"目标{lastDamageUnit.Guid}超出了技能{_ActiveSkillInfo.skillConfigID}的最大范围{AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._MaxRangeRadius}");
                }

            }
            else
            {
                return;
            }
            return;
        }
        ClickSkillBtn(slot);

        if (_ActiveSkillInfo != null && _CurrentSkillReleaseMode == SkillReleaseMode.Instant)
        {
            XApi.TimerApi.StartTimer(OnLongPressTimer, slot, null, (int)AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._ShowRangeTime, 1, 0);
        }
    }

    private void OnLongPressTimer(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        if (state == XApi.ITimer.eState.END && _ActiveSkillInfo != null)
        {
            _IsLongPress = true;
            Int64 skillConfigId = _ActiveSkillInfo.skillConfigID;
            GameObject skillRange = Resources.Load<GameObject>("UI/Sprites/Skill/Ring");//加载技能范围
            IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
            GameObject skillRangeParent = mainCharacterUnit.transform.Find("SkillRange").gameObject;//找到主角色的技能范围父物体
            if (skillRangeParent != null)
            {
                for (int i = 0; i < skillRangeParent.transform.childCount; i++)
                {
                    GameObject.Destroy(skillRangeParent.transform.GetChild(i).gameObject);
                }
            }
            GameObject skillRangeInstance = GameObject.Instantiate(skillRange);
            skillRangeInstance.transform.SetParent(skillRangeParent.transform);//设置技能范围的父物体为主角色的技能范围
            skillRangeInstance.transform.localPosition = Vector3.zero;
            Vector3 scale = skillRangeInstance.transform.localScale;
            scale.x = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
            scale.y = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
            skillRangeInstance.transform.localScale = scale;
        }
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;
        if (exitBtn != null)
        {
            exitBtn.SetActive(true);
        }
    }

    public void OnSkillButtonRelease(string slot)
    {
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        _IsSkillReleasing = false;
        switch (_CurrentSkillReleaseMode)
        {
            case SkillReleaseMode.Instant:
                XApi.TimerApi.StopTimer(OnLongPressTimer, slot);

                if (_IsLongPress)
                {
                    GameObject skillRangeParent = mainCharacterUnit.transform.Find("SkillRange").gameObject;//找到主角色的技能范围父物体
                    if (skillRangeParent != null)
                    {
                        for (int i = 0; i < skillRangeParent.transform.childCount; i++)
                        {
                            GameObject.Destroy(skillRangeParent.transform.GetChild(i).gameObject);
                        }
                    }

                    _IsLongPress = false;
                    if (_ActiveSkillInfo != null)
                    {
                        AutoConfig.Skill currentskill = AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID);
                        ExecuteSkill(currentskill);
                        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                        GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;
                        if (exitBtn != null)
                        {
                            exitBtn.SetActive(false);
                        }
                    }
                }
                else
                {
                    if (_ActiveSkillInfo != null)
                    {
                        AutoConfig.Skill currentskill = AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID);
                        ExecuteSkill(currentskill);
                        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                        GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;
                        if (exitBtn != null)
                        {
                            exitBtn.SetActive(false);
                        }
                    }
                }
                CancelSkillRelease();
                break;

            case SkillReleaseMode.TargetSelection:
                break;
            case SkillReleaseMode.DragPlacement:
                float endangle = 0;
                Vector2 endPos = Vector2.zero;

                if (_IsDragSkill == false)
                {
                    IUnit lastDamageUnit = _DamageManager.GetLastDamageUnit();
                    if (lastDamageUnit == null)
                    {
                        IUnit nowAttackUnit = GetCanAttackUint(_ActiveSkillInfo.skillConfigID);
                        if (nowAttackUnit != null)
                        {
                            endPos = new Vector2(nowAttackUnit.Position.x, nowAttackUnit.Position.y);
                            _SkillDragAngle = Mathf.Atan2(nowAttackUnit.Position.y - mainCharacterUnit.Position.y, nowAttackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                            // if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == true)
                            // {
                            //     _SkillPreviewVisual.transform.position = new Vector3(endPos.x, endPos.y, 0);
                            //     // Debug.LogError($"技能目标位置{nowAttackUnit.Position}，最终目标位置{endPos}，面片位置{_SkillPreviewVisual.transform.position}，技能角度{_SkillDragAngle}");
                            // }
                            // else if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == false)
                            // {
                            //     _SkillPreviewVisual.transform.eulerAngles = new Vector3(_SkillDragAngle, -90, -90);
                            //     _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
                            // }
                            Debug.LogWarning($"以技能范围内最近的单位为目标");
                        }
                        else
                        {
                            endPos = new Vector2(mainCharacterUnit.Position.x, mainCharacterUnit.Position.y);
                            _SkillDragAngle = mainCharacterUnit.Angle;
                            Debug.LogWarning("无目标，使用自身位置和角度");
                        }

                    }
                    else
                    {
                        float distance = Vector2.Distance(mainCharacterUnit.Position, lastDamageUnit.Position);
                        if (distance <= AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._MaxSkillDistacne)
                        {
                            endPos = new Vector2(lastDamageUnit.Position.x, lastDamageUnit.Position.y);
                            _SkillDragAngle = Mathf.Atan2(lastDamageUnit.Position.y - mainCharacterUnit.Position.y, lastDamageUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                            // if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == true)
                            // {
                            //     _SkillPreviewVisual.transform.position = new Vector3(endPos.x, endPos.y, 0);
                            // }
                            // else if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._CanSeparated == false)
                            // {
                            //     _SkillPreviewVisual.transform.eulerAngles = new Vector3(_SkillDragAngle, -90, -90);
                            //     _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
                            // }
                            Debug.LogWarning($"技能距离满足，目标位置：{endPos}，角度：{_SkillDragAngle}");
                        }
                        else
                        {
                            IUnit nowAttackUnit = GetCanAttackUint(_ActiveSkillInfo.skillConfigID);
                            if (nowAttackUnit != null)
                            {
                                endPos = new Vector2(nowAttackUnit.Position.x, nowAttackUnit.Position.y);
                                _SkillDragAngle = Mathf.Atan2(nowAttackUnit.Position.y - mainCharacterUnit.Position.y, nowAttackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                                // if (_SkillPreviewVisual != null)
                                // {
                                //     _SkillPreviewVisual.transform.position = new Vector3(endPos.x, endPos.y, 0);
                                // }
                                Debug.LogWarning($"以技能范围内最近的单位为目标");
                            }
                            else
                            {
                                _SkillDragAngle = mainCharacterUnit.Angle;
                                endPos = new Vector2(mainCharacterUnit.Position.x, mainCharacterUnit.Position.y);
                                Debug.LogWarning("技能距离不满足，使用自身位置和角度");
                            }
                        }
                    }

                }
                else
                {
                    Debug.LogWarning("结束拖动");
                    endPos = new Vector2(_FinalCastPos.x, _FinalCastPos.y);
                }
                oVector2 targetpos = new oVector2//最终拖动位置
                {
                    X = endPos.x,
                    Y = endPos.y
                };


                Vector2 dragDistance = endPos - new Vector2(_CharacterManager.GetMainCharacterUnit().transform.position.x, _CharacterManager.GetMainCharacterUnit().transform.position.y);
                if (dragDistance.magnitude >= _DragSkillDeadZone)
                {
                    endangle = _SkillDragAngle;
                }
                Vector2 characterPos = mainCharacterUnit.Position;
                oVector2 castPos = new oVector2
                {
                    X = characterPos.x,
                    Y = characterPos.y
                };

                _SkillManager.UseSkill(_ActiveSkillInfo.skillGuid, _ActiveSkillInfo.skillConfigID, endangle, targetpos, 0, castPos);
                Debug.LogWarning($"拖动结束，拖动距离：{dragDistance.magnitude}，拖动角度：{endangle}，目标位置：{targetpos}，技能GUID：{_ActiveSkillInfo.skillGuid}，技能ID：{_ActiveSkillInfo.skillConfigID}");
                CancelSkillRelease();
                break;
        }
    }

    public void ChangeAtkInfo(UInt64 playerGuid, Int64 configID, Int64 timetick)
    {
        SetCharacterLastAttackConfigID(playerGuid, configID);
        SetCharacterLastAttackTime(playerGuid, timetick);
    }


    // 获取指定角色的上次攻击配置ID
    private Int64 GetCharacterLastAttackConfigID(UInt64 characterGuid)
    {
        if (_CharacterLastAttackConfigID.ContainsKey(characterGuid))
        {
            return _CharacterLastAttackConfigID[characterGuid];
        }
        return 0;
    }

    // 设置指定角色的上次攻击配置ID
    private void SetCharacterLastAttackConfigID(UInt64 characterGuid, Int64 configID)
    {
        _CharacterLastAttackConfigID[characterGuid] = configID;
    }

    // 获取指定角色的上次攻击时间
    private Int64 GetCharacterLastAttackTime(UInt64 characterGuid)
    {
        if (_CharacterLastAttackTime.ContainsKey(characterGuid))
        {
            return _CharacterLastAttackTime[characterGuid];
        }
        return 0;
    }

    // 设置指定角色的上次攻击时间
    private void SetCharacterLastAttackTime(UInt64 characterGuid, Int64 time)
    {
        _CharacterLastAttackTime[characterGuid] = time;
    }



    IUnit GetCanAttackUint(Int64 skillconfigID)//获取可攻击的Unit用于智能施法
    {
        List<IUnit> potentialTargets = new List<IUnit>();//可攻击的Unit列表
        AutoEnum.eTargetType[] targetType = AutoConfig.Skill.Get(skillconfigID)._TargetType;
        for (int i = 0; i < targetType.Length; i++)
        {
            eUnitType unitType = ChangeClickType(targetType[i]);//转换类型
            var units = GameUnitApi.GetUnitsByType(unitType);
            if (units != null)
            {
                foreach (var unit in units)
                {
                    if (unit.Value != null)
                    {
                        potentialTargets.Add(unit.Value);
                    }
                }
            }
        }

        if (potentialTargets.Count == 0)
        {
            Debug.LogWarning("没有可攻击的Unit");
            return null;
        }
        //此时获得所有可以使用此技能的Unit,进行距离内unit筛选
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        float skillDistance = AutoConfig.Skill.Get(skillconfigID)._MaxSkillDistacne;//技能最大距离
        List<IUnit> targetInRange = new List<IUnit>();
        if (mainCharacterUnit != null)
        {
            foreach (var unit in potentialTargets)
            {
                if (unit.GetBool(eEntityAttr.IsCantDamaged_Bool) == true)
                {
                    continue;//技能不能对这个单位造成伤害，跳过
                }
                
                float distance = Vector2.Distance(mainCharacterUnit.Position, unit.Position);
                
                if (distance <= skillDistance)
                {
                    targetInRange.Add(unit);
                    //Debug.LogError($"Unit类型：{unit.Type}，Unit名字：{unit.ConfigID}，距离：{distance}，技能设定：{skillDistance}");
                }
            }
        }
        if (targetInRange.Count == 0)
        {
            Debug.LogError($"技能距离内没有可施法的目标");
            return null;
        }
        //获得技能范围内对应的可以使用技能的Unit，进行优先级判断
        List<IUnit> priorityTargets = new List<IUnit>();
        int minPriority = int.MaxValue;
        foreach (var unit in targetInRange)
        {
            int unitdamagepriority = unit.GetInt32(GameUnit.eEntityAttr.DamagePriority_Int32);
            if (unitdamagepriority < minPriority)
            {
                minPriority = unitdamagepriority;
                priorityTargets.Clear();
                priorityTargets.Add(unit);
            }
            else if (unitdamagepriority == minPriority)
            {
                priorityTargets.Add(unit);
            }
        }
        //获得技能范围以内的最高优先级的Unit，进行距离判断
        if (priorityTargets.Count > 0)
        {
            IUnit closestTarget = priorityTargets[0];
            float closestDistance = Vector2.Distance(mainCharacterUnit.Position, closestTarget.Position);
            foreach (var unit in priorityTargets)
            {
                float distance = Vector2.Distance(mainCharacterUnit.Position, unit.Position);
                if (distance < closestDistance)
                {
                    closestDistance = distance;
                    closestTarget = unit;
                }
            }
            //Debug.LogError($"技能距离内可施法的目标有{targetInRange.Count}个，最高优先级为{minPriority}，技能范围是{skillDistance}，他和我的距离是{closestDistance}");
            return closestTarget;

        }
        return null;
    }

}
