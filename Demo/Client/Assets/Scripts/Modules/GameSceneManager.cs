using System;
using System.Runtime.CompilerServices;
using UnityEngine;
using UnityEngine.SceneManagement;
using XEngine;
using XGame;
using XGameClient;
using XUtils;
using static GameUnitApi;

public interface IGameSceneManager : IModule
{
    public enum eEvent
    {
        SceneLoaded = 0,
        GameMapLoaded = 1
    }

    public delegate void OnSceneLoaded(Scene scene);
    void RegistEvent(eEvent e, in OnSceneLoaded callback, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0, [CallerMemberName] string member = "");

    void AsyncLoadScene(string sceneName, Action callback = null);

    public AutoConfig.Scene GetCurrentSceneConfig();

    IGameTilemap GetCurrentTilemap();
}

class GameSceneManager : IGameSceneManager
{

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IGameSceneManager, GameSceneManager>();
    }

    IClient _Client;
    Int64 _CurPublicSceneConfigID;
    GameObject _CurPublicScene;

    AutoConfig.Scene _CurrentSceneConfig = null;
    IMatchManager _MatchManager;
    GameObject _TeleportPoints;
    IUIMain _UIMain;
    IEvacuationPointManager _EvacuationPointManager;
    XEventPool<IGameSceneManager.eEvent> _EventPool = new XEventPool<IGameSceneManager.eEvent>();
    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _Client.RegistProtobuf<oSceneInfoPush>(eServerID.SceneInfoPush, OnSceneInfoPush);
        _MatchManager = Launcher.GetModule<IMatchManager>();
        _EvacuationPointManager = Launcher.GetModule<IEvacuationPointManager>();
        _UIMain = Launcher.GetModule<IUIMain>();

        _TeleportPoints = new GameObject("TeleportPoints");
        GameObject.DontDestroyOnLoad(_TeleportPoints);

        return true;
    }

    public void Release()
    {

    }

    public IGameTilemap GetCurrentTilemap()
    {
        if (_CurPublicScene == null)
        {
            return null;
        }

        return _CurPublicScene.GetComponent<GameTilemap>();
    }

    void OnLeaveTimer(in XEngine.XApi.ITimer.eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
    {
        if (state == XApi.ITimer.eState.BEAT)
        {
            oExitCopyScene o = new oExitCopyScene();
            _Client.SendProtobuf(eClientID.ExitCopyScene, o);
        }
    }

    void OnSceneInfoPush(oSceneInfoPush push)
    {
        if (_CurPublicScene != null)
        {
            GameObject.Destroy(_CurPublicScene);
            _CurPublicScene = null;
        }

        XApi.Debug($"OnSceneInfoPush Scene ConfigID {push.Info.ConfigId} Name {AutoConfig.Scene.Get(push.Info.ConfigId)._Name}");
        _CurPublicSceneConfigID = push.Info.ConfigId;
        _CurrentSceneConfig = AutoConfig.Scene.Get(_CurPublicSceneConfigID);
        GameObject go = Resources.Load<GameObject>($"Tilemap/Prefabs/{_CurrentSceneConfig._Preload}");
        GameObject publicScene = GameObject.Instantiate(go);
        publicScene.transform.parent = GameObject.Find("PublicScene").transform;
        publicScene.transform.localScale = Vector3.one;
        publicScene.transform.localPosition = Vector3.zero;
        _CurPublicScene = publicScene;

        GameObjectCommon.ClearAllChild(_TeleportPoints);

        for (int i = 0; i < _CurrentSceneConfig._TransPort.Length; i++)
        {
            AutoConfig.TransPort config = AutoConfig.TransPort.Get(_CurrentSceneConfig._TransPort[i]);
            if (config != null)
            {

                GameObject teleportPoint = GameObject.Instantiate(Resources.Load<GameObject>(config._Effect));
                teleportPoint.transform.parent = _TeleportPoints.transform;
                teleportPoint.transform.position = new Vector3(config._Position[0], config._Position[1], 0);

                Trigger2D tp = teleportPoint.AddComponent<Trigger2D>();
                switch (config._TransPortType)
                {
                    case AutoConfig.AutoEnum.eTransPortType.Normal:
                        tp.RegistTriggerCallback(
                         (Collider2D other) =>
                         {
                             IUnit unit = other.GetComponent<IUnit>();
                             if (unit != null && unit.Guid == Launcher.GetModule<ICharacterManager>().GetMainCharacterUnit().Guid)
                             {
                                 _MatchManager.StartMatch(unit);
                             }
                         },
                         exit: null,
                         stay: null
                         );
                        break;
                    case AutoConfig.AutoEnum.eTransPortType.Evacuate:
                        Debug.LogError("Evacuate进入");
                        tp.RegistTriggerCallback(
                          (Collider2D other) =>
                          {
                              IUnit unit = other.GetComponent<IUnit>();
                              if (unit != null && unit.Guid == Launcher.GetModule<ICharacterManager>().GetMainCharacterUnit().Guid)
                              {
                                  // _EvacuationPointManager.EnterEvacuationPoint(unit);
                                  // XApi.TimerApi.StartTimer(OnLeaveTimer, unit, unit, (int)config._EffecTime, 1, (int)config._EffecTime);
                                  _EvacuationPointManager.HandleEvacuationPointEnter(unit, config);

                              }
                          },
                          (Collider2D other) =>
                          {
                              IUnit unit = other.GetComponent<IUnit>();
                              if (unit != null && unit.Guid == Launcher.GetModule<ICharacterManager>().GetMainCharacterUnit().Guid)
                              {
                                //   _EvacuationPointManager.ExitEvacuationPoint(unit);
                                //   XApi.TimerApi.StopTimer(OnLeaveTimer, unit);
                                 _EvacuationPointManager.HandleEvacuationPointExit(unit, config);
                              }
                          },
                          stay: null
                          );
                        break;
                }
            }
        }
        _EventPool.Trigger(IGameSceneManager.eEvent.GameMapLoaded, SceneManager.GetActiveScene());
    }

    private AsyncOperation _CurrentOperation = null;
    public void AsyncLoadScene(string sceneName, Action callback)
    {
        if (_CurrentOperation != null)
        {
            XApi.Error($"[GameSceneManager] 正在加载场景中，无法再次加载: {_CurrentOperation?.progress:P0}");
            return;
        }

        XApi.Trace($"[GameSceneManager] 开始加载场景: {sceneName}");
        _CurrentOperation = SceneManager.LoadSceneAsync(sceneName, LoadSceneMode.Single);
        if (null == _CurrentOperation)
        {
            XApi.Error($"[GameSceneManager] 无法加载场景: {sceneName}");
            return;
        }
        XApi.NetworkApi.Pause();
        _CurrentOperation.completed += (op) =>
        {
            // 确保是当前加载的场景操作
            if (op == _CurrentOperation)
            {
                _CurrentOperation = null;
                XApi.Trace($"[GameSceneManager] 场景加载完成: {sceneName}");
                _EventPool.Trigger(IGameSceneManager.eEvent.SceneLoaded, SceneManager.GetActiveScene());
                callback?.Invoke();
                XApi.NetworkApi.Resume();
            }
        };
    }

    public void RegistEvent(IGameSceneManager.eEvent e, in IGameSceneManager.OnSceneLoaded callback, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0, [CallerMemberName] string member = "")
    {
        _EventPool.Regist(e, callback, file, line, member);
    }

    public AutoConfig.Scene GetCurrentSceneConfig()
    {
        return _CurrentSceneConfig;
    }
}
