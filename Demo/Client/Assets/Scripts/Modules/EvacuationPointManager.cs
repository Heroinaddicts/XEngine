using System;
using UnityEngine;
using XGame;
using UnityEngine.UI;
using GameUnit;
using XEngine;
using XUtils;
using static GameUnitApi;
using DG.Tweening;
using TMPro;
using Spine.Unity;

namespace XGameClient
{
    public interface IEvacuationPointManager : IModule
    {
        public void HandleEvacuationPointEnter(IUnit unit, AutoConfig.TransPort config);
        public void HandleEvacuationPointExit(IUnit unit, AutoConfig.TransPort config);
    }

    class EvacuationPointManager : IEvacuationPointManager
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IEvacuationPointManager, EvacuationPointManager>();
        }

        ICharacterManager _CharacterManager;
        IClient _Client;
        IFiniteStateMachine _FSM;
        GameObject _EvacuationPage;

        private Int64 _CountdownStartTime;
        private bool _IsCountingDown = false;
        private Int64 _CountdownDuration;
        private AutoConfig.TransPort _CurrentTransPortConfig;

        XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _FSM = Launcher.GetModule<IFiniteStateMachine>();

            _Client = Launcher.GetModule<IClient>();

            return true;
        }

        public void Update()
        {
            
            if (_IsCountingDown && _EvacuationPage != null)
            {
                IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
                if(mainCharacter == null)
                {
                    return;
                }

                if(_FSM.GetState(mainCharacter.Guid) == AutoConfig.AutoEnum.eState.Death)
                {
                    Debug.LogError("主角死亡，停止倒计时");
                    _IsCountingDown = false;
                    HideEvacuationPage();
                    return;
                }

                Int64 elapsed = SafeSystem.Time.GetMillisecondTick() - _CountdownStartTime;//已用时
                Int64 remaining = _CountdownDuration - elapsed;//计算剩余时间
                //Debug.LogError($"已用时 {elapsed / 1000f:F2} 秒，剩余时间 {remaining / 1000f:F2} 秒，开始时间 {_CountdownStartTime / 1000f:F2} 秒");

                TextMeshProUGUI countdownText = _EvacuationPage.transform.Find("Num").GetComponent<TextMeshProUGUI>();
                if (countdownText != null)
                {
                    if (remaining > 0)
                    {
                        int seconds = Mathf.CeilToInt(remaining / 1000f);
                        countdownText.text = $"{seconds}";
                        //Debug.LogError($"倒计时 {seconds} 秒，剩余时间 {remaining / 1000f:F2} 秒");
                    }
                    else
                    {
                        countdownText.text = "0";
                        ExecuteEvacuation();
                        _IsCountingDown = false;
                        Debug.LogError("倒计时结束，执行撤离");
                    }
                }
            }
        }

        public void Release()
        {

        }

        public void HandleEvacuationPointEnter(IUnit unit, AutoConfig.TransPort config)
        {
            if (config._TransPortType != AutoConfig.AutoEnum.eTransPortType.Normal)
            {
                _CurrentTransPortConfig = config;
                ShowEvacuationPage(config);
            }
        }

        public void HandleEvacuationPointExit(IUnit unit, AutoConfig.TransPort config)
        {
            if (config._TransPortType != AutoConfig.AutoEnum.eTransPortType.Normal)
            {
                _IsCountingDown = false;
                _CurrentTransPortConfig = null;
                HideEvacuationPage();
            }
        }

        private void ShowEvacuationPage(AutoConfig.TransPort config)
        {
            Debug.LogError("ShowEvacuationPage");
            if (_EvacuationPage != null)
            {
                _EvacuationPage.transform.DOKill();

                _EvacuationPage.SetActive(true);
            }
            else
            {
                Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                Transform evacuationPoint = canvas.transform.Find("TeamPoint/MatchPoint");

                GameObject evacuationPagePrefab = Resources.Load<GameObject>("UI/Prefabs/Battle/EvacuationPage");
                GameObject evacuationPageGO = GameObject.Instantiate(evacuationPagePrefab, evacuationPoint);
                _EvacuationPage = evacuationPageGO;

                _EvacuationPage.transform.SetParent(evacuationPoint, false);
                
            }
            _EvacuationPage.transform.localPosition = Vector3.zero;
            Vector3 startPosition = _EvacuationPage.transform.position;
            float screenHeightOffset = Screen.height * 0.2f;
            Vector3 endPosition = startPosition + new Vector3(0, -screenHeightOffset, 0);
            _EvacuationPage.transform.DOMove(endPosition, 0.5f);

            TextMeshProUGUI countdownText = _EvacuationPage.transform.Find("Num").GetComponent<TextMeshProUGUI>();
            if (countdownText != null)
            {
                _CountdownDuration = config._EffecTime;
                _CountdownStartTime = SafeSystem.Time.GetMillisecondTick();
                _IsCountingDown = true;
            }
        }

        private void HideEvacuationPage()
        {
            _IsCountingDown = false;
            _CurrentTransPortConfig = null;

            if (_EvacuationPage != null)
            {
                _EvacuationPage.transform.DOKill();
                Vector3 startPosition = _EvacuationPage.transform.position;
                Debug.LogError("开始EvacuationPage移动动画");
                _EvacuationPage.transform.DOMove(startPosition, 0.5f).OnComplete(() =>
                {
                    Debug.LogError("HideEvacuationPage移动动画完成");
                    _EvacuationPage.SetActive(false);
                });
            }
        }

        private void ExecuteEvacuation()
        {
            if (_CurrentTransPortConfig != null)
            {
                oExitCopyScene o = new oExitCopyScene();
                _Client.SendProtobuf(eClientID.ExitCopyScene, o);
            }

            HideEvacuationPage();
        }

    }
}