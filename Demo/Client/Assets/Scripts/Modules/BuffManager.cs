using System;
using AutoConfig;
using GameUnit;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using XGameClient;
using XUtils;
using static GameUnitApi;


public interface IBuffManager : IModule
{

}
public class BuffManager : IBuffManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IBuffManager, BuffManager>();
    }
    IFiniteStateMachine _FiniteStateMachine;
    ICharacterManager _CharacterManager;
    IClient _Client;
    GameObject _CanvasBuffLab;
    IShowFXManager _ShowFXManager;
    ISpineAnimationController _SpineAnimationController;

    IGameObjectSync _GameObjectSync;

    class BuffInfo
    {
        public ulong buffGuid;
        public GameObject buffUI;
        public Int64 duration;
        public Int64 buffConfigId;
        public UInt64 hostGuid;
        public UInt64 createTimeTick;
    }
    private System.Collections.Generic.Dictionary<ulong, BuffInfo> activeBuffs = new System.Collections.Generic.Dictionary<ulong, BuffInfo>();

    public bool Initialize()
    {
        return true;
    }


    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _FiniteStateMachine = Launcher.GetModule<IFiniteStateMachine>();
        _FiniteStateMachine.RegistExitJudge(AutoConfig.AutoEnum.eState.Stuned, OnExitStunedJudge);
        _ShowFXManager = Launcher.GetModule<IShowFXManager>();
        _SpineAnimationController = Launcher.GetModule<ISpineAnimationController>();

        _Client.RegistProtobuf<oAddBuffPush>(eServerID.AddBuffPush, OnAddBuffPush);
        _Client.RegistProtobuf<oReleaseBuffPush>(eServerID.ReleaseBuffPush, OnReleaseBuffPush);
        _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, ClickMainUIOnMainCharacterEnter);
        // GameUnitApi.RegistUnitEvent();

        GameUnitApi.RegistUnitEvent(eUnitType.Entity, (in eUnitEvent ev, IUnit unit, in bool isParentType, in string file, in int line) =>
        {
            if (ev == eUnitEvent.Create)
            {
                unit.RegistInt32AttributeChange(eEntityAttr.CantControllerRef_Int32, eUnitAttributeEvent.Changed,
                (in eUnitAttributeEvent ev, in IUnit unit, in int index, in int oldValue, in int newValue) =>
                {
                    if (newValue <= 0)
                    {
                        int shiftRef = unit.GetInt32(eEntityAttr.ShiftRef_Int32);
                        if (shiftRef <= 0)
                        {
                            _SpineAnimationController.HandleAnimation(unit, "idle", true);
                        }

                    }
                    return true;
                });
            }
        });

        return true;
    }

    public void Release()
    {

    }
    void OnAddBuffPush(oAddBuffPush body)
    {
        Int64 buffconfigID = body.Buff.ConfigId;
        Int64 buffLifeTime = AutoConfig.Buff.Get(buffconfigID)._LifeCycle;
        //Debug.LogError($"收到Buff，名字{AutoConfig.Buff.Get(buffconfigID)._Name} 持续时间{buffLifeTime} 毫秒，开始倒计时 {SafeSystem.Time.GetMillisecondTick()}");
        UInt64 hostGuid = body.Buff.Attributes[eBuffAttr.HostGuid_UInt64].Value.UInt;
        IUnit unit = GameUnitApi.QueryUnit(hostGuid);//目标
        UInt64 createTimeTick = body.Buff.Attributes[eBuffAttr.CreateTimeTick_UInt64].Value.UInt;
        Int64 specialEffectID = AutoConfig.Buff.Get(buffconfigID)._SpecialEffects; // 使用Buff配置中的特效ID

        _ShowFXManager.ShowFX(unit.Guid, body.Buff.Guid, specialEffectID, "BuffFX", buffLifeTime);
        UInt64 mainGuid = _CharacterManager.GetMainCharacterUnit().Guid;

        BuffInfo buffInfo = new BuffInfo()
        {
            buffGuid = body.Buff.Guid,
            buffUI = null,
            duration = buffLifeTime,
            buffConfigId = buffconfigID,
            hostGuid = hostGuid,
            createTimeTick = createTimeTick,
        };
        activeBuffs[body.Buff.Guid] = buffInfo;

        if (hostGuid == mainGuid)
        {
            AddBuffIntoLab(buffInfo);
        }
        else
        {
            XApi.Debug($"buff对象非主角色，跳过");
        }

        bool isBuffact = AutoConfig.Buff.Get(buffconfigID)._IsBuffAct;
        if (isBuffact == true)
        {
            string test = AutoConfig.Buff.Get(buffconfigID)._BuffAct;
            bool test2 = AutoConfig.Buff.Get(buffconfigID)._actIsLoop;
            _SpineAnimationController.HandleAnimation(unit, AutoConfig.Buff.Get(buffconfigID)._BuffAct, AutoConfig.Buff.Get(buffconfigID)._actIsLoop);
            //播放技能动画
        }

        bool isNocontral = AutoConfig.Buff.Get(buffconfigID)._CanAct;
        if (isNocontral == false)
        {

        }
        bool isCamera = AutoConfig.Buff.Get(buffconfigID)._CanCamera;
        if (isCamera == true)
        {
            if (unit != null && unit.Guid == _CharacterManager.GetMainCharacterUnit().Guid)
            {
                //unit.RegistUpdate(UpdateCameraMoveIn);
            }
        }
    }

    void OnReleaseBuffPush(oReleaseBuffPush body)
    {
        if (activeBuffs.TryGetValue(body.BuffGuid, out BuffInfo buffInfo))
        {
            UInt64 hostGuid = buffInfo.hostGuid;
            IUnit unit = GameUnitApi.QueryUnit(hostGuid);
            Int64 buffconfigID = buffInfo.buffConfigId;
            IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
            if (unit != null)
            {
                if (unit.Guid == mainCharacter.Guid)
                {
                    _ShowFXManager.ClearFX(mainCharacter.Guid, body.BuffGuid, "BuffFX");
                    RemoveBuffFromLab(body.BuffGuid);
                }
                else
                {
                    _ShowFXManager.ClearFX(unit.Guid, body.BuffGuid, "BuffFX");
                }
            }
            activeBuffs.Remove(body.BuffGuid);
            bool isCamera = AutoConfig.Buff.Get(buffconfigID)._CanCamera;
            if (isCamera == true)
            {
                if (hostGuid == _CharacterManager.GetMainCharacterUnit().Guid)
                {
                    //unit.RegistUpdate(UpdateCameraMoveBack);
                }
            }
        }
    }


    void UpdateCameraMoveIn(IUnit unit)
    {
        Camera mainCamera = Camera.main;
        if (mainCamera != null && mainCamera.orthographic)
        {
            if (unit != null && unit.Guid == _CharacterManager.GetMainCharacterUnit().Guid)
            {
                // 找到所有有相机效果的buff，并获取最短的距离设置
                float closestCameraDistance = AutoConst.GameSetting.Camera; // 默认值
                bool hasCameraBuff = false;

                foreach (var buffEntry in activeBuffs)
                {
                    if (buffEntry.Value.hostGuid == unit.Guid)
                    {
                        Int64 buffConfigId = buffEntry.Value.buffConfigId;
                        bool isCamera = AutoConfig.Buff.Get(buffConfigId)._CanCamera;

                        if (isCamera)
                        {
                            hasCameraBuff = true;
                            float buffCameraDistance = AutoConfig.Buff.Get(buffConfigId)._CameraDisatance;
                            // 选择最小的距离（最靠近的相机）
                            if (buffCameraDistance < closestCameraDistance)
                            {
                                closestCameraDistance = buffCameraDistance;
                            }
                        }
                    }
                }

                if (hasCameraBuff)
                {
                    float defaultCameraSetting = AutoConst.GameSetting.Camera;

                    if (mainCamera.orthographicSize > closestCameraDistance)
                    {
                        // 计算所有相机buff中的最短时间作为过渡时间
                        float minTransitionTime = float.MaxValue;
                        foreach (var buffEntry in activeBuffs)
                        {
                            if (buffEntry.Value.hostGuid == unit.Guid)
                            {
                                Int64 buffConfigId = buffEntry.Value.buffConfigId;
                                bool isCamera = AutoConfig.Buff.Get(buffConfigId)._CanCamera;

                                if (isCamera)
                                {
                                    float buffTransitionTime = AutoConfig.Buff.Get(buffConfigId)._CameraTime / 1000.0f;
                                    if (buffTransitionTime < minTransitionTime)
                                    {
                                        minTransitionTime = buffTransitionTime;
                                    }
                                }
                            }
                        }

                        if (minTransitionTime == float.MaxValue) minTransitionTime = 0.5f; // 默认时间

                        float newSize = Mathf.MoveTowards(mainCamera.orthographicSize, closestCameraDistance,
                            Mathf.Abs(defaultCameraSetting - closestCameraDistance) * Time.deltaTime / minTransitionTime);
                        mainCamera.orthographicSize = newSize;
                        //Debug.LogError($"UpdateCameraMoveIn 相机移动中 {unit.Guid} {mainCamera.orthographicSize}");
                    }
                    else
                    {
                        // 达到目标大小，移除更新函数
                        unit.CancelRegistUpdate(UpdateCameraMoveIn);
                        //Debug.LogError($"UpdateCameraMoveIn 移除更新函数 {unit.Guid}");
                    }
                }
                else
                {
                    // 没有相机buff，移除更新函数
                    unit.CancelRegistUpdate(UpdateCameraMoveIn);
                }
            }
        }
    }

    void UpdateCameraMoveBack(IUnit unit)
    {
        Camera mainCamera = Camera.main;
        if (mainCamera != null && mainCamera.orthographic)
        {
            UInt64 characterGuid = _CharacterManager.GetMainCharacterUnit().Guid;
            if (unit != null && unit.Guid == characterGuid)
            {
                // 检查是否还有其他相机影响的buff
                float closestCameraDistance = AutoConst.GameSetting.Camera; // 默认值
                bool hasCameraBuff = false;

                foreach (var buffEntry in activeBuffs)
                {
                    if (buffEntry.Value.hostGuid == unit.Guid)
                    {
                        Int64 buffConfigId = buffEntry.Value.buffConfigId;
                        bool isCamera = AutoConfig.Buff.Get(buffConfigId)._CanCamera;

                        if (isCamera)
                        {
                            hasCameraBuff = true;
                            float buffCameraDistance = AutoConfig.Buff.Get(buffConfigId)._CameraDisatance;
                            // 选择最小的距离（最靠近的相机）
                            if (buffCameraDistance < closestCameraDistance)
                            {
                                closestCameraDistance = buffCameraDistance;
                            }
                        }
                    }
                }

                if (hasCameraBuff)
                {
                    float defaultCameraSetting = AutoConst.GameSetting.Camera;

                    if (Mathf.Abs(mainCamera.orthographicSize - closestCameraDistance) > 0.01f)
                    {
                        // 计算所有相机buff中的最短时间作为过渡时间
                        float minTransitionTime = float.MaxValue;
                        foreach (var buffEntry in activeBuffs)
                        {
                            if (buffEntry.Value.hostGuid == unit.Guid)
                            {
                                Int64 buffConfigId = buffEntry.Value.buffConfigId;
                                bool isCamera = AutoConfig.Buff.Get(buffConfigId)._CanCamera;

                                if (isCamera)
                                {
                                    float buffTransitionTime = AutoConfig.Buff.Get(buffConfigId)._CameraTime / 1000.0f;
                                    if (buffTransitionTime < minTransitionTime)
                                    {
                                        minTransitionTime = buffTransitionTime;
                                    }
                                }
                            }
                        }

                        if (minTransitionTime == float.MaxValue) minTransitionTime = 0.5f; // 默认时间

                        float newSize = Mathf.MoveTowards(mainCamera.orthographicSize, closestCameraDistance,
                            Mathf.Abs(defaultCameraSetting - closestCameraDistance) * Time.deltaTime / minTransitionTime);
                        mainCamera.orthographicSize = newSize;
                        //Debug.LogError($"UpdateCameraMoveBack 相机移动中 {unit.Guid} {mainCamera.orthographicSize}, 目标: {closestCameraDistance}");
                    }
                }
                else
                {
                    // 没有相机buff了，还原到默认大小
                    float defaultCameraSetting = AutoConst.GameSetting.Camera;

                    if (Mathf.Abs(mainCamera.orthographicSize - defaultCameraSetting) > 0.01f)
                    {
                        float transitionTime = 0.5f; // 默认时间

                        foreach (var buffEntry in activeBuffs)
                        {
                            if (buffEntry.Value.hostGuid == unit.Guid)
                            {
                                Int64 buffConfigId = buffEntry.Value.buffConfigId;
                                bool isCamera = AutoConfig.Buff.Get(buffConfigId)._CanCamera;

                                if (isCamera)
                                {
                                    transitionTime = AutoConfig.Buff.Get(buffConfigId)._CameraRecover / 1000.0f;
                                    break;
                                }
                            }
                        }

                        float newSize = Mathf.MoveTowards(mainCamera.orthographicSize, defaultCameraSetting,
                            Mathf.Abs(defaultCameraSetting - mainCamera.orthographicSize) * Time.deltaTime / transitionTime);
                        mainCamera.orthographicSize = newSize;
                        //Debug.LogError($"UpdateCameraMoveBack 相机还原中 {unit.Guid} {mainCamera.orthographicSize}, 目标: {defaultCameraSetting}");
                    }
                    else
                    {
                        // 已经还原到默认大小，移除更新函数
                        unit.CancelRegistUpdate(UpdateCameraMoveBack);
                        //Debug.LogError($"UpdateCameraMoveBack 移除更新函数 {unit.Guid}");
                    }
                }
            }
        }
    }

    void ClickMainUIOnMainCharacterEnter(IUnit character)
    {
        _CanvasBuffLab = GameObject.FindFirstObjectByType<Canvas>().transform.gameObject;
        GameObject buffLabs = _CanvasBuffLab.transform.Find("UIMain/HeadIcon/BuffLab/Viewport/Content").gameObject;
        for (int i = buffLabs.transform.childCount - 1; i >= 0; i--)
        {
            GameObject.Destroy(buffLabs.transform.GetChild(i).gameObject);
        }
    }

    void AddBuffIntoLab(BuffInfo buffInfo)
    {
        GameObject BuffItem = Resources.Load<GameObject>(AutoConst.BuffRevelant.BuffPrefab);
        GameObject buffUI = GameObject.Instantiate(BuffItem, _CanvasBuffLab.transform.Find("UIMain/HeadIcon/BuffLab/Viewport/Content"));
        buffUI.transform.GetComponent<Image>().sprite = Resources.Load<Sprite>(AutoConfig.Buff.Get(buffInfo.buffConfigId)._Material);

        buffInfo.buffUI = buffUI;
        activeBuffs[buffInfo.buffGuid] = buffInfo; // 更新字典中的引用

        Image mask = buffUI.transform.Find("UseMask").GetComponent<Image>();
        {
            if (mask != null)
            {
                if (buffInfo.duration > 0)
                {
                    mask.fillAmount = 1f;
                    XApi.TimerApi.StartTimer(OnBuffMaskUpdate, buffInfo.buffGuid, null, 0, 0, 50);
                }
                else if (buffInfo.duration <= 0)
                {
                    mask.fillAmount = 0f;
                }
            }
        }

        if (buffInfo.duration > 0)
        {
            XApi.TimerApi.StartTimer(OnBuffTimeout, buffInfo.buffGuid, 0, (int)buffInfo.duration, 1, 1, "");
        }
    }

    void OnBuffMaskUpdate(in XApi.ITimer.eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
    {
        ulong buffGuid = (ulong)obj;

        if (state == XApi.ITimer.eState.BEAT && activeBuffs.TryGetValue(buffGuid, out BuffInfo buffInfo))
        {
            Image maskImg = buffInfo.buffUI?.transform.Find("UseMask").GetComponent<Image>();
            if (maskImg != null)
            {
                float elapsed = (float)count * 50;
                float progress = Mathf.Clamp01(elapsed / (float)buffInfo.duration);
                maskImg.fillAmount = 1f - progress;
            }
        }
    }
    void OnBuffTimeout(in XApi.ITimer.eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
    {
        ulong buffGuid = (ulong)obj;
        RemoveBuffFromLab(buffGuid);
    }

    void RemoveBuffFromLab(in ulong buffGuid)
    {
        if (activeBuffs.TryGetValue(buffGuid, out BuffInfo buffInfo))
        {
            Debug.LogWarning($"Buff结束,名字{AutoConfig.Buff.Get(buffInfo.buffConfigId)._Name} ,倒计时结束 {SafeSystem.Time.GetMillisecondTick()}");
            _ShowFXManager.ClearFX(buffInfo.hostGuid, buffGuid, "BuffFX");


            XApi.TimerApi.StopTimer(OnBuffTimeout, buffGuid);
            XApi.TimerApi.StopTimer(OnBuffMaskUpdate, buffGuid);

            if (buffInfo.buffUI != null)
            {
                GameObject.Destroy(buffInfo.buffUI);
            }
            activeBuffs.Remove(buffGuid);
        }
    }

    bool OnExitStunedJudge(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            return false;
        }
        int cantcontrolref = unit.GetInt32(eCharacterAttr.CantControllerRef_Int32);

        return cantcontrolref == 0;
    }
}
