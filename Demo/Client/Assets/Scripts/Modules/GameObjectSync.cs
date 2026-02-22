using AutoConfig;
using GameUnit;
using System;
using UnityEngine;
using XEngine;
using XGame;
using XUtils;
using static GameUnitApi;

public interface IGameObjectSync : IModule
{
    public enum eEvents
    {
        SyncCreate,
        SyncRelease,
        HpChange,
        MpChange
    }

    void RegistEvent(eEvents ev, Action<IUnit> fun);
    IUnit CreateUnit(oGameUnit data, GameObject parent);

    public const string NodeGameUnits = "GameUnits";
    public GameObject GameUnitsNode { get; }

    void Clear();
}

class GameObjectSync : IGameObjectSync
{

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IGameObjectSync, GameObjectSync>();
    }

    IClient _Client;
    GameObject _GameUnitsNode;
    IFiniteStateMachine _FiniteStateMachine;
    ICharacterManager _CharacterManager;
    IShowFXManager _ShowFXManager;

    public bool Initialize()
    {
        _GameUnitsNode = new GameObject(IGameObjectSync.NodeGameUnits);
        GameObject.DontDestroyOnLoad(_GameUnitsNode);
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _ShowFXManager = Launcher.GetModule<IShowFXManager>();
        _Client.RegistProtobuf<oGameObjectAppearPush>(eServerID.GameObjectAppearPush, OnGameObjectAppearPush);
        _Client.RegistProtobuf<oGameObjectDisappearPush>(eServerID.GameObjectDisappearPush, OnGameObjectDisappearPush);
        _Client.RegistProtobuf<oGameObjectAttributeSync>(eServerID.GameObjectAttributeSync, OnGameObjectAttributeSync);

        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _FiniteStateMachine = Launcher.GetModule<IFiniteStateMachine>();
        return true;
    }

    public void Release()
    {

    }
    public GameObject GameUnitsNode => _GameUnitsNode;


    XEventPool<IGameObjectSync.eEvents> _Pool = new XEventPool<IGameObjectSync.eEvents>();
    public void RegistEvent(IGameObjectSync.eEvents ev, Action<IUnit> fun)
    {
        _Pool.Regist(ev, fun);
    }

    public IUnit CreateUnit(oGameUnit data, GameObject parent)
    {
        if (GameUnitApi.QueryUnit(data.Guid) != null)
        {
            XApi.Error($"GameObject {data.Guid} Already Exist");
            return null;
        }

        eUnitType type = (eUnitType)data.Type;
        IUnit unit = GameUnitApi.CreateUnit(
            data.Guid,
            data.ConfigId,
            type,
            (in UInt64 guid, in Int64 configId, in eUnitType type) =>
            {
                GameObject gameObject = CreateGameObject(guid, configId, type);
                if (gameObject != null)
                {
                    return gameObject;
                }

                return null;
            }, (IUnit unit) =>
            {
                for (int a = 0; a < data.Attributes.Count; a++)
                {
                    oAttribute attr = data.Attributes[a];
                    switch ((eValueType)attr.Value.Type)
                    {
                        case eValueType.Int16:
                            {
                                unit.SetInt16(attr.Index, (Int16)attr.Value.Int, false);
                                break;
                            }
                        case eValueType.UInt16:
                            {
                                unit.SetUInt16(attr.Index, (UInt16)attr.Value.UInt, false);
                                break;
                            }
                        case eValueType.Int32:
                            {
                                unit.SetInt32(attr.Index, (Int32)attr.Value.Int, false);
                                break;
                            }
                        case eValueType.UInt32:
                            {
                                unit.SetUInt32(attr.Index, (UInt32)attr.Value.UInt, false);
                                break;
                            }
                        case eValueType.Int64:
                            {
                                unit.SetInt64(attr.Index, (Int64)attr.Value.Int, false);
                                break;
                            }
                        case eValueType.UInt64:
                            {
                                unit.SetUInt64(attr.Index, (UInt64)attr.Value.UInt, false);
                                break;
                            }
                        case eValueType.String:
                            {
                                unit.SetString(attr.Index, attr.Value.String, false);
                                break;
                            }
                        case eValueType.Float:
                            {
                                unit.SetFloat(attr.Index, attr.Value.Float, false);
                                break;
                            }
                        case eValueType.Bool:
                            {
                                unit.SetBool(attr.Index, attr.Value.Bool, false);
                                break;
                            }
                        case eValueType.Vector2:
                            {
                                unit.SetVector2(attr.Index, new Vector2(attr.Value.Vector2.X, attr.Value.Vector2.Y), false);
                                break;
                            }
                        case eValueType.Vector3:
                            {
                                unit.SetVector3(attr.Index, new Vector3(attr.Value.Vector3.X, attr.Value.Vector3.Y, attr.Value.Vector3.Z), false);
                                break;
                            }
                        case eValueType.Vector4:
                            {
                                unit.SetVector4(attr.Index, new Vector4(attr.Value.Vector4.X, attr.Value.Vector4.Y, attr.Value.Vector4.Z, attr.Value.Vector4.W), false);
                                break;
                            }
                    }
                }

                unit.transform.parent = parent.transform;
                unit.Position = new Vector3(
                    data.Position.X,
                    data.Position.Y,
                    0
                );
                unit.Angle = data.Angle;
            }
        );
        return unit;
    }

    public void Clear()
    {
        throw new NotImplementedException();
    }
    public void CreateMainCharacterUnit(oGameUnit data)
    {
        oGameObjectAppearPush push = new oGameObjectAppearPush();
        push.Gos.Add(data);
        OnGameObjectAppearPush(push);
    }

    void OnGameObjectAppearPush(oGameObjectAppearPush push)
    {
        for (int i = 0; i < push.Gos.Count; i++)
        {
            //XApi.Error($"GameObject Appear {push.Gos[i]} {push.Gos[i].Type}");
            IUnit goUnit = CreateUnit(push.Gos[i], _GameUnitsNode);
            _Pool.Trigger(IGameObjectSync.eEvents.SyncCreate, goUnit);
            if (goUnit.Is(eUnitType.Creature))
                _FiniteStateMachine.EnterState(push.Gos[i].Guid, (AutoConfig.AutoEnum.eState)push.Gos[i].State, push.Gos[i].Angle, new Vector2(push.Gos[i].Position.X, push.Gos[i].Position.Y), true);
        }
    }

    void OnGameObjectDisappearPush(oGameObjectDisappearPush push)
    {
        for (int i = 0; i < push.Guids.Count; i++)
        {
            XApi.Debug($"GameObject Disppear {push.Guids[i]}");
            IUnit unit = GameUnitApi.QueryUnit(push.Guids[i]);
            if (unit != null)
            {
                _Pool.Trigger(IGameObjectSync.eEvents.SyncRelease, unit);
                _FiniteStateMachine.ReleaseFsm(push.Guids[i]);
                GameUnitApi.ReleaseUnit(unit);
            }
        }
    }

    void OnGameObjectAttributeSync(oGameObjectAttributeSync push)
    {
        IUnit unit = GameUnitApi.QueryUnit(push.Guid);
        if (null == unit)
        {
            XApi.Error($"Where is unit {push.Guid}");
            return;
        }

        switch (unit.Attributes[push.Attribute.Index].Type)
        {
            case eAttributeType.UInt16:
                {
                    unit.SetUInt16(push.Attribute.Index, (UInt16)push.Attribute.Value.UInt);
                    break;
                }
            case eAttributeType.Int16:
                {
                    unit.SetInt16(push.Attribute.Index, (Int16)push.Attribute.Value.Int);
                    break;
                }
            case eAttributeType.UInt32:
                {
                    unit.SetUInt32(push.Attribute.Index, (UInt32)push.Attribute.Value.UInt);
                    break;
                }
            case eAttributeType.Int32:
                {
                    unit.SetInt32(push.Attribute.Index, (Int32)push.Attribute.Value.Int);
                    break;
                }
            case eAttributeType.UInt64:
                {
                    unit.SetUInt64(push.Attribute.Index, (UInt64)push.Attribute.Value.UInt);
                    break;
                }
            case eAttributeType.Int64:
                {
                    unit.SetInt64(push.Attribute.Index, (Int64)push.Attribute.Value.Int);
                    break;
                }
            case eAttributeType.Float:
                {
                    unit.SetFloat(push.Attribute.Index, push.Attribute.Value.Float);
                    break;
                }
            case eAttributeType.String:
                {
                    unit.SetString(push.Attribute.Index, push.Attribute.Value.String);
                    break;
                }
            case eAttributeType.Bool:
                {
                    unit.SetBool(push.Attribute.Index, push.Attribute.Value.Bool);
                    break;
                }
            case eAttributeType.Vector2:
                {
                    unit.SetVector2(push.Attribute.Index, new Vector2(push.Attribute.Value.Vector2.X, push.Attribute.Value.Vector2.Y));
                    break;
                }
            case eAttributeType.Vector3:
                {
                    unit.SetVector3(push.Attribute.Index, new Vector3(push.Attribute.Value.Vector3.X, push.Attribute.Value.Vector3.Y, push.Attribute.Value.Vector3.Z));
                    break;
                }
            case eAttributeType.Vector4:
                {
                    unit.SetVector4(push.Attribute.Index, new Vector4(push.Attribute.Value.Vector4.X, push.Attribute.Value.Vector4.Y, push.Attribute.Value.Vector4.Z, push.Attribute.Value.Vector4.W));
                    break;
                }
        }

    }

    public GameObject CreateGameObject(in UInt64 guid, in Int64 configId, in eUnitType type)
    {
        switch (type)
        {
            case eUnitType.Character:
                return CreateCharacter(guid, configId);
            case eUnitType.Bullet:
                return CreateBullet(guid, configId);
            case eUnitType.Monster:
                return CreateMonster(guid, configId);
            case eUnitType.Boss:
                return CreateBoss(guid, configId);
            case eUnitType.Item:
                return CreateItem(guid, configId);
        }
        XApi.Error($"Create Game Unit {type} error");
        IUnit unit = null;//GameUnitApi.CreateUnit();
        unit.GetFloat(eCharacterAttr.Atk_Float);
        return null;
    }

    public GameObject CreateCharacter(in UInt64 guid, in Int64 configId)
    {
        var config = AutoConfig.Character.Get(configId);
        if (config == null)
        {
            XApi.Error($"CreateCharacter error configId {configId}");
            return null;
        }

        GameObject go = Resources.Load<GameObject>(config._Prefab);
        GameObject gameObject = GameObject.Instantiate(go);

        int layerIndex = LayerMask.NameToLayer("Character");
        gameObject.AddComponent<BoxCollider2D>();
        gameObject.gameObject.layer = layerIndex;
        return gameObject;
    }

    public GameObject CreateBullet(in UInt64 guid, in Int64 configId)
    {
        var config = AutoConfig.Bullet.Get(configId);
        if (config == null)
        {
            XApi.Error($"CreateBullet error configId {configId}");
            return null;
        }
        if (config._BulletPrefab == null || config._BulletPrefab == "")
        {
            return new GameObject();
        }

        GameObject go = Resources.Load<GameObject>(config._BulletPrefab);
        if (go == null)
        {
            return new GameObject();
        }
        GameObject gameObject = GameObject.Instantiate(go);

        _ShowFXManager.ShowBulletStartFX(gameObject, configId);

        return gameObject;
    }

    public GameObject CreateMonster(in UInt64 guid, in Int64 configId)
    {
        var config = AutoConfig.Monster.Get(configId);
        if (config == null)
        {
            XApi.Error($"CreateMonster error configId {configId}");
            return null;
        }

        GameObject go = Resources.Load<GameObject>(config._Prefab);
        GameObject gameObject = GameObject.Instantiate(go);

        return gameObject;
    }

    public GameObject CreateBoss(in UInt64 guid, in Int64 configId)
    {
        var config = AutoConfig.MonsterBoss.Get(configId);
        if (config == null)
        {
            XApi.Error($"CreateMonsterBoss error configId {configId}");
            return null;
        }

        GameObject go = Resources.Load<GameObject>(config._Prefab);
        GameObject gameObject = GameObject.Instantiate(go);

        return gameObject;
    }

    public GameObject CreateItem(in UInt64 guid, in Int64 configId)
    {
        var config = AutoConst.GameSetting.DropPrefab;
        if (config == null)
        {
            XApi.Error($"CreateItem error configId {configId}");
            return null;
        }

        GameObject go = Resources.Load<GameObject>(config);
        GameObject gameObject = GameObject.Instantiate(go);

        gameObject.AddComponent<CircleCollider2D>().isTrigger = true;
        gameObject.AddComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Kinematic;

        return gameObject;
    }
}
