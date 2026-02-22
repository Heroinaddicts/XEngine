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
using AutoConfig;


namespace XGameClient
{
    public interface IMatchManager : IModule
    {
        public void StartMatch(IUnit unit);

    }
    class MatchManager : IMatchManager
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IMatchManager, MatchManager>();
        }

        ICharacterManager _CharacterManager;
        IGameSceneManager _GameSceneManager;

        IClient _Client;

        GameObject _MatchPoint;
        GameObject _MatchUI;
        GameObject _SuccessPage;
        private Int64 _CountdownStartTime;
        private bool _IsCountingDown = false;
        private Int64 _CountdownDuration;

        XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
            _Client = Launcher.GetModule<IClient>();

            _Client.RegistProtobuf<oMatchApplyReqPush>(eServerID.MatchApplyReqPush, OnMathApplyReqPush);
            _Client.RegistProtobuf<oMatchCancleReqPush>(eServerID.MatchCancleReqPush, OnMatchCancleReqPush);
            _Client.RegistProtobuf<oMatchSuccessPush>(eServerID.MatchSuccessPush, OnMatchSuccessPush);

            _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.SceneLoaded, (UnityEngine.SceneManagement.Scene scene) =>
            {
                if (scene.name == "GameScene")
                {
                    _MatchPoint = GameObjectCommon.GetGameObjectByPath("Canvas/TeamPoint/MatchPoint");
                    GameObject matchPage = Resources.Load<GameObject>("UI/Prefabs/Team/MatchPage");
                    _MatchUI = GameObject.Instantiate(matchPage, _MatchPoint.transform);
                }
            });
            return true;
        }

        public void Release()
        {

        }
        public void Update()
        {
            if (_IsCountingDown && _SuccessPage != null)
            {
                Int64 elapsed = SafeSystem.Time.GetMillisecondTick() - _CountdownStartTime;
                Int64 remaining = _CountdownDuration - elapsed;
                if (remaining > 0)
                {
                    int seconds = Mathf.CeilToInt(remaining / 1000f);
                    string timeText = $"{seconds}";

                    TextMeshProUGUI countdownText = _SuccessPage.transform.Find("Num").GetComponent<TextMeshProUGUI>();
                    if (countdownText != null)
                    {
                        countdownText.text = timeText;
                    }
                }
                else
                {
                    EndCountDown();
                }
            }
        }



        public void StartMatch(IUnit unit)//开始匹配
        {
            if (unit == null)
            {
                return;
            }
            if (unit.GetUInt64(eCharacterAttr.TeamGuid_UInt64) != 0)//有队伍
            {
                bool isteamLeader = unit.GetBool(eCharacterAttr.IsTeamLeader_Bool);
                if (!isteamLeader)
                {
                    Debug.LogError("队长才能匹配");
                    //TODO: 提示队伍队长才能开始匹配
                    return;
                }
                else
                {
                    Debug.LogError("有队伍开始匹配");
                    oMatchApplyReq req = new oMatchApplyReq();
                    req.SceneId = 20002;
                    _Client.SendProtobuf(eClientID.MatchApplyReq, req);
                }
            }
            else
            {
                //Debug.LogError("无队伍单人开始匹配");
                //无队伍单人匹配
                oMatchApplyReq req = new oMatchApplyReq();
                req.SceneId = 20002;
                _Client.SendProtobuf(eClientID.MatchApplyReq, req);
            }
        }

        private void OnMathApplyReqPush(oMatchApplyReqPush body)//服务器开始匹配的回应推送
        {
            //Debug.LogError($"开始匹配");
            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
            if (canvas == null)
            {
                return;
            }

            XApi.TimerApi.StopTimer(MatchTime, _MatchUI);
            TextMeshProUGUI timeDisplay = _MatchUI.transform.Find("Num").GetComponent<TextMeshProUGUI>();
            if (timeDisplay != null)
            {
                timeDisplay.text = "00:00";
            }
            _MatchUI.SetActive(true);

            _MatchUI.transform.localPosition = Vector3.zero;
            Vector3 startPosition = _MatchUI.transform.position;
            float screenHeightOffset = Screen.height * 0.2f;
            Vector3 endPosition = startPosition + new Vector3(0, -screenHeightOffset, 0);

            XApi.TimerApi.StartTimer(MatchTime, _MatchUI, null, 1000, XApi.ITimer.Unlimited, 1000);
            _MatchUI.transform.Find("No").GetComponent<Button>().onClick.RemoveAllListeners();
            _MatchUI.transform.Find("No").GetComponent<Button>().onClick.AddListener(() =>
            {
                oMatchCancleReq cancleReq = new oMatchCancleReq();
                _Client.SendProtobuf(eClientID.MatchCancleReq, cancleReq);//发送取消匹配请求
            });
            _MatchUI.transform.DOMove(endPosition, 0.5f);
            canvas.transform.Find("ExitTeam").gameObject.SetActive(false);
        }

        private void MatchTime(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
        {
            GameObject matchPageGO = (GameObject)obj;
            if (state == XApi.ITimer.eState.BEAT)
            {
                int passTime = count;
                int minutes = passTime / 60;
                int seconds = passTime % 60;
                string timeText = $"{minutes:D2}:{seconds:D2}";
                TextMeshProUGUI timeDisplay = matchPageGO.transform.Find("Num").GetComponent<TextMeshProUGUI>();
                if (timeDisplay != null)
                {
                    timeDisplay.text = timeText;
                }
            }
        }

        private void OnMatchCancleReqPush(oMatchCancleReqPush body)//服务器取消匹配的回应推送
        {
            Debug.LogError($"取消匹配");
            if (_MatchUI == null)
            {
                return;
            }
            XApi.TimerApi.StopTimer(MatchTime, _MatchUI);
            TextMeshProUGUI timeDisplay = _MatchUI.transform.Find("Num").GetComponent<TextMeshProUGUI>();
            if (timeDisplay != null)
            {
                timeDisplay.text = "00:00";
            }
            _MatchUI.transform.DOMove(_MatchUI.transform.position, 0.5f).OnComplete(() =>
            {
                _MatchUI.SetActive(false);
            });
            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
            IUnit character = _CharacterManager.GetMainCharacterUnit();
            if (character.GetUInt64(eCharacterAttr.TeamGuid_UInt64) != 0)
            {
                canvas.transform.Find("ExitTeam").gameObject.SetActive(true);
            }

        }

        private void OnMatchSuccessPush(oMatchSuccessPush body)//服务器匹配成功的回应推送
        {
            //Debug.LogError($"匹配成功");
            if (_MatchUI == null)
            {
                return;
            }
            XApi.TimerApi.StopTimer(MatchTime, _MatchUI);
            TextMeshProUGUI timeDisplay = _MatchUI.transform.Find("Num").GetComponent<TextMeshProUGUI>();
            if (timeDisplay != null)
            {
                timeDisplay.text = "00:00";
            }

            _MatchUI.transform.DOMove(_MatchUI.transform.position, 0.5f).OnComplete(() =>
            {
                _MatchUI.SetActive(false);
            });
            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
            Transform matchsuccesspoint = canvas.transform.Find("TeamPoint/MatchPoint");
            if (_SuccessPage == null)
            {
                GameObject successPage = Resources.Load<GameObject>("UI/Prefabs/Team/MatchSuccess");
                GameObject successPageGO = GameObject.Instantiate(successPage, matchsuccesspoint);
                _SuccessPage = successPageGO;
            }
            else
            {
                _SuccessPage.SetActive(true);
            }
            _SuccessPage.transform.SetParent(matchsuccesspoint, false);
            _SuccessPage.transform.localPosition = Vector3.zero;
            Vector3 startPosition = _SuccessPage.transform.position;
            float screenHeightOffset = Screen.height * 0.2f;
            Vector3 endPosition = startPosition + new Vector3(0, -screenHeightOffset, 0);
            _SuccessPage.transform.DOMove(endPosition, 0.5f);

            GameObject num = _SuccessPage.transform.Find("Num").gameObject;
            _CountdownDuration = AutoConst.MatchRules.Countdown;
            _CountdownStartTime = SafeSystem.Time.GetMillisecondTick();
            _IsCountingDown = true;
            //XApi.TimerApi.StartTimer(CountDownOver, _SuccessPage, startPosition, (int)_CountdownDuration, 1, XApi.ITimer.Unlimited);
        }

        // private void CountDownOver(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
        // {
        //     XApi.TimerApi.StopTimer(CountDownOver, obj);
        //     GameObject successpageGO = (GameObject)obj;
        //     Vector3 startPosition = (Vector3)context;
        //     successpageGO.transform.DOMove(startPosition, 0.5f).OnComplete(() =>
        //     {
        //         successpageGO.SetActive(false);
        //     });

        // }

        private void EndCountDown()
        {
            _IsCountingDown = false;
            if (_SuccessPage != null)
            {
                Vector3 startPosition = _SuccessPage.transform.position;
                _SuccessPage.transform.DOMove(startPosition, 0.5f).OnComplete(() =>
                {
                    _SuccessPage.SetActive(false);
                });
            }
        }
    }
}
