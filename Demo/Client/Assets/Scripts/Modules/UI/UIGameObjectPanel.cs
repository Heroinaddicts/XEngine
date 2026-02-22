
using AutoConfig;
using DG.Tweening;
using GameUnit;
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using XGameClient;
using static GameUnitApi;

public interface IUIGameObjectPanel : IModule
{
    public GameObject CreatePanel(in eUnitType type, in UInt64 guid, in IUnit go);
    public GameObject QueryPanel(in UInt64 guid);
    public void RemovePanel(in UInt64 guid);
}

class UIGameObjectPanel : IUIGameObjectPanel
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IUIGameObjectPanel, UIGameObjectPanel>();
    }

    class PanelInfo
    {
        public readonly IUnit _Unit;
        public readonly GameObject _Panel;
        public PanelInfo(IUnit go, GameObject panel)
        {
            _Unit = go;
            _Panel = panel;
        }
    }

    Dictionary<UInt64, PanelInfo> _Panels = new Dictionary<UInt64, PanelInfo>();


    ICharacterManager _CharacterManager;
    IGameObjectSync _GameObjectSync;
    IGameSceneManager _GameSceneManager;
    ITeamManager _TeamManager;
    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, OnMainCharacterEnter);
        _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterExit, OnMainCharacterExit);

        _GameObjectSync = Launcher.GetModule<IGameObjectSync>();
        _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncCreate, OnSyncCreate);
        _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncRelease, OnSyncRelease);
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
        _TeamManager = Launcher.GetModule<ITeamManager>();

        _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.GameMapLoaded, OnSceneLoaded);

        return true;
    }

    private void OnSceneLoaded(UnityEngine.SceneManagement.Scene scene)//切换人物头顶UI
    {
        AutoConfig.Scene currentSceneConfig = _GameSceneManager.GetCurrentSceneConfig();
        if (currentSceneConfig == null)
        {
            return;
        }

        if (currentSceneConfig._SceneType == AutoEnum.eSceneType.Public)
        {
            var characters = GameUnitApi.GetUnitsByType(eUnitType.Character);
            if (characters != null)
            {
                Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                if (canvas == null)
                {
                    XApi.Error("找不到主Canvas");
                    return;
                }

                Transform topUILab = canvas.transform.Find("TopUILab");
                if (topUILab == null)
                {
                    XApi.Error("找不到TopUILab");
                    return;
                }

                GameObject topUIitem = topUILab.gameObject;

                foreach (var kvp in characters)
                {
                    UInt64 guid = kvp.Key;
                    IUnit character = kvp.Value;

                    Transform characterUI = topUIitem.transform.Find("CharacterTopUI" + guid);
                    if (characterUI == null)
                    {
                        Debug.LogWarning($"找不到角色UI: CharacterTopUI{guid}");
                        continue;
                    }

                    Transform publicUI = characterUI.transform.Find("PublicUI");
                    Transform battleTopUI = characterUI.transform.Find("BattleTopUI");

                    if (publicUI != null) publicUI.gameObject.SetActive(true);
                    if (battleTopUI != null) battleTopUI.gameObject.SetActive(false);
                }
            }
        }
        else if (currentSceneConfig._SceneType == AutoEnum.eSceneType.BattleCopy || currentSceneConfig._SceneType == AutoEnum.eSceneType.PublicBattle)
        {
            var characters = GameUnitApi.GetUnitsByType(eUnitType.Character);
            if (characters != null)
            {
                Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                if (canvas == null)
                {
                    XApi.Error("找不到主Canvas");
                    return;
                }

                Transform topUILab = canvas.transform.Find("TopUILab");
                if (topUILab == null)
                {
                    XApi.Error("找不到TopUILab");
                    return;
                }

                GameObject topUIitem = topUILab.gameObject;

                foreach (var kvp in characters)
                {
                    UInt64 guid = kvp.Key;
                    IUnit character = kvp.Value;

                    Transform characterUI = topUIitem.transform.Find("CharacterTopUI" + guid);
                    if (characterUI == null)
                    {
                        Debug.LogWarning($"找不到角色UI: CharacterTopUI{guid}");
                        continue;
                    }

                    Transform publicUI = characterUI.transform.Find("PublicUI");
                    Transform battleTopUI = characterUI.transform.Find("BattleTopUI");

                    if (publicUI != null) publicUI.gameObject.SetActive(false);
                    if (battleTopUI != null) battleTopUI.gameObject.SetActive(true);
                }
            }
        }
    }

    public void Release()
    {

    }

    void OnMainCharacterEnter(IUnit character)
    {
        CreatePanel(character.Type, character.Guid, character);
    }

    void OnMainCharacterExit(IUnit character)
    {
        RemovePanel(character.Guid);
    }


    void OnSyncCreate(IUnit unit)
    {
        if (unit.Type == eUnitType.Boss)
        {
            CreateBossPanel(unit, unit.Guid);
            return;
        }
        CreatePanel(unit.Type, unit.Guid, unit);
    }

    void OnSyncRelease(IUnit unit)
    {
        if (unit.Type == eUnitType.Boss)
        {
            ReleaseBossPanel(unit, unit.Guid);
            return;
        }
        RemovePanel(unit.Guid);
    }



    public GameObject CreatePanel(in eUnitType type, in ulong guid, in IUnit go)//创建面板
    {

        switch (type)
        {
            case eUnitType.Character:
                {
                    if (_Panels.ContainsKey(guid))
                    {
                        return null;
                    }
                    return CreateCharacterPanel(go, guid);
                }
            case eUnitType.Monster:
                {
                    if (_Panels.ContainsKey(guid))
                    {
                        return null;
                    }
                    return CreateMonsterPanel(go, guid);
                }
        }

        return null;
    }

    public GameObject QueryPanel(in ulong guid)//查询面板
    {
        PanelInfo searchInfo = null;
        if (_Panels.TryGetValue(guid, out searchInfo))
        {
            return searchInfo._Panel;
        }
        return null;

    }

    public void RemovePanel(in ulong guid)//移除面板
    {
        PanelInfo info = null;
        if (!_Panels.TryGetValue(guid, out info))
        {
            return;
        }

        GameObject.Destroy(info._Panel);
        _Panels.Remove(guid);
    }

    public void Update()//更新面板信息
    {
        List<ulong> toRemove = new List<ulong>();

        foreach (var Item in _Panels)
        {
            ulong guid = Item.Key;
            PanelInfo info = Item.Value;

            if (info._Unit == null || info._Panel == null)
            {
                if (info._Panel != null)
                {
                    GameObject.Destroy(info._Panel);
                }
                toRemove.Add(guid);
                continue;
            }

            Transform UITop = null;
            Queue<Transform> transformQueue = new Queue<Transform>();
            transformQueue.Enqueue(info._Unit.transform);
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

            if (UITop != null && info._Panel != null)
            {

                Camera mainCamera = Camera.main;
                if (mainCamera != null)
                {
                    // 将世界坐标转换为屏幕坐标
                    Vector3 screenPos = mainCamera.WorldToScreenPoint(UITop.position);

                    // 检查目标是否在摄像机视野内
                    if (screenPos.z > 0 &&
                        screenPos.x >= 0 && screenPos.x <= Screen.width &&
                        screenPos.y >= 0 && screenPos.y <= Screen.height)
                    {
                        Canvas canvas = info._Panel.GetComponentInParent<Canvas>();
                        if (canvas != null)
                        {
                            RectTransform canvasRect = canvas.GetComponent<RectTransform>();
                            RectTransform panelRect = info._Panel.GetComponent<RectTransform>();

                            if (canvasRect != null && panelRect != null)
                            {
                                // 将屏幕坐标转换为Canvas本地坐标
                                Vector2 localPoint;
                                if (RectTransformUtility.ScreenPointToLocalPointInRectangle(
                                        canvasRect,
                                        screenPos,
                                        canvas.renderMode == RenderMode.ScreenSpaceOverlay ? null : canvas.worldCamera,
                                        out localPoint))
                                {
                                    panelRect.anchoredPosition = localPoint;

                                    if (info._Unit.Type != eUnitType.Monster)
                                    {
                                        info._Panel.SetActive(true);
                                    }
                                    if (info._Unit.Type == eUnitType.Monster)
                                    {
                                        bool currentIsCantDamage = info._Unit.GetBool(eEntityAttr.IsCantDamaged_Bool);
                                        info._Panel.SetActive(!currentIsCantDamage);
                                    }

                                }
                            }
                        }
                    }
                    else
                    {
                        info._Panel.SetActive(false);
                    }
                }
            }
        }
        foreach (ulong guid in toRemove)
        {
            _Panels.Remove(guid);
        }
    }

    bool OnCharacterLevelChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in Int64 oldValue, in Int64 newValue) 
    {
        GameObject expLab = GameObjectCommon.GetGameObjectByPath("Canvas/UIMain/HeadIcon/ExpLab");
        if (expLab != null)
        {
            Int64 exp = Int64.MaxValue;
            AutoConfig.CharacterLevel config = AutoConfig.CharacterLevel.Get(newValue);
            if (config != null) 
            {
                exp = config._UpExp;
            }

            var slider = expLab.transform.Find("Slider")?.GetComponent<Slider>();
            if (slider != null) slider.maxValue = exp;
            var maxText = expLab.transform.Find("Max")?.GetComponent<TextMeshProUGUI>();
            if (maxText != null) maxText.text = Mathf.Ceil(exp).ToString();
            var bgSlider = expLab.transform.Find("Slider/BGSlider")?.GetComponent<Slider>();
            if (bgSlider != null) bgSlider.maxValue = exp;
        }

        return true;
    }

    bool OnCharacterExpChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in Int64 oldValue, in Int64 newValue)
    {
        GameObject expLab = GameObjectCommon.GetGameObjectByPath("Canvas/UIMain/HeadIcon/ExpLab");
        if (expLab != null)
        {
            var slider = expLab.transform.Find("Slider")?.GetComponent<Slider>();
            if (slider != null)
            {
                slider.value = newValue;
                if (slider.value < 0)
                {
                    slider.value = 0;
                }
            }
            var currentText = expLab.transform.Find("Current")?.GetComponent<TextMeshProUGUI>();
            if (currentText != null) currentText.text = Mathf.Ceil(newValue).ToString();

            var bgSlider = expLab.transform.Find("Slider/BGSlider").GetComponent<Slider>();
            if (bgSlider.value < newValue)
            {
                bgSlider.value = newValue;
                if (bgSlider.value < 0)
                {
                    bgSlider.value = 0;
                }
            }

            StartSliderTween(bgSlider, newValue, AutoConst.HpGradient.BossHpGradient);
        }
        return true;
    }

    private GameObject CreateCharacterPanel(IUnit character, ulong guid)//角色topUI
    {
        try
        {
            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
            Transform canvasTransform = canvas != null ? canvas.transform : null;
            if (canvasTransform == null)
            {
                XApi.Error("找不到主Canvas，无法创建头顶UI");
                return null;
            }
            Transform topUILab = canvasTransform.transform.Find("TopUILab");
            if (topUILab == null)
            {
                Debug.LogWarning("找不到TopUILab,按照原canvas生成");
            }

            GameObject topUIPre = Resources.Load<GameObject>(AutoConfig.AutoConst.UI.TopUI);
            if (topUIPre == null)
            {
                XApi.Error("TopUI.prefab 加载失败，检查路径");
                return null;
            }

            Transform parent = topUILab != null ? topUILab : canvasTransform;
            GameObject topUIPanel = GameObject.Instantiate(topUIPre, parent);
            topUIPanel.name = "CharacterTopUI" + guid;
            topUIPanel.transform.SetSiblingIndex(0);
            _Panels[guid] = new PanelInfo(character, topUIPanel);
            AutoConfig.Scene currentSceneConfig = _GameSceneManager.GetCurrentSceneConfig();
            string nickname = "";
            Int64 configId = character.ConfigID;

            nickname = character.GetString(eCharacterAttr.Nickname_String);

            Int64 level = character.GetInt64(eCharacterAttr.Level_Int64);

            topUIPanel.transform.Find("PublicUI/PlayerName").GetComponent<Text>().text = nickname;
            topUIPanel.transform.Find("PublicUI/Level").GetComponent<Text>().text = $"(Lv{level})";

            AutoConfig.Character characterConfig = AutoConfig.Character.Get(configId);
            if (characterConfig != null)
            {
                topUIPanel.transform.Find("PublicUI/RoleName").GetComponent<Text>().text = characterConfig._Name;
                topUIPanel.transform.Find("BattleTopUI/RoleName").GetComponent<TextMeshProUGUI>().text = characterConfig._Name;
            }
            

            character.RegistInt64AttributeChange(eCharacterAttr.Level_Int64, eUnitAttributeEvent.Changed,
                (in eUnitAttributeEvent ev, in IUnit unit, in int index, in Int64 oldValue, in Int64 newValue) =>
                {
                    try
                    {
                        topUIPanel.transform.Find("PublicUI/Level").GetComponent<Text>().text = $"(Lv{newValue})";
                        topUIPanel.transform.Find("BattleTopUI/Level").GetComponent<Text>().text = $"(Lv{newValue})";
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"Update character level UI error: {ex.Message}");
                    }
                    return true;
                }
            );

            // 设置战斗UI信息
            topUIPanel.transform.Find("BattleTopUI/CharacterName").GetComponent<Text>().text = nickname;
            topUIPanel.transform.Find("BattleTopUI/Level").GetComponent<Text>().text = $"(Lv{level})";

            float maxHp = 0;
            float currentHp = 0;


            maxHp = character.GetFloat(eCharacterAttr.MaxHp_Float);
            currentHp = character.GetFloat(eCharacterAttr.Hp_Float);

            float maxMP = 0;
            float currentMP = 0;


            maxMP = character.GetFloat(eCharacterAttr.MaxMp_Float);
            currentMP = character.GetFloat(eCharacterAttr.Mp_Float);


            Int64 currentExp = character.GetInt64(eCharacterAttr.Exp_Int64);
            Int64 maxExp = Int64.MaxValue;
            AutoConfig.CharacterLevel config = AutoConfig.CharacterLevel.Get(character.GetInt64(eCharacterAttr.Level_Int64));
            if (null != config)
            {
                maxExp = config._UpExp;
            }

            topUIPanel.transform.Find("BattleTopUI/HpBar").GetComponent<Slider>().maxValue = maxHp;
            topUIPanel.transform.Find("BattleTopUI/HpBar").GetComponent<Slider>().value = currentHp;

            topUIPanel.transform.Find("BattleTopUI/HpBar/BGBar").GetComponent<Slider>().value = currentHp;
            topUIPanel.transform.Find("BattleTopUI/HpBar/BGBar").GetComponent<Slider>().maxValue = maxHp;

            // 主角特殊处理
            IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
            if (mainCharacter != null && mainCharacter.Guid == character.Guid)
            {
                var hpLab = canvas.transform.Find("UIMain/HeadIcon/HPLab");
                var mpLab = canvas.transform.Find("UIMain/HeadIcon/MPLab");
                var expLab = canvas.transform.Find("UIMain/HeadIcon/ExpLab");


                if (hpLab != null && mpLab != null && expLab != null)
                {
                    var maxText = hpLab.Find("Max");
                    var currentText = hpLab.Find("Current");
                    var sliderComp = hpLab.Find("Slider");

                    var mpMaxText = mpLab.Find("Max");
                    var mpCurrentText = mpLab.Find("Current");
                    var mpsliderComp = mpLab.Find("Slider");


                    var expMaxText = expLab.Find("Max");
                    var expCurrentText = expLab.Find("Current");
                    var expsliderComp = expLab.Find("Slider");

                    var bgSlider = hpLab.Find("Slider/BGSlider");
                    var mpBgSlider = mpLab.Find("Slider/BGSlider");


                    if (maxText != null) maxText.GetComponent<TextMeshProUGUI>().text = maxHp.ToString();
                    if (currentText != null) currentText.GetComponent<TextMeshProUGUI>().text = currentHp.ToString() + "/";
                    if (sliderComp != null)
                    {
                        var slider = sliderComp.GetComponent<Slider>();
                        if (slider != null)
                        {
                            slider.maxValue = maxHp;
                            slider.value = currentHp;
                        }
                        var bgslider = bgSlider.GetComponent<Slider>();
                        if (bgslider != null)
                        {
                            bgslider.maxValue = maxHp;
                            bgslider.value = currentHp;
                        }
                    }

                    if (mpMaxText != null) mpMaxText.GetComponent<TextMeshProUGUI>().text = maxMP.ToString();
                    if (mpCurrentText != null) mpCurrentText.GetComponent<TextMeshProUGUI>().text = currentMP.ToString() + "/";
                    if (mpsliderComp != null)
                    {
                        var slider = mpsliderComp.GetComponent<Slider>();
                        if (slider != null)
                        {
                            slider.maxValue = maxMP;
                            slider.value = currentMP;
                        }
                        var mpbgSlider = mpBgSlider.GetComponent<Slider>();
                        if (mpbgSlider != null)
                        {
                            mpbgSlider.maxValue = maxMP;
                            mpbgSlider.value = currentMP;
                        }
                    }

                    if (expMaxText != null) expMaxText.GetComponent<TextMeshProUGUI>().text = maxExp.ToString();
                    if (expCurrentText != null) expCurrentText.GetComponent<TextMeshProUGUI>().text = currentExp.ToString() + "/";
                    if (expsliderComp != null)
                    {
                        var slider = expsliderComp.GetComponent<Slider>();
                        if (slider != null)
                        {
                            slider.maxValue = maxExp;
                            slider.value = currentExp;
                        }
                        var expbgSlider = expsliderComp.GetComponent<Slider>();
                        if (expbgSlider != null)
                        {
                            expbgSlider.maxValue = maxExp;
                            expbgSlider.value = currentExp;
                        }
                    }
                }

                character.RegistFloatAttributeChange(eEntityAttr.MaxHp_Float, eUnitAttributeEvent.Changed,
                (in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue) =>
                {
                    try
                    {
                        var hpLab = canvas.transform.Find("UIMain/HeadIcon/HPLab");
                        if (hpLab != null)
                        {
                            var slider = hpLab.Find("Slider")?.GetComponent<Slider>();
                            if (slider != null) slider.maxValue = newValue;
                            var maxText = hpLab.Find("Max")?.GetComponent<TextMeshProUGUI>();
                            if (maxText != null) maxText.text = Mathf.Ceil(newValue).ToString();
                            var bgSlider = hpLab.Find("Slider/BGSlider")?.GetComponent<Slider>();
                            if (bgSlider != null) bgSlider.maxValue = newValue;
                        }
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"Update main character max HP UI error: {ex.Message}");
                    }
                    return true;
                });
                character.RegistFloatAttributeChange(eEntityAttr.Hp_Float, eUnitAttributeEvent.Changed,
                (in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue) =>
                {
                    try
                    {
                        var hpLab = canvas.transform.Find("UIMain/HeadIcon/HPLab");
                        if (hpLab != null)
                        {
                            var slider = hpLab.Find("Slider")?.GetComponent<Slider>();
                            if (slider != null)
                            {
                                slider.value = newValue;
                                if (slider.value < 0)
                                {
                                    slider.value = 0;
                                }
                            }
                            var currentText = hpLab.Find("Current")?.GetComponent<TextMeshProUGUI>();
                            if (currentText != null) currentText.text = Mathf.Ceil(newValue).ToString();
                            if (currentHp < 0)
                            {
                                currentHp = 0;
                            }

                            var bgSlider = hpLab.Find("Slider/BGSlider").GetComponent<Slider>();
                            if (bgSlider.value < newValue)
                            {
                                bgSlider.value = newValue;
                                if (bgSlider.value < 0)
                                {
                                    bgSlider.value = 0;
                                }
                            }

                            StartSliderTween(bgSlider, newValue, AutoConst.HpGradient.BossHpGradient);
                        }
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"Update main character HP UI error: {ex.Message}");
                    }
                    return true;
                });

                character.RegistFloatAttributeChange(eEntityAttr.MaxMp_Float, eUnitAttributeEvent.Changed,
                (in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue) =>
                {
                    try
                    {
                        var mpLab = canvas.transform.Find("UIMain/HeadIcon/MPLab");
                        if (mpLab != null)
                        {
                            var slider = mpLab.Find("Slider")?.GetComponent<Slider>();
                            if (slider != null) slider.maxValue = newValue;
                            var maxText = mpLab.Find("Max")?.GetComponent<TextMeshProUGUI>();
                            if (maxText != null) maxText.text = Mathf.Ceil(newValue).ToString();
                            var bgSlider = mpLab.Find("Slider/BGSlider")?.GetComponent<Slider>();
                            if (bgSlider != null) bgSlider.maxValue = newValue;
                        }
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"Update main character max MP UI error: {ex.Message}");
                    }
                    return true;
                });
                character.RegistFloatAttributeChange(eEntityAttr.Mp_Float, eUnitAttributeEvent.Changed,
                (in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue) =>
                {
                    try
                    {
                        var mpLab = canvas.transform.Find("UIMain/HeadIcon/MPLab");
                        if (mpLab != null)
                        {
                            var slider = mpLab.Find("Slider")?.GetComponent<Slider>();
                            if (slider != null)
                            {
                                slider.value = newValue;
                                if (slider.value < 0)
                                {
                                    slider.value = 0;
                                }
                            }
                            var currentText = mpLab.Find("Current")?.GetComponent<TextMeshProUGUI>();
                            if (currentText != null) currentText.text = Mathf.Ceil(newValue).ToString();
                            if (currentMP < 0)
                            {
                                currentMP = 0;
                            }

                            var bgSlider = mpLab.Find("Slider/BGSlider").GetComponent<Slider>();
                            if (bgSlider.value < newValue)
                            {
                                bgSlider.value = newValue;
                                if (bgSlider.value < 0)
                                {
                                    bgSlider.value = 0;
                                }
                            }

                            StartSliderTween(bgSlider, newValue, AutoConst.HpGradient.BossHpGradient);
                        }
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"Update main character MP UI error: {ex.Message}");
                    }
                    return true;
                });

                character.RegistInt64AttributeChange(eCharacterAttr.Exp_Int64, eUnitAttributeEvent.Changed, OnCharacterExpChange);
                character.RegistInt64AttributeChange(eCharacterAttr.Level_Int64, eUnitAttributeEvent.Changed, OnCharacterLevelChange);

                Transform headIcon = canvas.transform.Find("UIMain/HeadIcon/CharacterImgBG/CharacterHead").gameObject.transform;
                IUnit unit = _CharacterManager.GetMainCharacterUnit();
                Int64 configid = unit.ConfigID;
                headIcon.GetComponent<Image>().sprite = Resources.Load<Sprite>(AutoConfig.Character.Get(configid)._Icon);
            }

            if (currentSceneConfig == null)
            {
                return topUIPanel;
            }

            character.RegistFloatAttributeChange(eEntityAttr.MaxHp_Float, eUnitAttributeEvent.Changed,
                    (in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue) =>
                    {
                        try
                        {
                            if (topUIPanel != null)
                            {
                                topUIPanel.transform.Find("BattleTopUI/HpBar").GetComponent<Slider>().maxValue = newValue;
                                var bgSlider = topUIPanel.transform.Find("BattleTopUI/HpBar/BGBar").GetComponent<Slider>();
                                bgSlider.maxValue = newValue;
                            }
                        }
                        catch (Exception ex)
                        {
                            XApi.Error($"Update character max HP UI error: {ex.Message}");
                        }
                        return true;
                    });

            character.RegistFloatAttributeChange(eEntityAttr.Hp_Float, eUnitAttributeEvent.Changed,
                  OnCharacterHpChanged);

            if (currentSceneConfig._SceneType == AutoEnum.eSceneType.BattleCopy || currentSceneConfig._SceneType == AutoEnum.eSceneType.PublicBattle)
            {
                topUIPanel.transform.Find("PublicUI").gameObject.SetActive(false);
                topUIPanel.transform.Find("BattleTopUI").gameObject.SetActive(true);

                if (character != _CharacterManager.GetMainCharacterUnit())
                {
                    bool isSameTeam = _TeamManager.IsSameTeam(character);
                    if (isSameTeam == true)
                    {
                        GameObject colorfillArea = topUIPanel.transform.Find("BattleTopUI/HpBar/Fill Area/Fill").gameObject;
                        float[] color = AutoConst.Partner.TeamHp;
                        colorfillArea.GetComponent<Image>().color = new Color(color[0], color[1], color[2], 1);
                    }
                    else
                    {
                        GameObject colorfillArea = topUIPanel.transform.Find("BattleTopUI/HpBar/Fill Area/Fill").gameObject;
                        float[] color = AutoConst.Partner.EnemyHp;
                        colorfillArea.GetComponent<Image>().color = new Color(color[0], color[1], color[2], 1);
                    }
                }
            }
            else
            {
                topUIPanel.transform.Find("PublicUI").gameObject.SetActive(true);
                topUIPanel.transform.Find("BattleTopUI").gameObject.SetActive(false);
                canvas.transform.Find("EndTime").gameObject.SetActive(false);
            }

            return topUIPanel;

        }

        catch (Exception ex)
        {
            XApi.Error($"CreateCharacterPanel error: {ex.Message}, Character GUID: {guid}, ConfigID: {character.ConfigID}");
            return null;
        }
    }
    private bool OnCharacterHpChanged(in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue)
    {
        try
        {
            var topUIPanel = _Panels[unit.Guid]?._Panel;
            if (topUIPanel != null)
            {
                topUIPanel.transform.Find("BattleTopUI/HpBar").GetComponent<Slider>().value = newValue;
            }

            var bgSlider = topUIPanel?.transform.Find("BattleTopUI/HpBar/BGBar").GetComponent<Slider>();
            if (bgSlider != null && bgSlider.value < newValue)
            {
                bgSlider.value = newValue;
            }

            if (bgSlider != null)
            {
                StartSliderTween(bgSlider, newValue, AutoConst.HpGradient.BossHpGradient);
            }
        }
        catch (Exception ex)
        {
            XApi.Error($"Update character HP UI error: {ex.Message}");
        }
        return true;
    }


    private void StartSliderTween(Slider bgSlider, float newValue, float v)
    {
        //Debug.LogError($"开始缓降血条,目标值:{newValue},缓动时间:{v}");
        if (bgSlider != null)
        {
            float currentValue = bgSlider.value;

            DG.Tweening.Tween tween = DG.Tweening.DOTween.To(() => bgSlider.value, x => bgSlider.value = x, newValue, v);
            tween.SetId(bgSlider);
        }
    }

    private GameObject CreateMonsterPanel(IUnit Monstercharacter, ulong guid)//怪物战斗场景topUI
    {
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        Transform canvasTransform = canvas != null ? canvas.transform : null;
        if (canvasTransform == null)
        {
            XApi.Error("找不到主Canvas，无法创建怪物头顶UI");
            return null;
        }
        Transform topUILab = canvasTransform.transform.Find("TopUILab");
        if (topUILab == null)
        {
            Debug.LogWarning("找不到TopUILab,按照原canvas生成");
        }

        GameObject topUIPre = Resources.Load<GameObject>(AutoConfig.AutoConst.UI.Monster);
        if (topUIPre == null)
        {
            XApi.Error("TopUI.prefab 加载失败，检查路径");
            return null;
        }

        Transform parent = topUILab != null ? topUILab : canvasTransform;
        GameObject topUIPanel = GameObject.Instantiate(topUIPre, parent);
        topUIPanel.name = "MonsterTopUI" + guid;
        topUIPanel.transform.SetSiblingIndex(0);
        _Panels[guid] = new PanelInfo(Monstercharacter, topUIPanel);

        topUIPanel.transform.Find("MonsterName").GetComponent<Text>().text = Monstercharacter.GetString(eMonsterAttr.Nickname_String);
        Text leveltext = topUIPanel.transform.Find("Level").GetComponent<Text>();
        leveltext.text = AutoConfig.MonsterQuality.Get(AutoConfig.Monster.Get(Monstercharacter.ConfigID)._MonsterQuality)._Monster;

        leveltext.color = new Color(AutoConfig.MonsterQuality.Get(AutoConfig.Monster.Get(Monstercharacter.ConfigID)._MonsterQuality)._Color[0], AutoConfig.MonsterQuality.Get(AutoConfig.Monster.Get(Monstercharacter.ConfigID)._MonsterQuality)._Color[1], AutoConfig.MonsterQuality.Get(AutoConfig.Monster.Get(Monstercharacter.ConfigID)._MonsterQuality)._Color[2]);

        //topUIPanel.transform.Find("HpBar").GetComponent<Slider>().maxValue = AutoConfig.Monster.Get(Monstercharacter.ConfigID)._Hp;
        topUIPanel.transform.Find("HpBar").GetComponent<Slider>().maxValue = Monstercharacter.GetFloat(eMonsterAttr.MaxHp_Float);
        topUIPanel.transform.Find("HpBar").GetComponent<Slider>().value = Monstercharacter.GetFloat(eMonsterAttr.Hp_Float);

        topUIPanel.transform.Find("HpBar/BGBar").GetComponent<Slider>().value = Monstercharacter.GetFloat(eMonsterAttr.Hp_Float);
        topUIPanel.transform.Find("HpBar/BGBar").GetComponent<Slider>().maxValue = Monstercharacter.GetFloat(eMonsterAttr.MaxHp_Float);
        //Debug.LogError($"怪物{guid} 最大生命值：{Monstercharacter.GetFloat(eMonsterAttr.MaxHp_Float)} 当前生命值：{Monstercharacter.GetFloat(eMonsterAttr.Hp_Float)}");
        bool isCantDamage = Monstercharacter.GetBool(eEntityAttr.IsCantDamaged_Bool);
        topUIPanel.SetActive(!isCantDamage);
        //Debug.LogError("面板创建，目前面板开关：" + !isCantDamage + "怪物名字" + Monstercharacter.GetString(eMonsterAttr.Nickname_String));
        // Monstercharacter.RegistBoolAttributeChange(eEntityAttr.IsCantDamaged_Bool, eUnitAttributeEvent.Changed,
        //           (in eUnitAttributeEvent ev, in IUnit unit, in int index, in bool oldValue, in bool newValue) =>
        //           {
        //               if (topUIPanel != null)
        //               {
        //                   topUIPanel.SetActive(!newValue);
        //                   //Debug.LogError(newValue);
        //                   //Debug.LogError("面板状态改变，目前面板开关：" + !newValue + "怪物名字" + Monstercharacter.GetString(eMonsterAttr.Nickname_String));
        //               }
        //               return true;
        //           });
        Monstercharacter.RegistFloatAttributeChange(eEntityAttr.MaxHp_Float, eUnitAttributeEvent.Changed,
                  (in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue) =>
                  {
                      if (topUIPanel != null)
                      {
                          topUIPanel.transform.Find("HpBar").GetComponent<Slider>().maxValue = newValue;
                      }

                      return true;
                  });
        Monstercharacter.RegistFloatAttributeChange(eEntityAttr.Hp_Float, eUnitAttributeEvent.Changed,
                  (in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue) =>
                  {
                      if (topUIPanel != null)
                      {
                          topUIPanel.transform.Find("HpBar").GetComponent<Slider>().value = newValue;
                      }

                      var bgSlider = topUIPanel.transform.Find("HpBar/BGBar").GetComponent<Slider>();
                      if (bgSlider.value < newValue)
                      {
                          bgSlider.value = newValue;
                      }
                      StartSliderTween(bgSlider, newValue, AutoConst.HpGradient.BossHpGradient);
                      return true;
                  });


        return topUIPanel;
    }

    private void CreateBossPanel(IUnit Bosscharacter, ulong guid)
    {

        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        Transform canvasTransform = canvas != null ? canvas.transform : null;
        if (canvasTransform == null)
        {
            XApi.Error("找不到Canvas");
            return;
        }
        Transform topUILab = canvasTransform.transform.Find("BossTopUILab");
        if (topUILab == null)
        {
            Debug.LogWarning("找不到BossTopUILab");
            return;
        }
        topUILab.gameObject.SetActive(true);
        topUILab.transform.Find("BossTopUI/Icon").GetComponent<Image>().sprite = Resources.Load<Sprite>(AutoConfig.MonsterBoss.Get(Bosscharacter.ConfigID)._BossHead);
        topUILab.transform.Find("BossTopUI/Name").GetComponent<TextMeshProUGUI>().text = AutoConfig.MonsterBoss.Get(Bosscharacter.ConfigID)._Name;
        Transform sliderTransform = topUILab.transform.Find("BossTopUI/Slider");
        Transform bgSliderTransform = topUILab.transform.Find("BossTopUI/BGSlider"); // 缓降2血条

        if (sliderTransform != null)
        {
            Slider slider = sliderTransform.GetComponent<Slider>();
            Slider bgSlider = bgSliderTransform.GetComponent<Slider>(); // 背景缓降血条
            if (slider != null)
            {
                float maxHp = Bosscharacter.GetFloat(eMonsterAttr.MaxHp_Float);
                float currentHp = Bosscharacter.GetFloat(eMonsterAttr.Hp_Float);

                slider.maxValue = maxHp;
                slider.value = currentHp;
                if (bgSlider != null)
                {
                    bgSlider.maxValue = maxHp;
                    bgSlider.value = currentHp;
                }
            }
            else
            {
                Debug.LogError("Slider组件不存在");
            }
        }
        else
        {
            Debug.LogError("Slider对象不存在");
        }
    }

    private void ReleaseBossPanel(IUnit unit, ulong guid)
    {
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        Transform canvasTransform = canvas != null ? canvas.transform : null;
        if (canvasTransform == null)
        {
            XApi.Error("找不到Canvas");
            return;
        }
        Transform topUILab = canvasTransform.transform.Find("BossTopUILab");
        if (topUILab == null)
        {
            Debug.LogWarning("找不到BossTopUILab");
            return;
        }
        topUILab.gameObject.SetActive(false);
    }

}
