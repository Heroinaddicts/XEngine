using AutoConfig;
using GameUnit;
using System;
using UnityEngine;
using XEngine;
using XGame;
using XGameClient;
using static GameUnitApi;


public interface INpcManager : IModule
{
    Transform RandomNpc();
    public void OnTriggerEnter(IUnit self, GameObject other, Collider2D collider);
    public void OpenCompletePage();
    public void OnTriggerExit(IUnit self, GameObject other, Collider2D collider);

    public Vector2 GetNpcPosition(Int64 npcConfigID);
}

class NpcManager : INpcManager
{

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<INpcManager, NpcManager>();
    }

    IClient _Client;
    IGameSceneManager _GameScene;
    GameObject _NpcsNode;
    oTask _ReceivedTask = null;
    GameObject _Canvas;
    IUITask _UITask;
    IUINPCTalk _UINPCTalk;
    Int64 _NPCConfigID = 0;
    ICharacterManager _CharacterManager;
    ITaskManager _TaskManager;
    IGameSceneManager _GameSceneManager;

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _NpcsNode = new GameObject("Npcs");
        GameObject.DontDestroyOnLoad(_NpcsNode);
        _NpcsNode.transform.position = Vector3.zero;
        _UITask = Launcher.GetModule<IUITask>();
        _Client = Launcher.GetModule<IClient>();
        _Client.RegistProtobuf<oNpcInfoPush>(eServerID.NpcInfoPush, OnNpcInfoPush);
        _GameScene = Launcher.GetModule<IGameSceneManager>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _UINPCTalk = Launcher.GetModule<IUINPCTalk>();
        _TaskManager = Launcher.GetModule<ITaskManager>();
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();

        _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.GameMapLoaded, OnSceneLoaded);

        return true;
    }

    public void Release()
    {

    }

    void OnSceneLoaded(UnityEngine.SceneManagement.Scene scene)
    {
        GameObjectCommon.ClearAllChild(_NpcsNode);
    }

    public Transform RandomNpc()
    {
        if (0 == _NpcsNode.transform.childCount)
        {
            return null;
        }

        return _NpcsNode.transform.GetChild(new System.Random().Next(0, _NpcsNode.transform.childCount));
    }

    void OnNpcInfoPush(oNpcInfoPush push)
    {
        int layerIndex = LayerMask.NameToLayer("Npc");

        for (int i = 0; i < push.Infos.Count; i++)
        {
            var config = AutoConfig.Npc.Get(push.Infos[i].ConfigId);
            if (config == null)
            {
                XApi.Error($"OnNpcInfoPush error configId {push.Infos[i].ConfigId}");
                continue;
            }

            IUnit npcUnit = GameUnitApi.CreateUnit(
                push.Infos[i].Guid,
                push.Infos[i].ConfigId,
                eUnitType.Npc,
                (in UInt64 guid, in Int64 configId, in eUnitType type) =>
                {
                    GameObject prefab = Resources.Load<GameObject>(config._Prefab);
                    GameObject npc = GameObject.Instantiate(prefab);
                    npc.name = config._Name;
                    npc.gameObject.layer = layerIndex;
                    npc.AddComponent<BoxCollider2D>().isTrigger = true;
                    npc.AddComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Kinematic;
                    return npc;
                },
                (IUnit unit) =>
                {
                    unit.transform.parent = _NpcsNode.transform;
                    unit.transform.position = new Vector2(push.Infos[i].Pos.X, push.Infos[i].Pos.Y);
                }
            );

        }
    }
    public void OnClickEnter(IUnit self, GameObject other, Collider2D collider)
    {
        _NPCConfigID = 0;
        _Canvas = GameObject.Find("Canvas") ?? GameObject.FindFirstObjectByType<Canvas>()?.gameObject;
        _ReceivedTask = _UITask.GetTask();
        if (_ReceivedTask == null)
        {
            return;
        }
        IUnit colliderUnit = collider.GetComponent<IUnit>();
        Int64 npcconfigID = colliderUnit.ConfigID;//获取NPC的配置ID
        _NPCConfigID = npcconfigID;
        AutoConfig.Task taskconfig = AutoConfig.Task.Get(_ReceivedTask.ConfigId);
        if (collider.gameObject.layer == LayerMask.NameToLayer("Npc"))
        {
            AutoConfig.Npc npcConfig = AutoConfig.Npc.Get(npcconfigID);
            if (npcConfig == null)
            {
                XApi.Debug($"OnTriggerEnter error configID {npcconfigID}");
                return;
            }

            if (taskconfig._CompleteNpc == npcconfigID && (AutoConfig.AutoEnum.eTaskEvent)_ReceivedTask.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.Received && (AutoConfig.AutoEnum.eTaskReceiveType)AutoConfig.Task.Get(_ReceivedTask.ConfigId)._TaskReceiveType == AutoConfig.AutoEnum.eTaskReceiveType.TriggleNpc)
            {
                _TaskManager.CompleteTask(_ReceivedTask.ConfigId);
                return;
            }
            if ((AutoConfig.AutoEnum.eTaskEvent)_ReceivedTask.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.Complete)
            {
                OpenCompletePage();
                return;
            }

            else if (_ReceivedTask != null)
            {

                if (taskconfig._AcceptNpc == npcconfigID)
                {
                    if ((AutoConfig.AutoEnum.eTaskEvent)_ReceivedTask.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.PreRecive)
                    {
                        _UINPCTalk.ShowNPCDialog(_ReceivedTask.ConfigId, AutoConfig.AutoEnum.eTaskEvent.PreRecive, npcconfigID);//显示接受任务的对话（接受任务NPC）
                    }
                    else if ((AutoConfig.AutoEnum.eTaskEvent)_ReceivedTask.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.Received)
                    {
                        _UINPCTalk.ShowNPCDialog(_ReceivedTask.ConfigId, AutoConfig.AutoEnum.eTaskEvent.Received, npcconfigID);//显示接了任务未完成任务的对话（接受任务NPC）
                    }
                }
                if (taskconfig._CompleteNpc == npcconfigID)
                {
                    if ((AutoConfig.AutoEnum.eTaskEvent)_ReceivedTask.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.Complete)//如果任务已经完成
                    {
                        _UINPCTalk.ShowNPCDialog(_ReceivedTask.ConfigId, AutoConfig.AutoEnum.eTaskEvent.Complete, npcconfigID);//显示完成任务的对话（完成任务NPC）
                    }
                    if ((AutoConfig.AutoEnum.eTaskEvent)_ReceivedTask.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.Received)
                    {
                        _UINPCTalk.ShowNPCDialog(_ReceivedTask.ConfigId, AutoConfig.AutoEnum.eTaskEvent.Received, npcconfigID);//显示接了任务未完成任务的对话（完成任务NPC）
                    }
                }
                else if (taskconfig._CompleteNpc != npcconfigID && taskconfig._AcceptNpc != npcconfigID)
                {
                    //Debug.LogError("这个NPC啥也不是");
                }

            }
        }
    }
    public void OnTriggerExit(IUnit self, GameObject other, Collider2D collider)
    {
        if (_CharacterManager.GetMainCharacterUnit().ConfigID == self.ConfigID)
        {
            _UINPCTalk.CloseNPCTalk();
        }
    }

    public void OpenCompletePage()
    {
        _ReceivedTask = _UITask.GetTask();
        if ((AutoConfig.AutoEnum.eTaskEvent)_ReceivedTask.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.Complete)
        {
            _UINPCTalk.ShowNPCDialog(_ReceivedTask.ConfigId, AutoConfig.AutoEnum.eTaskEvent.Complete, _NPCConfigID);//显示完成任务的对话,领取奖励（接受任务NPC）
        }
    }

    public Vector2 GetNpcPosition(Int64 npcConfigID)
    {
        float npcpositionX = AutoConfig.Npc.Get(npcConfigID)._NpcPosition[0];
        float npcpositionY = AutoConfig.Npc.Get(npcConfigID)._NpcPosition[1];

        return new Vector2(npcpositionX, npcpositionY);
    }

    public void OnTriggerEnter(IUnit self, GameObject other, Collider2D collider)
    {
        _Canvas = GameObject.Find("Canvas") ?? GameObject.FindFirstObjectByType<Canvas>()?.gameObject;
        GameObject topButton = Resources.Load<GameObject>("UI/Prefabs/Npc/UINpcTips");
        if (topButton != null)
        {
            GameObject npcTips = GameObject.Instantiate(topButton);
            npcTips.transform.SetParent(_Canvas.transform.Find("NPCTopLab"), false);
        }
    }
}