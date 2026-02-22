using System;
using AutoConfig;
using GameUnit;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;
using static GameUnitApi;

public interface IUIMain : IModule
{
    enum eEvent
    {
        OpenCharacterBag,
        OpenTask,
        OpenMiniTask,
        OpenCharacterCenter,
        OpenCharacterInfo,
    }

    void RegistEvent(eEvent eventID, Action callback);
    void UnRegistEvent(eEvent eventID, Action callback);

}

class UIMain : IUIMain
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IUIMain, UIMain>();
    }

    GameObject _UIMain;
    IUnit _MainCharacterUnit;
    ICharacterManager _CharacterManager;
    IGameSceneManager _GameSceneManager;

    IWeaponManager _WeaponManager;
    ITaskManager _TaskManager;
    IClient _Client;

    private TextMeshProUGUI _BattleTimeText;//战斗倒计时文本
    private Int64 _CountdownEndTime; // 时间戳结束时间
    private Int64 _CountdownDuration;//倒计时总时长
    private bool _IsCountdownActive = false;//倒计时是否激活

    XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _WeaponManager = Launcher.GetModule<IWeaponManager>();
        _TaskManager = Launcher.GetModule<ITaskManager>();
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
        _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, OnMainCharacterEnter);
        _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.GameMapLoaded, IntoTimeStart);
        _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, OnBattleStarTime);

        _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.GameMapLoaded, OnSceneLoaded);
        
        return true;
    }

    private void OnSceneLoaded(UnityEngine.SceneManagement.Scene scene)
    {
        Int64 sceneConfigID = _GameSceneManager.GetCurrentSceneConfig()._ID;
        if(AutoConfig.Scene.Get(sceneConfigID)._SceneType != AutoEnum.eSceneType.BattleCopy)
        {
            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
            Transform transform = canvas.transform.Find("EndTime");
            if(transform != null)
            {
                transform.gameObject.SetActive(false);
            }
        }
    }

    private void IntoTimeStart(UnityEngine.SceneManagement.Scene scene)
    {
        IUnit unit = _CharacterManager.GetMainCharacterUnit();
        if(unit == null)
        {
            return;
        }
        else
        {
            OnBattleStarTime(unit);
        }
        
    }

    public void OnMainCharacterEnter(IUnit unit)
    {
        if (unit != null)
        {
            _MainCharacterUnit = unit;
        }

        _UIMain = GameObject.Find("UIMain");
        InitMainUI();
    }

    private void InitMainUI()
    {
        ListenerBtnClickEvent();
    }

    private void ListenerBtnClickEvent()
    {
        _UIMain.transform.Find("TopBtnPanel/BagBtn").GetComponent<Button>().onClick.AddListener(() =>
        {
            _Events.Trigger(IUIMain.eEvent.OpenCharacterBag);//打开背包
        });

        _UIMain.transform.Find("TopBtnPanel/CharacterBtn").GetComponent<Button>().onClick.AddListener(() =>
        {
            _Events.Trigger(IUIMain.eEvent.OpenCharacterInfo);//打开角色
        });

        _UIMain.transform.Find("LeftBtnPanel/TaskIcon").GetComponent<Button>().onClick.AddListener(() =>
        {
            _Events.Trigger(IUIMain.eEvent.OpenTask);//打开任务
        });

        _UIMain.transform.Find("TopBtnPanel/CharacterCenterBtn").GetComponent<Button>().onClick.AddListener(() =>
        {
            _Events.Trigger(IUIMain.eEvent.OpenCharacterCenter);
        });

        GameObject game = _UIMain.transform.Find("TaskFrame").gameObject;
        game.GetComponent<Button>().onClick.AddListener(() =>
        {
            _Events.Trigger(IUIMain.eEvent.OpenMiniTask);//打开任务
        });

        _UIMain.transform.Find("Button").GetComponent<Button>().onClick.AddListener(() =>
        {
            _TaskManager.CompleteTask((Int64)10002);
        });


    }

    public void RegistEvent(IUIMain.eEvent eventID, Action callback)
    {
        _Events.Regist(eventID, callback);
    }

    public void Release()
    {

    }

    public void Update()
    {
        if(_IsCountdownActive && _BattleTimeText != null)
        {
            Int64 currentTime = SafeSystem.Time.GetMillisecondTick();
            Int64 remainingTimeMs = _CountdownEndTime - currentTime;
            if (remainingTimeMs <= 0)
            {
                remainingTimeMs = 0;
                _IsCountdownActive = false;
            }

            int totalSeconds = (int)(remainingTimeMs / 1000);
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;
            _BattleTimeText.text = string.Format("{0:00}:{1:00}", minutes, seconds);
            //Debug.LogError($"战斗倒计时更新，剩余时间: {minutes:00}:{seconds:00}，总时长: {_CountdownDuration}毫秒，当前时间: {currentTime}，结束时间: {_CountdownEndTime}，剩余时间毫秒: {remainingTimeMs}");
        }
    }

    public void UnRegistEvent(IUIMain.eEvent eventID, Action callback)
    {
        _Events.CancelRegist(eventID, callback);
    }


    public void OnBattleStarTime(IUnit unit)//战斗倒计时启动
    {
        //Debug.LogError("战斗倒计时启动");
        Int64 sceneConfigID = _GameSceneManager.GetCurrentSceneConfig()._ID;
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        var battleTime = canvas.transform.Find("EndTime");
        if(AutoConfig.AutoEnum.eSceneType.Public == AutoConfig.Scene.Get(sceneConfigID)._SceneType && AutoConfig.AutoEnum.eSceneType.PublicBattle == AutoConfig.Scene.Get(sceneConfigID)._SceneType)
        {
            XApi.Debug("非战斗副本，不显示倒计时");
            battleTime.gameObject.SetActive(true);
            return;
        }
        battleTime.gameObject.SetActive(true);
        _BattleTimeText = battleTime.GetComponent<TextMeshProUGUI>();
        //IUnit unit = _CharacterManager.GetMainCharacterUnit();
        if(unit == null)
        {
            return;
        }
        Int64 battleStartTime = (Int64)unit.GetUInt64(eEntityAttr.BattleStartTime_UInt64);//战斗开始时间
        if(battleStartTime == 0)
        {
            XApi.Debug("战斗开始时间为0，不显示倒计时");
            battleTime.gameObject.SetActive(false);
            return;
        }
        Int64 mapdisappearTime = AutoConfig.Scene.Get(sceneConfigID)._DisappearTime;//倒计时时间
        _CountdownDuration = mapdisappearTime;
        _CountdownEndTime = battleStartTime + _CountdownDuration;
        //Debug.LogError($"战斗倒计时启动，场景ID: {sceneConfigID}，战斗开始时间: {battleStartTime}，倒计时总时长: {mapdisappearTime}毫秒，结束时间: {_CountdownEndTime}");
        _IsCountdownActive = true;
    }
}
