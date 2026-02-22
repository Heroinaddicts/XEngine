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
    public interface IUIMiniMap : IModule
    {


    }
    class UIMiniMap : IUIMiniMap
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IUIMiniMap, UIMiniMap>();
        }

        ICharacterManager _CharacterManager;
        IGameObjectSync _GameObjectSync;
        XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _GameObjectSync = Launcher.GetModule<IGameObjectSync>();
            _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncCreate, OnCharacterSyncCreate);
            _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, OnMainCharacterEnter);
            return true;
        }

        private void OnMainCharacterEnter(IUnit unit)
        {
            if (unit.Type == eUnitType.Character)
            {
                bool isSelf = _CharacterManager.IsSameAccount(unit);
                //Debug.LogError($"OnCharacterSyncCreate: isSelf = {isSelf}");

                unit.transform.Find("MiniMapIcon").gameObject.SetActive(true);
                unit.transform.Find("TeammateIcon").gameObject.SetActive(false);

            }
        }

        private void OnCharacterSyncCreate(IUnit unit)
        {
            if (unit.Type == eUnitType.Character)
            {
                bool isSelf = _CharacterManager.IsSameAccount(unit);
                //Debug.LogError($"OnCharacterSyncCreate: isSelf = {isSelf}");

                unit.transform.Find("MiniMapIcon").gameObject.SetActive(false);
                unit.transform.Find("TeammateIcon").gameObject.SetActive(true);

            }
        }

        public void Release()
        {

        }


    }
}
