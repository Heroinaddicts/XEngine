using System;
using UnityEngine;
using XGame;
using UnityEngine.UI;
using GameUnit;
using XEngine;
using XUtils;
using static GameUnitApi;
using Spine.Unity;


namespace XGameClient
{
    public interface IUIGameResult : IModule
    {

    }

    class UIGameResult : IUIGameResult
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IUIGameResult, UIGameResult>();
        }

        ICharacterManager _CharacterManager;
        IClient _Client;
        IUICommon _UICommon;
        XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _Client = Launcher.GetModule<IClient>();
            _UICommon = Launcher.GetModule<IUICommon>();

            _Client.RegistProtobuf<oGameResultInfoPush>(eServerID.GameResultInfoPush, OnGameResultInfoPush);
            return true;
        }



        public void Release()
        {

        }

        private void OnGameResultInfoPush(oGameResultInfoPush body)//游戏结果推送
        {
            if (body.Success)
            {
                _UICommon.AlertUI("已成功离开","已成功离开","确认");
            }
            else
            {
                _UICommon.AlertUI("离开失败","离开失败","确认");
            }
            
        }

    }
}
