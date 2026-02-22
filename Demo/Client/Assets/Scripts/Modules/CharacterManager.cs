using GameUnit;
using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;
using static GameUnitApi;
using static XEngine.XApi.ITimer;

public interface ICharacterManager : IModule
{
    enum eEvents
    {
        MainCharacterExit = 0,
        MainCharacterEnter = 1,
        CharacterCreated = 2
    }

    void RegistEvent(eEvents e, Action<IUnit> callback, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0, [CallerMemberName] string member = "");

    IUnit GetMainCharacterUnit();

    bool IsSameAccount(IUnit unit);

    public void SendSwitchCharacter(in UInt64 characterId);

    public void WearEquip(in UInt64 equipId);
    public void UnWearEquip(in UInt64 equipId);

    public GameObject SelfCharactersNode { get; }

    Dictionary<UInt64, IUnit> GetSelfCharacters();
}

class CharacterManager : ICharacterManager
{

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<ICharacterManager, CharacterManager>();
    }

    public const string ConstNodeSelfCharactersName = "SelfCharacters";

    IClient _Client;
    IGameSceneManager _GameScene;
    IFiniteStateMachine _FiniteStateMachine;
    IWeaponManager _WeaponManager;
    IGameObjectSync _GameObjectSync;
    IUILootItemLab _UILootItemLab;
    IMovement _Movement;
    IUICommon _UICommon;

    UInt64 _MainCharacterID;
    INpcManager _NpcManager;
    Dictionary<UInt64, IUnit> _SelfCharacters = new Dictionary<ulong, IUnit>();

    XEventPool<ICharacterManager.eEvents> _EventPool = new XEventPool<ICharacterManager.eEvents>();
    GameObject _SelfCharactersNode;
    public GameObject SelfCharactersNode => _SelfCharactersNode;

    public bool Initialize()
    {
        _SelfCharactersNode = new GameObject(ConstNodeSelfCharactersName);
        _SelfCharactersNode.SetActive(false);
        GameObject.DontDestroyOnLoad(_SelfCharactersNode);
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _Client.RegistProtobuf<oSwitchCharacterPush>(eServerID.SwitchCharacterPush, OnSwitchCharacterPush);
        _Client.RegistProtobuf<oEquipPush>(eServerID.EquipPush, OnEquipPush);
        _NpcManager = Launcher.GetModule<INpcManager>();
        _GameScene = Launcher.GetModule<IGameSceneManager>();
        _UICommon = Launcher.GetModule<IUICommon>();
        _UILootItemLab = Launcher.GetModule<IUILootItemLab>();

        _FiniteStateMachine = Launcher.GetModule<IFiniteStateMachine>();
        _FiniteStateMachine.RegistEnterJudge(AutoConfig.AutoEnum.eState.Moving, OnEnterMovingJudge);
        _FiniteStateMachine.RegistStateEnter(AutoConfig.AutoEnum.eState.Death, OnEnterDeath);

        _WeaponManager = Launcher.GetModule<IWeaponManager>();
        _GameObjectSync = Launcher.GetModule<IGameObjectSync>();
        _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncCreate, OntherPlayerCreate);

        UIJoyStick.RegistJoyStickEvent(UIJoyStick.eJoyStickEvent.StickSliding, OnJoyStickSliding);
        UIJoyStick.RegistJoyStickEvent(UIJoyStick.eJoyStickEvent.StickUp, OnJoyStickUp);

        _Client.RegistProtobuf<oCharacterInfoPush>(eServerID.CharacterInfoPush, OnCharacterInfoPush);
        _Client.RegistProtobuf<oCorrectPosition>(eServerID.CorrectPosition, OnCorrectPosition);
        _Client.RegistProtobuf<oDelayTestAsw>(eServerID.DelayTestAsw, OnDelayTestAsw);


        _Movement = Launcher.GetModule<IMovement>();
        return true;
    }

    public void Release()
    {

    }


    public Dictionary<UInt64, IUnit> GetSelfCharacters()
    {
        return _SelfCharacters;
    }

    void OnCorrectPosition(oCorrectPosition pos)
    {
        XApi.Debug($"OnCorrectPosition to ({pos.Position.X}, {pos.Position.Y})");
        GetMainCharacterUnit().transform.position = new Vector2(pos.Position.X, pos.Position.Y);
    }

    void OnDelayTestAsw(oDelayTestAsw asw)
    {
        GameObjectCommon.GetGameObjectByPath("Canvas/TextDelayTest").GetComponent<Text>().text = $"{SafeSystem.Time.GetMillisecondTick() - asw.Req.Tick}";
    }

    void OnDelayTestTimer(in eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
    {
        oDelayTestReq req = new oDelayTestReq();
        req.Tick = SafeSystem.Time.GetMillisecondTick();
        _Client.SendProtobuf(eClientID.DelayTestReq, req);
    }

    public void SendSwitchCharacter(in UInt64 characterId)
    {
        oSwitchCharacterReq req = new oSwitchCharacterReq();
        req.CharacterId = characterId;
        _Client.SendProtobuf(eClientID.SwitchCharacterReq, req);
    }

    void OnCharacterInfoPush(oCharacterInfoPush push)
    {
        int layerIndex = LayerMask.NameToLayer("MainCharacter");

        for (int i = 0; i < push.Infos.Count; i++)
        {
            IUnit mainCharacter = _GameObjectSync.CreateUnit(push.Infos[i], _SelfCharactersNode);
            //mainCharacter.AddComponent<BoxCollider2D>();
            mainCharacter.gameObject.layer = layerIndex;

            _SelfCharacters.Add(push.Infos[i].Guid, mainCharacter);
        }

        XApi.TimerApi.StartTimer(OnDelayTestTimer, this, this, 1000, Unlimited, 1000);
    }
    void OnTriggerEnter2D(IUnit self, GameObject other, Collider2D collider)
    {
        IUnit otherunit = other.GetComponent<IUnit>();
        if (otherunit == null)
        {
            return;
        }
        switch (otherunit.Type)
        {
            case eUnitType.Npc:
                _NpcManager.OnTriggerEnter(self, other, collider);
                XApi.Debug($"On Main Character TriggerEnter2D other is {collider.gameObject.name}");
                Debug.LogError($"On Main Character TriggerEnter2D other is {collider.gameObject.name}");
                break;
            case eUnitType.Item:
                _UILootItemLab.InitAndAddClickUI(otherunit);
                break;
        }


    }

    void OnTriggerExit2D(IUnit self, GameObject other, Collider2D collider)
    {
        IUnit otherunit = other.GetComponent<IUnit>();
        if (otherunit == null)
        {
            return;
        }
        switch (otherunit.Type)
        {
            case eUnitType.Npc:
                _NpcManager.OnTriggerExit(self, other, collider);
                XApi.Debug($"On Main Character OnTriggerExit2D other is {collider.gameObject.name}");
                Debug.LogError($"On Main Character OnTriggerExit2D other is {collider.gameObject.name}");
                break;
            case eUnitType.Item:
                _UILootItemLab.ReleaseItem(otherunit);
                break;
        }
    }

    private void OnSwitchCharacterPush(oSwitchCharacterPush push)
    {
        IUnit oldCharacter = GameUnitApi.QueryUnit(_MainCharacterID);
        if (null != oldCharacter)
        {
            oldCharacter.transform.parent = _SelfCharactersNode.transform;

            _EventPool.Trigger(ICharacterManager.eEvents.MainCharacterExit, oldCharacter);

            oldCharacter.CancelRegistTriggerEnter2D(OnTriggerEnter2D);
            oldCharacter.CancelRegistTriggerExit2D(OnTriggerExit2D);
        }

        _MainCharacterID = push.CharacterId;
        IUnit character = GameUnitApi.QueryUnit(_MainCharacterID);
        character.transform.parent = GameObject.Find(IGameObjectSync.NodeGameUnits).transform;
        character.Position = new Vector2(push.X, push.Y);

        character.RegistTriggerEnter2D(OnTriggerEnter2D);
        character.RegistTriggerExit2D(OnTriggerExit2D);
        _EventPool.Trigger(ICharacterManager.eEvents.MainCharacterEnter, character);
    }

    public IUnit GetMainCharacterUnit()
    {
        return GameUnitApi.QueryUnit(_MainCharacterID);
    }

    void OnEnterDeath(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        if (guid == _MainCharacterID)
        {
            _UICommon.AlertUI("提示", "角色已死亡", "回城", () =>
            {
                //                 oSwitchSceneTest req = new oSwitchSceneTest();
                //                 req.SceneConfigId = AutoConfig.AutoConst.World.DefaultWorld;
                //                 _Client.SendProtobuf(eClientID.SwitchSceneTest, req);
                oExitCopyScene req = new oExitCopyScene();
                _Client.SendProtobuf(eClientID.ExitCopyScene, req);
            });
        }
    }

    bool OnEnterMovingJudge(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            return false;
        }
        if (unit.GetFloat(eEntityAttr.Hp_Float) <= 0)
        {
            return false;
        }
        float angleInRadians = angle * (3.141592653589793f / 180.0f);
        float dx = (float)(Math.Cos(angleInRadians) * 0.1f);
        float dy = (float)(Math.Sin(angleInRadians) * 0.1f);

        IGameTilemap tilemap = _GameScene.GetCurrentTilemap();
        if (null == tilemap)
        {
            return false;
        }

        if (tilemap.CanMove(new Vector2(unit.Position.x + dx, unit.Position.y + dy)))
        {
            return true;
        }
        else if (unit.Guid == GetMainCharacterUnit().Guid)
        {
            Vector2 slidPos = new Vector2();
            return _Movement.CanSlide(unit, tilemap, new Vector2(dx, dy), ref slidPos);
        }

        return false;
    }

    void OnJoyStickSliding(float angle)
    {

        IUnit mainCharacter = GetMainCharacterUnit();
        if (null != mainCharacter)
        {
            Int64 lastSkillConfigID = mainCharacter.GetInt64(GameUnit.eCharacterAttr.LastSkillConfigID_Int64);
            UInt64 lastSkillUseTime = mainCharacter.GetUInt64(GameUnit.eCharacterAttr.LastSkillUseTime_UInt64);

            if (lastSkillConfigID != 0 && lastSkillUseTime != 0)
            {
                AutoConfig.Skill skillConfig = AutoConfig.Skill.Get(lastSkillConfigID);
                if (skillConfig != null)
                {
                    Int64 tick = SafeSystem.Time.GetMillisecondTick() - (Int64)lastSkillUseTime;

                    if (tick < skillConfig._CantMoveTime)
                    {
                        // Debug.LogWarning($"角色正在施法中，忽略摇杆输入: {tick} < {skillConfig._CantMoveTime}");
                        return;
                    }
                    if (tick < skillConfig._SkillInterval)
                    {
                        // Debug.LogWarning($"角色正在施法中，忽略摇杆输入: {tick} < {skillConfig._SkillInterval}");
                        return;
                    }

                    if (tick > skillConfig._CantMoveTime)
                    {
                        // Debug.LogError($"可以移动了: {tick} > {skillConfig._CantMoveTime}");

                    }
                    if (tick > skillConfig._SkillInterval)
                    {
                        // Debug.LogError($"可以移动了: {tick} > {skillConfig._SkillInterval}");

                    }

                }
            }
        }

        _Movement.StopMainCharacterMoveto();

        if (null != mainCharacter)
        {
            if (mainCharacter.GetFloat(eCharacterAttr.Hp_Float) <= 0)
            {
                return;
            }
            if (_FiniteStateMachine.EnterState(mainCharacter.Guid, AutoConfig.AutoEnum.eState.Moving, angle, mainCharacter.Position))
            {
                oStateChange change = new oStateChange();
                change.Angle = angle;
                change.State = (int)AutoConfig.AutoEnum.eState.Moving;
                change.Pos = new oVector2();
                change.Pos.X = mainCharacter.Position.x;
                change.Pos.Y = mainCharacter.Position.y;
                _Client.SendProtobuf(eClientID.StateChange, change);
            }
        }
    }

    void OnJoyStickUp(float angle)
    {

        IUnit mainCharacter = GetMainCharacterUnit();
        if (null != mainCharacter)
        {
            Int64 lastSkillConfigID = mainCharacter.GetInt64(GameUnit.eCharacterAttr.LastSkillConfigID_Int64);
            UInt64 lastSkillUseTime = mainCharacter.GetUInt64(GameUnit.eCharacterAttr.LastSkillUseTime_UInt64);

            if (lastSkillConfigID != 0 && lastSkillUseTime != 0)
            {
                AutoConfig.Skill skillConfig = AutoConfig.Skill.Get(lastSkillConfigID);
                if (skillConfig != null)
                {
                    Int64 tick = SafeSystem.Time.GetMillisecondTick() - (Int64)lastSkillUseTime;
                    if (tick < skillConfig._CantMoveTime)
                    {
                        Debug.LogWarning($"角色正在施法中，忽略摇杆释放输入: {tick} < {skillConfig._CantMoveTime}");
                        return;
                    }
                    if (tick < skillConfig._SkillInterval)
                    {
                        Debug.LogWarning($"角色正在施法中，忽略摇杆输入: {tick} < {skillConfig._SkillInterval}");
                        return;
                    }
                }
            }
        }

        _Movement.StopMainCharacterMoveto();
        if (null != mainCharacter)
        {
            if (mainCharacter.GetFloat(eCharacterAttr.Hp_Float) <= 0)
            {
                return;
            }
            if (_FiniteStateMachine.EnterState(mainCharacter.Guid, AutoConfig.AutoEnum.eState.Idle, angle, mainCharacter.Position))
            {
                oStateChange change = new oStateChange();
                change.Angle = angle;
                change.State = (int)AutoConfig.AutoEnum.eState.Idle;
                change.Pos = new oVector2();
                change.Pos.X = mainCharacter.Position.x;
                change.Pos.Y = mainCharacter.Position.y;

                _Client.SendProtobuf(eClientID.StateChange, change);
            }

        }
    }

    public void WearEquip(in UInt64 equipId)
    {
        IUnit mainCharacter = GetMainCharacterUnit();
        if (null != mainCharacter)
        {
            oEquipReq wear = new oEquipReq();
            wear.EquipGuid = equipId;
            _Client.SendProtobuf(eClientID.EquipReq, wear);
        }
    }

    public void UnWearEquip(in UInt64 equipId)
    {
        IUnit mainCharacter = GetMainCharacterUnit();
        if (null != mainCharacter)
        {
            oUnEquipReq unWear = new oUnEquipReq();
            unWear.EquipGuid = equipId;
            _Client.SendProtobuf(eClientID.UnEquipReq, unWear);
        }
    }

    void OnEquipPush(oEquipPush push)
    {
        IUnit unit = GameUnitApi.QueryUnit(push.CharacterId);
        int weaponID = (int)push.ConfigId;
        _WeaponManager.CharacterChangeWeapon(weaponID, unit);
    }

    void OntherPlayerCreate(IUnit unit)
    {
        // if (unit.Type == eUnitType.Character)
        //     _WeaponManager.CharacterChangeWeapon(unit.GetInt64(eCharacterAttr.WeaponConfig_Int64), unit);
    }

    public void RegistEvent(ICharacterManager.eEvents e, Action<IUnit> callback, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0, [CallerMemberName] string member = "")
    {
        _EventPool.Regist(e, callback, file, line, member);
    }

    public bool IsSameAccount(IUnit unit)
    {
        if (unit.Type != eUnitType.Character)
        {
            return false;
        }

        return unit.GetUInt64(eCharacterAttr.Account_UInt64) == _Client.Account();
    }
}
