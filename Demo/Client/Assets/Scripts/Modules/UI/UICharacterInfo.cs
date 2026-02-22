using GameUnit;
using Spine.Unity;
using System;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XGame;
using XUtils;
using static IUICharacterInfo;
using static GameUnitApi;

public interface IUICharacterInfo : IModule
{
    enum eNodeType
    {
        PrepareBattle = 0,
        Skill = 1,
    }

    enum eEvent
    { 
        RegistSkillUI = 0,
    }

    void RegistEvent(eEvent eventID, Action<GameObject, UInt64> callback);
    void UnRegistEvent(eEvent eventID, Action<GameObject, UInt64> callback);

    public void OpenCharacterInfo();
}

class UICharacterInfo : IUICharacterInfo
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IUICharacterInfo, UICharacterInfo>();
    }

    GameObject _UIPanelCharacterInfo;
    ICharacterManager _CharacterManager;
    IUIMain _UIMain;

    Int64 _SelectCharacterConfigId;
    eNodeType _SelectNodeIndex;

    XEventPool<IUICharacterInfo.eEvent> _Events = new XEventPool<IUICharacterInfo.eEvent> ();
    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _UIMain = Launcher.GetModule<IUIMain>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, OnMainCharacterEnter);
        _UIMain.RegistEvent(IUIMain.eEvent.OpenCharacterInfo, OpenCharacterInfo);
        return true;
    }

    private void OnMainCharacterEnter(GameUnitApi.IUnit unit)
    {
        if (_UIPanelCharacterInfo != null)
        {
            UpdateCharacterInfoUI();
            _Events.Trigger(IUICharacterInfo.eEvent.RegistSkillUI, _UIPanelCharacterInfo.transform.Find("NodeSkill").gameObject, GetCharacterIdByConfigId(_SelectCharacterConfigId));
        }
    }

    public void OpenCharacterInfo()
    {
        if (_UIPanelCharacterInfo == null)
        {
            GameObject panelCharacterInfo = Resources.Load<GameObject>("UI/Prefabs/Character/UIPanelCharacterInfo");
            _UIPanelCharacterInfo = GameObject.Instantiate(panelCharacterInfo);
            _UIPanelCharacterInfo.transform.parent = GameObject.FindFirstObjectByType<Canvas>().transform;

            _UIPanelCharacterInfo.GetComponent<RectTransform>().offsetMax = new Vector2(0, 0);
            _UIPanelCharacterInfo.GetComponent<RectTransform>().offsetMin = new Vector2(0, 0);
            _UIPanelCharacterInfo.transform.localScale = Vector3.one;

            InitCharacterInfoUI();
        }
        else
        {
            _UIPanelCharacterInfo.SetActive(true);
        }

        UpdateCharacterInfoUI();
    }

    public void RegistEvent(IUICharacterInfo.eEvent eventID, Action<GameObject, UInt64> callback)
    {
        _Events.Regist(eventID, callback);
    }

    public void UnRegistEvent(IUICharacterInfo.eEvent eventID, Action<GameObject, UInt64> callback)
    {
        _Events.CancelRegist(eventID, callback);
    }

    public void InitCharacterInfoUI()
    {
        GameObject go = Resources.Load<GameObject>("UI/Prefabs/Character/UICharacterItem");

        foreach(var kv in _CharacterManager.GetSelfCharacters())
        {
            IUnit character = kv.Value;

            GameObject characterItem = GameObject.Instantiate(go);
            characterItem.transform.parent = _UIPanelCharacterInfo.transform.Find("ScrollViewCharacterList/Viewport/Content");
            characterItem.name = $"{character.ConfigID}";
            characterItem.transform.localScale = Vector3.one;
            characterItem.transform.Find("ImageHead").GetComponent<Image>().sprite = Resources.Load<Sprite>(AutoConfig.Character.Get(character.ConfigID)._DirIcon);
            characterItem.transform.Find("ImageJob").GetComponent<Image>().sprite = Resources.Load<Sprite>(AutoConfig.Character.Get(character.ConfigID)._CharacterLogo);
            characterItem.transform.Find("TextCharacterName").GetComponent<TextMeshProUGUI>().text = AutoConfig.Character.Get(character.ConfigID)._Name;

            if (_SelectCharacterConfigId <= 0)
            {
                _SelectCharacterConfigId = character.ConfigID;
                _SelectNodeIndex = eNodeType.PrepareBattle;
            }

            characterItem.transform.Find("BtnHead").GetComponent<Button>().onClick.AddListener(() =>
            {
                OnCharacterHeadClick(character.ConfigID);
            });
        }

        RegistBtnClickEvent();

        _Events.Trigger(IUICharacterInfo.eEvent.RegistSkillUI, _UIPanelCharacterInfo.transform.Find("NodeSkill").gameObject, GetCharacterIdByConfigId(_SelectCharacterConfigId));
    }

    public void RegistBtnClickEvent()
    {
        _UIPanelCharacterInfo.transform.Find("NodeTop/BtnBack").GetComponent<Button>().onClick.AddListener(() =>
        {
            CloseCharacterInfo();
        });

        _UIPanelCharacterInfo.transform.Find("NodeButtons/BtnPrepareBattle").GetComponent<Button>().onClick.AddListener(() =>
        {
            _SelectNodeIndex = eNodeType.PrepareBattle;
            UpdateNodeShow();
        });

        _UIPanelCharacterInfo.transform.Find("NodeButtons/BtnSkill").GetComponent<Button>().onClick.AddListener(() =>
        {
            _SelectNodeIndex = eNodeType.Skill;
            UpdateNodeShow();
        });

        _UIPanelCharacterInfo.transform.Find("NodePrepareBattle/NodeCharacterInfo/BtnGoBattle").GetComponent<Button>().onClick.AddListener(() =>
        {
            _CharacterManager.SendSwitchCharacter(GetCharacterIdByConfigId(_SelectCharacterConfigId));
        });
    }

    public void CloseCharacterInfo()
    { 
        _UIPanelCharacterInfo.SetActive(false);
    }

    public void OnCharacterHeadClick(Int64 configId)
    {
        if (_SelectCharacterConfigId == configId)
        {
            return;
        }

        _SelectCharacterConfigId = configId;
        UpdateCharacterInfoUI();

        _Events.Trigger(IUICharacterInfo.eEvent.RegistSkillUI, _UIPanelCharacterInfo.transform.Find("NodeSkill").gameObject, GetCharacterIdByConfigId(_SelectCharacterConfigId));
    }

    public void UpdateCharacterInfoUI()
    {
        UpdateNodeCharacterListUI();
        UpdateNodeCharacterShow();
        UpdateNodeCharacterInfoUI();
        UpdateNodeShow();
    }
     
    public void UpdateNodeShow()
    { 
        _UIPanelCharacterInfo.transform.Find("NodePrepareBattle").gameObject.SetActive(_SelectNodeIndex == eNodeType.PrepareBattle);
        _UIPanelCharacterInfo.transform.Find("NodeSkill").gameObject.SetActive(_SelectNodeIndex == eNodeType.Skill);

        _UIPanelCharacterInfo.transform.Find("NodeButtons/BtnPrepareBattle/ImageSelect").gameObject.SetActive(_SelectNodeIndex == eNodeType.PrepareBattle);
        _UIPanelCharacterInfo.transform.Find("NodeButtons/BtnSkill/ImageSelect").gameObject.SetActive(_SelectNodeIndex == eNodeType.Skill);
    }

    public void UpdateNodeCharacterListUI()
    {
        foreach(var kv in _CharacterManager.GetSelfCharacters())
        {
            IUnit character = kv.Value;

            GameObject characterItem = _UIPanelCharacterInfo.transform.Find("ScrollViewCharacterList/Viewport/Content").Find($"{character.ConfigID}").gameObject;
            characterItem.transform.Find("ImageHeadSelect").gameObject.SetActive(_SelectCharacterConfigId == character.ConfigID);
            characterItem.transform.Find("ImageOnBattle").gameObject.SetActive(false);
            if (GetCharacterUnlock(character.ConfigID) != null)
            {
                characterItem.transform.Find("TextCharacterLevel").GetComponent<TextMeshProUGUI>().text = $"{GetCharacterUnlock(character.ConfigID).GetInt64(eCharacterAttr.Level_Int64)}级";
                characterItem.transform.Find("ImageOnBattle").gameObject.SetActive(GetCharacterUnlock(character.ConfigID).Guid == _CharacterManager.GetMainCharacterUnit().Guid);
            }
            else
            {
                characterItem.transform.Find("TextCharacterLevel").GetComponent<TextMeshProUGUI>().text = "1级";
            }
        }
    }

    public void UpdateNodeCharacterInfoUI()
    {
        GameObject characterInfo = _UIPanelCharacterInfo.transform.Find("NodePrepareBattle/NodeCharacterInfo").gameObject;
        Transform Content = characterInfo.transform.Find("ScrollViewCharacterInfo/Viewport/Content");
        characterInfo.transform.Find("TextCharacterName").GetComponent<TextMeshProUGUI>().text = AutoConfig.Character.Get(_SelectCharacterConfigId)._Name;
        Content.Find("Hp/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.Hp}值:";
        Content.Find("Atk/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.Atk}值:"; 
        Content.Find("Blood/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.Blood}值:";
        Content.Find("PhysicalPierce/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.PhysicalPierce}值:"; 
        Content.Find("MagicPierce/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.MagicPierce}值:"; 
        Content.Find("Defend/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.Defend}值:"; 
        Content.Find("MagicDefend/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.MagicDefend}值:"; 
        Content.Find("Afford/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.Weight}值:"; 
        Content.Find("Speed/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.Speed}值:"; 
        Content.Find("Critical/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.Critical}值:"; 
        Content.Find("CriticalDamage/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.CriticalDamage}值:"; 
        Content.Find("Resilience/TextAttribute").GetComponent<TextMeshProUGUI>().text = $"{AutoConfig.AutoConst.Attribute.Resilience}值:"; 
        characterInfo.transform.Find("BtnOnBattle").gameObject.SetActive(false);
        characterInfo.transform.Find("BtnGoBattle").gameObject.SetActive(true);
        if (GetCharacterUnlock(_SelectCharacterConfigId) != null)
        {
            characterInfo.transform.Find("TextLevel").GetComponent<TextMeshProUGUI>().text = $"{GetCharacterUnlock(_SelectCharacterConfigId).GetInt64(eCharacterAttr.Level_Int64)}级";
            Content.Find("Hp/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.Hp_Float);
            Content.Find("Atk/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.Atk_Float);
            Content.Find("Blood/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.Blood_Float);
            Content.Find("PhysicalPierce/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.PhysicalPierce_Float);
            Content.Find("MagicPierce/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.MagicPierce_Float);
            Content.Find("Defend/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.Defend_Float);
            Content.Find("MagicDefend/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.MagicDefend_Float);
            Content.Find("Afford/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.Afford_Float);
            Content.Find("Speed/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.Speed_Float);
            Content.Find("Critical/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.Critical_Float);
            Content.Find("CriticalDamage/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.CriticalDamage_Float);
            Content.Find("Resilience/Text").GetComponent<TextMeshProUGUI>().text = "" + GetCharacterUnlock(_SelectCharacterConfigId).GetFloat(eCharacterAttr.Resilience_Float);
            characterInfo.transform.Find("BtnOnBattle").gameObject.SetActive(GetCharacterUnlock(_SelectCharacterConfigId).Guid == _CharacterManager.GetMainCharacterUnit().Guid);
            characterInfo.transform.Find("BtnGoBattle").gameObject.SetActive(GetCharacterUnlock(_SelectCharacterConfigId).Guid != _CharacterManager.GetMainCharacterUnit().Guid);
        }
        else
        {
            Content.Find("Hp/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialHp;
            Content.Find("Atk/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialAtk;
            Content.Find("Blood/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialBlood;
            Content.Find("PhysicalPierce/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialPhysicalPierce;
            Content.Find("MagicPierce/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialMagicPierce;
            Content.Find("Defend/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialDefend;
            Content.Find("MagicDefend/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialMagicDefend;
            Content.Find("Afford/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialAfford;
            Content.Find("Speed/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialSpeed;
            Content.Find("Critical/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialCritical;
            Content.Find("CriticalDamage/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialCriticalDamage;
            Content.Find("Resilience/Text").GetComponent<TextMeshProUGUI>().text = "" + AutoConfig.Character.Get(_SelectCharacterConfigId)._InitialResilience;
        }
    }

    public GameUnitApi.IUnit GetCharacterUnlock(Int64 configId)
    {
        foreach (var value in _CharacterManager.GetSelfCharacters())
        {
            if (value.Value.ConfigID == configId)
            { 
                return value.Value;
            }
        }
        
        return null;
    }

    public UInt64 GetCharacterIdByConfigId(Int64 configId)
    {
        foreach (var value in _CharacterManager.GetSelfCharacters())
        {
            if (value.Value.ConfigID == configId)
            {
                return value.Key;
            }
        }

        return INVALID_UNIT_GUID;
    }

    public void UpdateNodeCharacterShow()
    {
        GameObject characterShow = _UIPanelCharacterInfo.transform.Find("NodePrepareBattle/NodeCharacterShow").gameObject;
        characterShow.transform.Find("CharacterSpine").GetComponent<SkeletonGraphic>().skeletonDataAsset = Resources.Load<SkeletonDataAsset>(AutoConfig.Character.Get(_SelectCharacterConfigId)._PrefabShow);
        characterShow.transform.Find("CharacterSpine").GetComponent<SkeletonGraphic>().startingAnimation = "idle";
        characterShow.transform.Find("CharacterSpine").GetComponent<SkeletonGraphic>().startingLoop = true;
        characterShow.transform.Find("CharacterSpine").GetComponent<SkeletonGraphic>().Initialize(true);
    }

    public void Release()
    {

    }
}
