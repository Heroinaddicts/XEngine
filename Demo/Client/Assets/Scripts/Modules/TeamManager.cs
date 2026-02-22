using System;
using UnityEngine;
using XGame;
using UnityEngine.UI;
using GameUnit;
using XEngine;
using XUtils;
using static GameUnitApi;
using AutoConfig;
using DG.Tweening;
using TMPro;


namespace XGameClient
{
    public interface ITeamManager : IModule
    {
        public void SendTeamInviteReq(in UInt64 accountId);//发送组队邀请给服务器
        public void SendApplyTeamReq(in UInt64 accountId);//发送加入队伍请求给服务器

        public bool IsSameTeam(in IUnit other);
    }

    class TeamManager : ITeamManager
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<ITeamManager, TeamManager>();
        }
        IClient _Client;
        ICharacterManager _CharacterManager;
        IGameSceneManager _GameSceneManager;
        IUICommon _UICommon;
        XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _Client = Launcher.GetModule<IClient>();
            _UICommon = Launcher.GetModule<IUICommon>();
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _Client.RegistProtobuf<oTeamInfoPush>(eServerID.TeamInfoPush, OnTeamInfoPush);
            _Client.RegistProtobuf<oTeamInviteReqPush>(eServerID.TeamInviteReqPush, OnTeamInviteReqPush);
            _Client.RegistProtobuf<oTeamApplyReqPush>(eServerID.TeamApplyReqPush, OnTeamApplyReqPush);
            _Client.RegistProtobuf<oTeamAddFailPush>(eServerID.TeamAddFailPush, OnTeamAddFailPush);
            _GameSceneManager = Launcher.GetModule<IGameSceneManager>();

            _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, OnMainCharacterEnter);
            _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.GameMapLoaded, OnSceneLoaded);


            return true;
        }

        private void OnSceneLoaded(UnityEngine.SceneManagement.Scene scene)
        {
            IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
            if(mainCharacter == null)
            {
                return;
            }
            Int64 sceneConfigID = _GameSceneManager.GetCurrentSceneConfig()._ID;
            if(AutoConfig.Scene.Get(sceneConfigID)._SceneType != AutoEnum.eSceneType.BattleCopy)
            {
                Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                Transform transform = canvas.transform.Find("ExitTeam");
                if(mainCharacter.GetUInt64(eCharacterAttr.TeamGuid_UInt64) != 0)
                {
                    transform.gameObject.SetActive(true);
                }
                else
                {
                    transform.gameObject.SetActive(false);
                }
            }
        }

        public void Release()
        {

        }
        private void OnMainCharacterEnter(IUnit unit)
        {
            if (unit.Type == eUnitType.Character)
            {
                bool isSelf = _CharacterManager.IsSameAccount(unit);
                if (isSelf)
                {
                    unit.RegistUInt64AttributeChange(eCharacterAttr.TeamGuid_UInt64, eUnitAttributeEvent.Changed,
                (in eUnitAttributeEvent ev, in IUnit unit, in int index, in UInt64 oldValue, in UInt64 newValue) =>
                {
                    try
                    {
                        if (newValue != 0)//有队伍
                        {
                            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                            Transform exitbtn = canvas.transform.Find("ExitTeam");
                            exitbtn.gameObject.SetActive(true);
                            exitbtn.gameObject.GetComponent<Button>().onClick.RemoveAllListeners();
                            exitbtn.gameObject.GetComponent<Button>().onClick.AddListener(() =>
                            {
                                oTeamQuitReq teamQuitReq = new oTeamQuitReq();
                                _Client.SendProtobuf(eClientID.TeamQuitReq, teamQuitReq);
                            });
                            return true;
                        }
                        else//没有队伍
                        {
                            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                            Transform exitbtn = canvas.transform.Find("ExitTeam");
                            exitbtn.gameObject.SetActive(false);
                            exitbtn.gameObject.GetComponent<Button>().onClick.RemoveAllListeners();

                            Transform teamLabContent = canvas.transform.Find("TeamLab/Viewport/Content");
                            foreach (Transform child in teamLabContent)
                            {
                                child.gameObject.SetActive(false);
                            }
                            return true;
                        }
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"Update main character team GUID UI error: {ex.Message}");
                    }
                    return true;
                });
                }
            }
        }

        public bool IsSameTeam(in IUnit other)
        {
            return other.GetUInt64(eCharacterAttr.TeamGuid_UInt64) != INVALID_UNIT_GUID && _CharacterManager.GetMainCharacterUnit().GetUInt64(eCharacterAttr.TeamGuid_UInt64) == other.GetUInt64(eCharacterAttr.TeamGuid_UInt64);
        }

        public void SendTeamInviteReq(in UInt64 targetAccountId)//发送组队邀请给服务器
        {
            Debug.LogError($"发送组队邀请给服务器，目标账号ID：{targetAccountId}");
            oTeamInviteReq teamInviteReq = new oTeamInviteReq();
            teamInviteReq.TargetGuid = targetAccountId;
            _Client.SendProtobuf(eClientID.TeamInviteReq, teamInviteReq);
        }
        private void OnTeamInviteReqPush(oTeamInviteReqPush body)//收到服务器的组队邀请
        {
            Debug.LogError($"收到某人的组队邀请，邀请者账号ID：{body.InviteGuid}");
            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
            if (canvas == null)
            {
                return;
            }
            Transform teammessagepoint = canvas.transform.Find("TeamPoint/TeamMassagePoint");
            if (teammessagepoint == null)
            {
                return;
            }
            foreach (Transform child in teammessagepoint)
            {
                if (child.name == "TeamMessagePage(Clone)" &&
                   child.Find("Message").GetComponent<TextMeshProUGUI>().text.Contains(body.NickName))
                {
                    return;
                }
            }
            GameObject teamMessagePage = Resources.Load<GameObject>("UI/Prefabs/Team/TeamMessagePage");
            GameObject teamMessagePageGO = GameObject.Instantiate(teamMessagePage, teammessagepoint);
            teamMessagePageGO.transform.localPosition = Vector3.zero;
            teamMessagePageGO.transform.Find("Message").GetComponent<TextMeshProUGUI>().text = body.NickName + "邀请你加入队伍";
            // Vector3 startPosition = teamMessagePageGO.transform.position;
            // Vector3 endPosition = startPosition + new Vector3(0, -260f, 0);
            Vector3 startPosition = teamMessagePageGO.transform.position;
            float screenHeightOffset = Screen.height * 0.2f;
            Vector3 endPosition = startPosition + new Vector3(0, -screenHeightOffset, 0);

            teamMessagePageGO.transform.Find("Yes").GetComponent<Button>().onClick.RemoveAllListeners();
            teamMessagePageGO.transform.Find("Yes").GetComponent<Button>().onClick.AddListener(() =>//接受组队
            {
                oTeamInviteReply teamInviteReply = new oTeamInviteReply();
                teamInviteReply.Agree = true;
                teamInviteReply.InviteGuid = body.InviteGuid;
                _Client.SendProtobuf(eClientID.TeamInviteReply, teamInviteReply);//发送同意回复
                XApi.TimerApi.StopTimer(ClearMessage, teamMessagePageGO);

                teamMessagePageGO.transform.DOMove(startPosition, 0.5f).OnComplete(() =>
                {
                    GameObject.Destroy(teamMessagePageGO);
                });
            });
            teamMessagePageGO.transform.Find("No").GetComponent<Button>().onClick.RemoveAllListeners();
            teamMessagePageGO.transform.Find("No").GetComponent<Button>().onClick.AddListener(() =>//拒绝组队
            {
                oTeamInviteReply teamInviteReply = new oTeamInviteReply();
                teamInviteReply.Agree = false;
                teamInviteReply.InviteGuid = body.InviteGuid;
                _Client.SendProtobuf(eClientID.TeamInviteReply, teamInviteReply);//发送拒绝回复
                XApi.TimerApi.StopTimer(ClearMessage, teamMessagePageGO);

                teamMessagePageGO.transform.DOMove(startPosition, 0.5f).OnComplete(() =>
                {
                    GameObject.Destroy(teamMessagePageGO);
                });

            });

            teamMessagePageGO.transform.DOMove(endPosition, 0.5f);  // 0.5秒内完成移动

            XApi.TimerApi.StartTimer(ClearMessage, teamMessagePageGO, startPosition, (int)AutoConst.Partner.PartTime, 1, 0);
        }

        public void SendApplyTeamReq(in UInt64 accountId)//发送加入队伍请求给服务器
        {
            Debug.LogError($"发送加入队伍请求给服务器，目标账号ID：{accountId}");
            oTeamApplyReq teamApplyReq = new oTeamApplyReq();
            teamApplyReq.TargetGuid = accountId;
            _Client.SendProtobuf(eClientID.TeamApplyReq, teamApplyReq);
        }
        private void OnTeamApplyReqPush(oTeamApplyReqPush body)//队伍申请请求推送
        {
            Debug.LogError($"收到某人的加入队伍请求，申请人账号ID：{body.ApplyGuid}");
            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
            if (canvas == null)
            {
                return;
            }
            Transform teammessagepoint = canvas.transform.Find("TeamPoint/TeamMassagePoint");
            if (teammessagepoint == null)
            {
                return;
            }
            foreach (Transform child in teammessagepoint)
            {
                if (child.name == "TeamMessagePage(Clone)" &&
                   child.Find("Message").GetComponent<TextMeshProUGUI>().text.Contains(body.NickName))
                {
                    return;
                }
            }
            GameObject teamMessagePage = Resources.Load<GameObject>("UI/Prefabs/Team/TeamMessagePage");
            GameObject teamMessagePageGO = GameObject.Instantiate(teamMessagePage, teammessagepoint);
            teamMessagePageGO.transform.localPosition = Vector3.zero;
            teamMessagePageGO.transform.Find("Message").GetComponent<TextMeshProUGUI>().text = body.NickName + "请求加入队伍";
            // Vector3 startPosition = teamMessagePageGO.transform.position;
            // Vector3 endPosition = startPosition + new Vector3(0, -260f, 0);
            Vector3 startPosition = teamMessagePageGO.transform.position;
            float screenHeightOffset = Screen.height * 0.2f;
            Vector3 endPosition = startPosition + new Vector3(0, -screenHeightOffset, 0);
            teamMessagePageGO.transform.Find("Yes").GetComponent<Button>().onClick.RemoveAllListeners();
            teamMessagePageGO.transform.Find("Yes").GetComponent<Button>().onClick.AddListener(() =>//接受加入队伍
            {
                oTeamApplyReply teamApplyReply = new oTeamApplyReply();
                teamApplyReply.Agree = true;
                teamApplyReply.ApplyGuid = body.ApplyGuid;
                _Client.SendProtobuf(eClientID.TeamApplyReply, teamApplyReply);//发送同意回复
                XApi.TimerApi.StopTimer(ClearMessage, teamMessagePageGO);

                teamMessagePageGO.transform.DOMove(startPosition, 0.5f).OnComplete(() =>
                {
                    GameObject.Destroy(teamMessagePageGO);
                });
            });
            teamMessagePageGO.transform.Find("No").GetComponent<Button>().onClick.RemoveAllListeners();
            teamMessagePageGO.transform.Find("No").GetComponent<Button>().onClick.AddListener(() =>//拒绝加入队伍
            {
                oTeamApplyReply teamApplyReply = new oTeamApplyReply();
                teamApplyReply.Agree = false;
                teamApplyReply.ApplyGuid = body.ApplyGuid;
                _Client.SendProtobuf(eClientID.TeamApplyReply, teamApplyReply);//发送拒绝回复
                XApi.TimerApi.StopTimer(ClearMessage, teamMessagePageGO);

                teamMessagePageGO.transform.DOMove(startPosition, 0.5f).OnComplete(() =>
                {
                    GameObject.Destroy(teamMessagePageGO);
                });

            });

            teamMessagePageGO.transform.DOMove(endPosition, 0.5f);  // 0.5秒内完成移动 

            XApi.TimerApi.StartTimer(ClearMessage, teamMessagePageGO, startPosition, (int)AutoConst.Partner.PartTime, 1, 0);
        }

        private void OnTeamInfoPush(oTeamInfoPush body)//队伍信息推送
        {
            Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
            if (canvas == null)
            {
                return;
            }
            Transform teamLabContent = canvas.transform.Find("TeamLab/Viewport/Content");
            if (teamLabContent == null)
            {
                return;
            }

            for (int i = 0; i < teamLabContent.childCount; i++)
            {
                Transform memberTransform = teamLabContent.GetChild(i);

                if (i < body.TeamInfo.MemberInfos.Count)
                {
                    var member = body.TeamInfo.MemberInfos[i];

                    memberTransform.gameObject.SetActive(true);

                    TextMeshProUGUI name = memberTransform.Find("Name").GetComponent<TextMeshProUGUI>();
                    if (name != null)
                    {
                        name.text = member.NickName;
                    }

                    GameObject leadericon = memberTransform.Find("Leader")?.gameObject;
                    if (leadericon != null)
                    {
                        leadericon.SetActive(member.Guid == body.TeamInfo.LeaderGuid);
                    }

                    Image icon = memberTransform.Find("Icon").GetComponent<Image>();
                    if (icon != null)
                    {
                        icon.sprite = Resources.Load<Sprite>(AutoConfig.Character.Get(member.ConfigId)._Icon);
                    }

                    GameObject HpBar = memberTransform.Find("HP").gameObject;
                    GameObject MpBar = memberTransform.Find("MP").gameObject;

                    if (HpBar != null)
                    {
                        HpBar.transform.GetComponent<Image>().fillAmount = member.Hp / member.MaxHp;
                    }
                    if (MpBar != null)
                    {
                        MpBar.transform.GetComponent<Image>().fillAmount = member.Mp / member.MaxMp;
                    }
                }
                else
                {
                    memberTransform.gameObject.SetActive(false);
                }
            }
        }

        private void OnTeamAddFailPush(oTeamAddFailPush body)//队伍失败信息推送
        {
            string errormessage = GetTeamError(body.Error, body.Operate, body.NickName);

            Debug.LogError(errormessage);
        }

        private string GetTeamError(eError error, eTeamOperate operate, string nickName)
        {
            switch (error)
            {
                case eError.Refuse:
                    if (operate == eTeamOperate.Invite)
                    {
                        return $"{nickName}拒绝了你的邀请";
                    }
                    else
                    {
                        return "入队申请已被拒绝";
                    }
                case eError.MembersFull:
                    return "队伍已满";
                default:
                    return "操作失败";
            }
        }

        private void ClearMessage(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)//时间到了销毁
        {
            XApi.TimerApi.StopTimer(ClearMessage, obj);
            GameObject teamMessagePageGO = (GameObject)obj;
            if (teamMessagePageGO == null)
            {
                return;
            }
            Vector3 startPosition = (Vector3)context;
            teamMessagePageGO.transform.DOMove(startPosition, 0.5f).OnComplete(() =>
            {
                GameObject.Destroy(teamMessagePageGO);
            });
        }

    }

}

