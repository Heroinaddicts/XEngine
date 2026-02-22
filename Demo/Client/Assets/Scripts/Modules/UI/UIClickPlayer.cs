using System;
using UnityEngine;
using XGame;
using UnityEngine.UI;
using GameUnit;
using XEngine;
using XUtils;
using static GameUnitApi;
using AutoConfig;

namespace XGameClient
{
    public interface IUIClickPlayer : IModule
    {

    }

    class UIClickPlayer : IUIClickPlayer
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IUIClickPlayer, UIClickPlayer>();
        }

        ICharacterManager _CharacterManager;
        IScreenSelecter _ScreenSelecter;
        ITeamManager _TeamManager;
        XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _ScreenSelecter = Launcher.GetModule<IScreenSelecter>();
            _TeamManager = Launcher.GetModule<ITeamManager>();

            _ScreenSelecter.Regist(IScreenSelecter.eEvent.Select, OnSelectUnit);
            _ScreenSelecter.Regist(IScreenSelecter.eEvent.UnSelect, OnDeselectUnit);
            return true;
        }

        private void OnSelectUnit(IUnit unit)
        {
            if (unit.Type == eUnitType.Character)
            {
                bool isSelf = _CharacterManager.IsSameAccount(unit);
                if (isSelf == true)
                {
                    //Debug.LogError($"点击自己");
                    return;
                }
                Transform canvasTransform = GameObject.Find("Canvas")?.transform;
                if (canvasTransform == null)
                {
                    Debug.LogError("找不到主Canvas");
                    return;
                }
                Transform ClickUI = canvasTransform.Find("PlayerClick");
                if (ClickUI == null)
                {
                    Debug.LogError("找不到PlayerClick");
                    return;
                }
                Vector3 inputPosition;
#if UNITY_ANDROID || UNITY_IOS
                if (Input.touchCount > 0)
                {
                    Touch touch = Input.GetTouch(0);
                    inputPosition = new Vector3(touch.position.x, touch.position.y, 0);
                }
                else
                {
                    inputPosition = Input.mousePosition;
                }
#else
                inputPosition = Input.mousePosition;
#endif
                Canvas canvas = canvasTransform.GetComponent<Canvas>();
                Vector2 anchoredPosition;

                if (canvas != null && RectTransformUtility.ScreenPointToLocalPointInRectangle(
                        canvas.GetComponent<RectTransform>(),
                        inputPosition,
                        canvas.renderMode == RenderMode.ScreenSpaceOverlay ? null : canvas.worldCamera,
                        out anchoredPosition))
                {
                    ClickUI.GetComponent<RectTransform>().anchoredPosition = anchoredPosition;
                }
                else
                {
                    ClickUI.GetComponent<RectTransform>().anchoredPosition = inputPosition;
                }
                ClickUI.gameObject.SetActive(true);
                GameObject TeamInviteBtn = ClickUI.transform.Find("Viewport/Content/TeamInvite").gameObject;
                TeamInviteBtn.GetComponent<Button>().onClick.RemoveAllListeners();
                TeamInviteBtn.GetComponent<Button>().onClick.AddListener(() =>
                {
                    //组队邀请
                    _TeamManager.SendTeamInviteReq(unit.Guid);
                    ClickUI.gameObject.SetActive(false);
                });
                GameObject JoinTeamBtn = ClickUI.transform.Find("Viewport/Content/JoinTeam").gameObject;
                JoinTeamBtn.GetComponent<Button>().onClick.RemoveAllListeners();
                JoinTeamBtn.GetComponent<Button>().onClick.AddListener(() =>
                {
                    //加入组队
                    _TeamManager.SendApplyTeamReq(unit.Guid);
                    ClickUI.gameObject.SetActive(false);
                });

                IUnit characterUnit = _CharacterManager.GetMainCharacterUnit();

                UInt64 selfTeamID = characterUnit.GetUInt64(eCharacterAttr.TeamGuid_UInt64);
                UInt64 targetTeamID = unit.GetUInt64(eCharacterAttr.TeamGuid_UInt64);

                if (selfTeamID == 0 && targetTeamID == 0)//我没队伍他也没队伍
                {
                    TeamInviteBtn.SetActive(true);
                    JoinTeamBtn.SetActive(false);
                }
                else if (selfTeamID != 0 && targetTeamID == 0)//我有队伍他没队伍
                {
                    TeamInviteBtn.SetActive(true);
                    JoinTeamBtn.SetActive(false);
                }
                else if (selfTeamID == 0 && targetTeamID != 0)//我没队伍他有队伍
                {
                    TeamInviteBtn.SetActive(false);
                    JoinTeamBtn.SetActive(true);
                }
                else if (selfTeamID != 0 && targetTeamID != 0)//我有队伍他也有队伍
                {
                    TeamInviteBtn.SetActive(false);
                    JoinTeamBtn.SetActive(false);
                }

                // bool hasSelfTeam = selfTeamID != 0;
                // bool hasTargetTeam = targetTeamID != 0;

                // TeamInviteBtn.SetActive(!hasTargetTeam && hasSelfTeam); // 只有当目标没有队伍且自己有队伍时才显示邀请按钮
                // JoinTeamBtn.SetActive(!hasSelfTeam && hasTargetTeam);   // 只有当自己没有队伍且目标有队伍时才显示加入按钮
            }

        }

        private void OnDeselectUnit(IUnit unit)
        {
            Transform canvasTransform = GameObject.Find("Canvas")?.transform;
            if (canvasTransform == null)
            {
                Debug.LogError("找不到主Canvas");
                return;
            }
            Transform ClickUI = canvasTransform.Find("PlayerClick");
            if (ClickUI == null)
            {
                Debug.LogError("找不到PlayerClick");
                return;
            }
            ClickUI.gameObject.SetActive(false);

        }

        public void Release()
        {

        }



    }
}
