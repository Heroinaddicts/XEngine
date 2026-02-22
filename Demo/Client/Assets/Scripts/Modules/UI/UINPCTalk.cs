using GameUnit;
using System;
using UnityEngine;
using XEngine;
using XGame;
using XGameClient;
using static GameUnitApi;
using TMPro;
using UnityEngine.UI;

public interface IUINPCTalk : IModule
{
    void ShowNPCDialog(Int64 taskID, AutoConfig.AutoEnum.eTaskEvent taskStatus, Int64 npcconfigID);
    void CloseNPCTalk();
}
public class DialogInfo
{
    public string Content;
    public string Title;
    public string Picture;
    public DialogInfo(string content, string title, string picture)
    {
        Content = content;
        Title = title;
        Picture = picture;
    }
}

class UINPCTalk : IUINPCTalk
{

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IUINPCTalk, UINPCTalk>();
    }

    IClient _Client;
    GameObject _NPCTalk;
    GameObject _Canvas;
    oTask _ReceivedTask = null;
    ICharacterManager _CharacterManager;
    IUITask _UITask;
    ITaskManager _TaskManager;


    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _UITask = Launcher.GetModule<IUITask>();
        _TaskManager = Launcher.GetModule<ITaskManager>();

        return true;
    }

    public void Release()
    {

    }


    public void ShowNPCDialog(Int64 taskID, AutoConfig.AutoEnum.eTaskEvent taskStatus, Int64 npcconfigID)
    {
        _Canvas = GameObject.Find("Canvas") ?? GameObject.FindFirstObjectByType<Canvas>()?.gameObject;
        _ReceivedTask = _UITask.GetTask();
        if (_ReceivedTask != null && (AutoConfig.AutoEnum.eTaskEvent)_ReceivedTask.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.Completed)
        {
            return;
        }
        if (_ReceivedTask == null)
        {
            return;
        }
        if (_NPCTalk == null)
        {
            _NPCTalk = Resources.Load<GameObject>("UI/Prefabs/Npc/UINpcTalk");
            _NPCTalk = GameObject.Instantiate(_NPCTalk);
            _NPCTalk.transform.SetParent(_Canvas.transform, false);

            _NPCTalk.GetComponent<RectTransform>().offsetMax = new Vector2(0, 0);
            _NPCTalk.GetComponent<RectTransform>().offsetMin = new Vector2(0, 0);
            _NPCTalk.transform.localScale = Vector3.one;
        }
        else
        {
            _NPCTalk.SetActive(true);
        }
        GameObject npcimg = _NPCTalk.transform.Find("NpcIMG").gameObject;//NPC图片
        TextMeshProUGUI name = _NPCTalk.transform.Find("NameBG/Name").GetComponent<TextMeshProUGUI>();//NPC名称
        TextMeshProUGUI dialogName = _NPCTalk.transform.Find("NpcName").GetComponent<TextMeshProUGUI>();//对话名字
        TextMeshProUGUI dialog = _NPCTalk.transform.Find("NpcName/Text").GetComponent<TextMeshProUGUI>();//对话内容
        Button yesBtn = _NPCTalk.transform.Find("TaskBtn/YesBtn").GetComponent<Button>();//接受任务按钮
        Button noBtn = _NPCTalk.transform.Find("TaskBtn/NoBtn").GetComponent<Button>();//拒绝任务按钮
        GameObject missionlab = _NPCTalk.transform.Find("MissionComplete").gameObject;//任务完成标签
        Button getrewardBtn = _NPCTalk.transform.Find("MissionComplete/GetBtn").GetComponent<Button>();
        Button backBtn = _NPCTalk.transform.Find("TaskBtn/BackBtn").GetComponent<Button>();//返回按钮

        DialogInfo dialogContent = GetDialogContentByTask(taskID, taskStatus, npcconfigID);
        dialog.text = dialogContent.Content;
        name.text = dialogContent.Title;
        dialogName.text = dialogContent.Title;
        npcimg.GetComponent<Image>().sprite = Resources.Load<Sprite>(dialogContent.Picture);



        yesBtn.onClick.RemoveAllListeners();
        yesBtn.onClick.AddListener(() =>
        {
            _TaskManager.AcceptTask(taskID);
            _NPCTalk.SetActive(false);
        });
        noBtn.onClick.RemoveAllListeners();
        noBtn.onClick.AddListener(() =>
        {
            _NPCTalk.SetActive(false);
        });
        backBtn.onClick.RemoveAllListeners();
        backBtn.onClick.AddListener(() =>
        {
            _NPCTalk.SetActive(false);
        });

        getrewardBtn.onClick.RemoveAllListeners();
        getrewardBtn.onClick.AddListener(() =>
        {
            //TODO：领取奖励
            if (_ReceivedTask != null && (AutoConfig.AutoEnum.eTaskEvent)_ReceivedTask.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.Complete)
            {
                _TaskManager.SubmitTask(taskID);
                _NPCTalk.SetActive(false);
            }
        });

        yesBtn.gameObject.SetActive(false);
        noBtn.gameObject.SetActive(false);
        getrewardBtn.gameObject.SetActive(false);
        backBtn.gameObject.SetActive(false);

        if (taskStatus == AutoConfig.AutoEnum.eTaskEvent.PreRecive)//如果是可以领取任务的状态
        {
            yesBtn.gameObject.SetActive(true);
            noBtn.gameObject.SetActive(true);
            getrewardBtn.gameObject.SetActive(false);
            backBtn.gameObject.SetActive(false);
            RewardLab(taskID);
        }
        if (taskStatus == AutoConfig.AutoEnum.eTaskEvent.Received)//如果任务已经领取但没做完
        {
            yesBtn.gameObject.SetActive(false);
            noBtn.gameObject.SetActive(false);
            getrewardBtn.gameObject.SetActive(false);
            backBtn.gameObject.SetActive(true);
            RewardLab(taskID);
        }
        if (taskStatus == AutoConfig.AutoEnum.eTaskEvent.Complete)//如果任务已经完成
        {
            yesBtn.gameObject.SetActive(false);
            noBtn.gameObject.SetActive(false);
            getrewardBtn.gameObject.SetActive(true);
            backBtn.gameObject.SetActive(false);
            RewardLab(taskID);
        }
    }

    DialogInfo GetDialogContentByTask(Int64 taskID, AutoConfig.AutoEnum.eTaskEvent taskStatus, Int64 npcconfigID)
    {
        foreach (var taskEventKvp in AutoConfig.TaskEvent.Datas)
        {
            var taskEvent = taskEventKvp.Value;
            if (taskEvent._Task == taskID && taskEvent._TaskProgress == taskStatus)
            {
                AutoConfig.Npc npc = AutoConfig.Npc.Get(npcconfigID);
                if (npc != null && npc._NpcTalk != null && npc._NpcTalk.Length > 0)
                {
                    foreach (Int64 talkId in npc._NpcTalk)
                    {
                        AutoConfig.NpcTalk npcTalk = AutoConfig.NpcTalk.Get(talkId);
                        if (npcTalk != null && npcTalk._TaskEvent == taskEvent._ID)
                        {
                            if (npcTalk._TalkContent != null && npcTalk._TalkContent.Length > 0)
                            {
                                AutoConfig.TalkContent talkContent = AutoConfig.TalkContent.Get(npcTalk._TalkContent[0]);
                                if (talkContent != null)
                                {
                                    return new DialogInfo(talkContent._Content, talkContent._Title, talkContent._Picture);
                                }
                            }
                        }
                    }

                    // AutoConfig.NpcTalk defaultNpcTalk = AutoConfig.NpcTalk.Get(npc._NpcTalk[0]);
                    // if (defaultNpcTalk != null && defaultNpcTalk._TalkContent.Length > 0)
                    // {
                    //     AutoConfig.TalkContent defaultTalk = AutoConfig.TalkContent.Get(defaultNpcTalk._TalkContent[0]);
                    //     if (defaultTalk != null)
                    //     {
                    //         return new DialogInfo(defaultTalk._Content, defaultTalk._Title, defaultTalk._Picture);

                    //     }
                    // }
                }
                break;
            }
        }

        return new DialogInfo("", "", "");
    }

    void RewardLab(Int64 taskID)
    {
        GameObject rewardlab = _NPCTalk.transform.Find("MissionComplete/RewardBar/Viewport/Content").gameObject;
        for (int i = rewardlab.transform.childCount - 1; i >= 0; i--)
        {
            GameObject.Destroy(rewardlab.transform.GetChild(i).gameObject);
        }
        Int64[] rewardID = AutoConfig.Task.Get(taskID)._Resource;
        GameObject rewarditem = Resources.Load<GameObject>("UI/Prefabs/TaskUI/RewardItem");

        for (int i = 0; i < rewardID.Length; i++)
        {
            GameObject item = GameObject.Instantiate(rewarditem, rewardlab.transform);
            Image Icon = item.transform.Find("BG/Icon").GetComponent<Image>();
            TextMeshProUGUI Num = item.transform.Find("BG/Num").GetComponent<TextMeshProUGUI>();

            Icon.sprite = Resources.Load<Sprite>(AutoConfig.Resource.Get(rewardID[i])._Icon);
            Num.text = AutoConfig.Resource.Get(rewardID[i])._Count.ToString();
        }

    }

    public void CloseNPCTalk()
    {
        if (_NPCTalk != null)
        {
            _NPCTalk.SetActive(false);
        }

    }
}
