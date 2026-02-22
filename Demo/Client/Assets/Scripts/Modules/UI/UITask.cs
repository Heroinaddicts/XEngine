using System;
using System.Threading.Tasks;
using AutoConfig;
using DG.Tweening;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using static GameUnitApi;

namespace XGameClient
{
    public interface IUITask : IModule
    {
        oTask GetTask();
    }

    public class UITask : IUITask
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IUITask, UITask>();
        }
        IUIMain _UIMain;
        ICharacterManager _CharacterManager;
        GameObject _UITask;
        IUnit _CharacterUnit;
        IClient _Client;
        oTaskDataPush _TaskDataPush;
        bool _IsLabOpen;
        oTask receivedTask = null;
        INpcManager _NpcManager;
        ITaskManager _TaskManager;
        private float _originalTaskViewWidth = -1;
        IMovement _Movement;

        GameObject rewardItemPrefab = Resources.Load<GameObject>("UI/Prefabs/TaskUI/RewardItem");

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _Client = Launcher.GetModule<IClient>();
            _UIMain = Launcher.GetModule<IUIMain>();
            _Movement = Launcher.GetModule<IMovement>();
            _TaskManager = Launcher.GetModule<ITaskManager>();
            _NpcManager = Launcher.GetModule<INpcManager>();
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _Client.RegistProtobuf<oTaskDataPush>(eServerID.TaskDataPush, OnTaskDataPush);
            _UIMain.RegistEvent(IUIMain.eEvent.OpenTask, OpenTask);

            _Client.RegistProtobuf<oTaskStatusChange>(eServerID.TaskStatusChange, OnTaskStatusChange);//任务状态改变
            _Client.RegistProtobuf<oNewTaskPush>(eServerID.NewTaskPush, OnNewTaskPush);
            return true;
        }
        public void Release()
        {

        }

        void OnTaskDataPush(oTaskDataPush taskDataPush)//任务数据推送
        {
            _TaskDataPush = taskDataPush;
            receivedTask = null;

            foreach (var task in _TaskDataPush.Tasks)
            {
                if ((AutoConfig.AutoEnum.eTaskEvent)task.TaskStatus != AutoConfig.AutoEnum.eTaskEvent.Completed)
                {
                    receivedTask = task;
                    //Debug.LogError($"任务数据推送 {receivedTask.TaskStatus} {receivedTask.ConfigId}");
                    break;
                }
            }

        }
        void OnTaskStatusChange(oTaskStatusChange changeTask)//任务状态改变
        {
            oTask Body = changeTask.ChangeTask;
            if (changeTask.ChangeTask.ConfigId == receivedTask.ConfigId)
            {
                receivedTask.TaskStatus = changeTask.ChangeTask.TaskStatus;
                AutoConfig.AutoEnum.eTaskReceiveType fuck = (AutoConfig.AutoEnum.eTaskReceiveType)AutoConfig.Task.Get(receivedTask.ConfigId)._TaskReceiveType;
                if ((AutoConfig.AutoEnum.eTaskReceiveType)AutoConfig.Task.Get(receivedTask.ConfigId)._TaskReceiveType == AutoConfig.AutoEnum.eTaskReceiveType.TriggleNpc && (AutoConfig.AutoEnum.eTaskEvent)Body.TaskStatus == AutoConfig.AutoEnum.eTaskEvent.Complete)
                {
                    _NpcManager.OpenCompletePage();
                }
            }
        }
        void OnNewTaskPush(oNewTaskPush newTaskPush)//新任务推送
        {
            oTask body = newTaskPush.NewTasks[0];

            receivedTask = newTaskPush.NewTasks[0];
            if (_TaskDataPush != null && newTaskPush.NewTasks.Count > 0)
            {
                foreach (var newTask in newTaskPush.NewTasks)
                {
                    // 检查任务是否已经在列表中
                    bool taskExists = false;
                    foreach (var task in _TaskDataPush.Tasks)
                    {
                        if (task.ConfigId == newTask.ConfigId)
                        {
                            taskExists = true;
                            break;
                        }
                    }

                    if (!taskExists)
                    {
                        _TaskDataPush.Tasks.Add(newTask);
                    }
                }
            }
            AutoEnum.eTaskVariety currentMissiontype = AutoConfig.TaskSystem.Get(AutoConfig.Task.Get(receivedTask.ConfigId)._TaskSystem)._TaskVariety;
            switch (currentMissiontype)
            {
                case AutoEnum.eTaskVariety.MainMission:
                    CheckPage(AutoEnum.eTaskVariety.MainMission);
                    break;
                case AutoEnum.eTaskVariety.SideMission:
                    CheckPage(AutoEnum.eTaskVariety.SideMission);
                    break;
                case AutoEnum.eTaskVariety.SpecialMission:
                    CheckPage(AutoEnum.eTaskVariety.SpecialMission);
                    break;
                case AutoEnum.eTaskVariety.DayMission:
                    CheckPage(AutoEnum.eTaskVariety.DayMission);
                    break;
            }
        }

        void OpenTask()
        {
            _UIMain.UnRegistEvent(IUIMain.eEvent.OpenTask, OpenTask);
            if (_UITask == null)
            {
                GameObject uiRes = Resources.Load<GameObject>("UI/Prefabs/TaskUI/UITask");
                _UITask = GameObject.Instantiate(uiRes);
                _UITask.transform.parent = GameObject.FindFirstObjectByType<Canvas>().gameObject.transform;
                _UITask.GetComponent<RectTransform>().offsetMax = new Vector2(0, 0);
                _UITask.GetComponent<RectTransform>().offsetMin = new Vector2(0, 0);
                _UITask.transform.localScale = Vector3.one;

                InitTaskUI();
            }
            else
            {
                _UITask.SetActive(true);
                InitTaskUI();
            }
        }
        void InitTaskUI()//初始化任务UI
        {
            ListenerBtnClickEvent();
            _UITask.transform.Find("BottomUI/TaskDeatil/AlreadyTaskView").gameObject.SetActive(false);
            _IsLabOpen = false;


            CheckPage(AutoConfig.AutoEnum.eTaskVariety.MainMission);
        }

        void ListenerBtnClickEvent()
        {
            Button close = _UITask.transform.Find("BottomUI/Close").GetComponent<Button>();
            close.onClick.RemoveAllListeners();
            close.onClick.AddListener(() =>
            {
                CloseTaskUI();
            });

            Button addAfford = _UITask.transform.Find("BottomUI/TaskDeatil/AddAfford").GetComponent<Button>();
            addAfford.onClick.RemoveAllListeners();
            addAfford.onClick.AddListener(() =>
            {
                if (_IsLabOpen)
                {
                    addAfford.transform.DORotate(new Vector3(0, 0, 0), 0.5f);
                }
                else
                {
                    addAfford.transform.DORotate(new Vector3(0, 0, 180), 0.5f);
                }

                OpenCompleteTaskLab();
            });
            Button main = _UITask.transform.Find("BottomUI/Tab/Main").GetComponent<Button>();
            main.onClick.RemoveAllListeners();
            main.onClick.AddListener(() =>//任务 
            {
                CheckPage(AutoConfig.AutoEnum.eTaskVariety.MainMission);
            });
            Button tree = _UITask.transform.Find("BottomUI/Tab/Tree").GetComponent<Button>();
            tree.onClick.RemoveAllListeners();
            tree.onClick.AddListener(() =>//副线
            {
                CheckPage(AutoConfig.AutoEnum.eTaskVariety.SideMission);
            });
            Button special = _UITask.transform.Find("BottomUI/Tab/Special").GetComponent<Button>();
            special.onClick.RemoveAllListeners();
            special.onClick.AddListener(() =>//特殊
            {
                CheckPage(AutoConfig.AutoEnum.eTaskVariety.SpecialMission);
            });
            Button day = _UITask.transform.Find("BottomUI/Tab/Day").GetComponent<Button>();
            day.onClick.RemoveAllListeners();
            day.onClick.AddListener(() =>//日常
            {
                CheckPage(AutoConfig.AutoEnum.eTaskVariety.DayMission);
            });

            // Button test1 = _UITask.transform.Find("BottomUI/Button1").GetComponent<Button>();
            // test1.onClick.RemoveAllListeners();
            // test1.onClick.AddListener(() =>
            // {
            //     Debug.LogError("接受任务");
            //     _TaskManager.AcceptTask(receivedTask.ConfigId);
            // });
            // Button test2 = _UITask.transform.Find("BottomUI/Button2").GetComponent<Button>();
            // test2.onClick.RemoveAllListeners();
            // test2.onClick.AddListener(() =>
            // {
            //     Debug.LogError("提交完成任务");
            //     _TaskManager.SubmitTask(receivedTask.ConfigId);
            // });
            // Button test3 = _UITask.transform.Find("BottomUI/Button3").GetComponent<Button>();
            // test3.onClick.RemoveAllListeners();
            // test3.onClick.AddListener(() =>
            // {
            //     Debug.LogError("碰到NPC完成任务");
            //     _TaskManager.CompleteTask(receivedTask.ConfigId);
            // });
        }

        private void OpenCompleteTaskLab()
        {
            GameObject taskView = _UITask.transform.Find("BottomUI/TaskDeatil/AlreadyTaskView").gameObject;
            RectTransform taskViewRect = taskView.GetComponent<RectTransform>();

            // 如果是第一次使用，保存原始宽度
            if (_originalTaskViewWidth == -1)
            {
                _originalTaskViewWidth = taskViewRect.sizeDelta.x;
            }

            if (_IsLabOpen == true)
            {
                // 关闭面板 - 向左收缩（宽度变小）
                taskViewRect.DOSizeDelta(new Vector2(0, taskViewRect.sizeDelta.y), 0.5f).OnComplete(() =>
                {
                    taskView.SetActive(false);
                });
                _IsLabOpen = false;
            }
            else
            {
                // 打开面板 - 从左到右展开（宽度变大）
                taskView.SetActive(true);
                taskViewRect.sizeDelta = new Vector2(0, taskViewRect.sizeDelta.y);
                taskViewRect.DOSizeDelta(new Vector2(_originalTaskViewWidth, taskViewRect.sizeDelta.y), 0.5f);
                _IsLabOpen = true;
            }
        }
        void CloseTaskUI()
        {
            _UIMain.RegistEvent(IUIMain.eEvent.OpenTask, OpenTask);
            _UITask.SetActive(false);
        }


        void CheckPage(AutoConfig.AutoEnum.eTaskVariety tasktype)
        {
            if (_UITask == null)
            {
                XApi.Debug("CheckPage: _UITask is null");
                return;
            }
            Transform taskItemList = _UITask.transform.Find("BottomUI/SmallTab/TabBottom/ItemView/Viewport/Content");
            for (int i = taskItemList.childCount - 1; i >= 0; i--)
            {
                GameObject.Destroy(taskItemList.GetChild(i).gameObject);
            }
            switch (tasktype)
            {
                case AutoConfig.AutoEnum.eTaskVariety.MainMission:
                    _UITask.transform.Find("BottomUI/Tab/Main/Light").gameObject.SetActive(true);
                    _UITask.transform.Find("BottomUI/Tab/Tree/Light").gameObject.SetActive(false);
                    _UITask.transform.Find("BottomUI/Tab/Special/Light").gameObject.SetActive(false);
                    _UITask.transform.Find("BottomUI/Tab/Day/Light").gameObject.SetActive(false);
                    OpenTaskPage(tasktype);
                    break;
                case AutoConfig.AutoEnum.eTaskVariety.SideMission:
                    _UITask.transform.Find("BottomUI/Tab/Main/Light").gameObject.SetActive(false);
                    _UITask.transform.Find("BottomUI/Tab/Tree/Light").gameObject.SetActive(true);
                    _UITask.transform.Find("BottomUI/Tab/Special/Light").gameObject.SetActive(false);
                    _UITask.transform.Find("BottomUI/Tab/Day/Light").gameObject.SetActive(false);
                    OpenTaskPage(tasktype);
                    break;
                case AutoConfig.AutoEnum.eTaskVariety.SpecialMission:
                    _UITask.transform.Find("BottomUI/Tab/Main/Light").gameObject.SetActive(false);
                    _UITask.transform.Find("BottomUI/Tab/Tree/Light").gameObject.SetActive(false);
                    _UITask.transform.Find("BottomUI/Tab/Special/Light").gameObject.SetActive(true);
                    _UITask.transform.Find("BottomUI/Tab/Day/Light").gameObject.SetActive(false);
                    OpenTaskPage(tasktype);
                    break;
                case AutoConfig.AutoEnum.eTaskVariety.DayMission:
                    _UITask.transform.Find("BottomUI/Tab/Main/Light").gameObject.SetActive(false);
                    _UITask.transform.Find("BottomUI/Tab/Tree/Light").gameObject.SetActive(false);
                    _UITask.transform.Find("BottomUI/Tab/Special/Light").gameObject.SetActive(false);
                    _UITask.transform.Find("BottomUI/Tab/Day/Light").gameObject.SetActive(true);
                    OpenTaskPage(tasktype);
                    break;
            }
        }
        void OpenTaskPage(AutoConfig.AutoEnum.eTaskVariety tasktype)
        {
            var taskSystems = System.Linq.Enumerable.ToList(
            System.Linq.Enumerable.Select(
            System.Linq.Enumerable.Where(
            AutoConfig.TaskSystem.Datas,
            kvp => kvp.Value._TaskVariety == tasktype
              ),
              kvp => kvp.Value
              )
            );
            GameObject defaultTaskItem = null;
            long defaultTaskId = 0;
            foreach (var taskSystem in taskSystems)
            {
                GameObject itemPrefab = Resources.Load<GameObject>("UI/Prefabs/TaskUI/TaskItem");
                Transform taskItemList = _UITask.transform.Find("BottomUI/SmallTab/TabBottom/ItemView/Viewport/Content");

                if (itemPrefab != null)
                {
                    GameObject taskItem = GameObject.Instantiate(itemPrefab, taskItemList);
                    taskItem.GetComponent<RectTransform>().offsetMax = new Vector2(0, 0);
                    taskItem.GetComponent<RectTransform>().offsetMin = new Vector2(0, 0);
                    taskItem.transform.localScale = Vector3.one;

                    taskItem.transform.Find("Title").GetComponent<TextMeshProUGUI>().text = taskSystem._Name;

                    GameObject title2 = taskItem.transform.Find("Title2").gameObject;
                    Button itemButton = taskItem.GetComponent<Button>();
                    Int64 currentCapter = AutoConfig.Task.Get(receivedTask.ConfigId)._TaskSystem;//得到当前进行中任务的章节 
                    if (currentCapter < taskSystem._ID)//如果当前进行中的任务章节ID小于Item的ID，那就是未解锁
                    {
                        title2.GetComponent<TextMeshProUGUI>().text = "未解锁";
                        itemButton.onClick.RemoveAllListeners();
                        itemButton.onClick.AddListener(() =>
                        {
                            return;
                        });

                    }
                    else if (currentCapter == taskSystem._ID)//如果当前进行中的任务章节ID等于Item的ID，那就是进行中
                    {
                        if (receivedTask != null)
                        {
                            title2.GetComponent<TextMeshProUGUI>().text = AutoConfig.Task.Get(receivedTask.ConfigId)._TaskName;
                            defaultTaskItem = taskItem;
                            defaultTaskId = taskSystem._ID;
                        }
                        itemButton.onClick.RemoveAllListeners();
                        itemButton.onClick.AddListener(() =>
                        {
                            OpenTaskItem(taskItem, taskSystem._ID);
                        });
                    }
                    else if (currentCapter > taskSystem._ID)//如果当前进行中的任务章节ID大于Item的ID，那就是已完成
                    {
                        title2.GetComponent<TextMeshProUGUI>().text = "已完成";
                        itemButton.onClick.RemoveAllListeners();
                        itemButton.onClick.AddListener(() =>
                        {
                            OpenTaskItem(taskItem, taskSystem._ID);
                        });
                    }
                }
            }
            if (defaultTaskItem != null)
                OpenTaskItem(defaultTaskItem, defaultTaskId);
        }

        void OpenTaskItem(GameObject taskItem, Int64 taskID)
        {
            Transform parent = taskItem.transform.parent;//处理点击亮暗
            if (parent != null)
            {
                for (int i = 0; i < parent.childCount; i++)
                {
                    Transform child = parent.GetChild(i);
                    Transform Lightnode = child.Find("Light");

                    if (child == taskItem.transform)
                    {
                        Lightnode.gameObject.SetActive(true);
                    }
                    else
                    {
                        Lightnode.gameObject.SetActive(false);
                    }
                }
            }
            Button btnGoToOBtain = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/BtnLab/GoToObtain").GetComponent<Button>();
            Button btnGoToFinish = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/BtnLab/GotoFinish").GetComponent<Button>();
            Button btnGoToReceive = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/BtnLab/GotoReceive").GetComponent<Button>();
            GameObject btnObtained = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/BtnLab/Obtained").gameObject;

            btnGoToOBtain.onClick.RemoveAllListeners();
            btnGoToOBtain.onClick.AddListener(() =>//点击去领取奖励
            {
                if (AutoConfig.Task.Get(receivedTask.ConfigId)._AcceptNpc == AutoConfig.Task.Get(receivedTask.ConfigId)._CompleteNpc)
                {
                    CloseTaskUI();
                    _NpcManager.OpenCompletePage();
                }
                else
                {
                    CloseTaskUI();
                    Int64 taskid = receivedTask.ConfigId;
                    Int64 completenpcid = AutoConfig.Task.Get(taskid)._CompleteNpc;
                    Vector2 completnpcPos = _NpcManager.GetNpcPosition(completenpcid);
                    _Movement.MainCharacterMoveto(in completnpcPos, (in bool arrived) =>
                    {
                        if (arrived)
                        {
                            XApi.Debug("已到达接任务NPC位置");
                        }
                        else
                        {
                            XApi.Debug("无法到达接任务NPC位置");
                        }
                    });
                }
            });
            btnGoToFinish.onClick.RemoveAllListeners();
            btnGoToFinish.onClick.AddListener(() =>                //点击去完成任务
            {
                if (AutoConfig.Task.Get(receivedTask.ConfigId)._TaskReceiveType == AutoEnum.eTaskReceiveType.TriggleNpc)
                {
                    CloseTaskUI();
                    Int64 taskid = receivedTask.ConfigId;
                    Int64 completenpcid = AutoConfig.Task.Get(taskid)._CompleteNpc;
                    Vector2 completnpcPos = _NpcManager.GetNpcPosition(completenpcid);
                    _Movement.MainCharacterMoveto(in completnpcPos, (in bool arrived) =>
                    {
                        if (arrived)
                        {
                            XApi.Debug("已到达接任务NPC位置");
                        }
                        else
                        {
                            XApi.Debug("无法到达接任务NPC位置");
                        }
                    });
                }
                else
                {
                    Debug.LogError("非接触完成的任务类型");
                }
            });
            btnGoToReceive.onClick.RemoveAllListeners();
            btnGoToReceive.onClick.AddListener(() =>                //点击去接任务
            {
                CloseTaskUI();
                Int64 taskid = receivedTask.ConfigId;
                Int64 acceptnpcid = AutoConfig.Task.Get(taskid)._AcceptNpc;
                Vector2 acceptnpcPos = _NpcManager.GetNpcPosition(acceptnpcid);
                _Movement.MainCharacterMoveto(in acceptnpcPos, (in bool arrived) =>
                {
                    if (arrived)
                    {
                        XApi.Debug("已到达接任务NPC位置");
                    }
                    else
                    {
                        XApi.Debug("无法到达接任务NPC位置");
                    }
                });

            });
            btnObtained.SetActive(false);
            btnGoToOBtain.gameObject.SetActive(false);
            btnGoToFinish.gameObject.SetActive(false);
            btnGoToReceive.gameObject.SetActive(false);

            TextMeshProUGUI taskName = _UITask.transform.Find("BottomUI/TaskDeatil/CurrentTask/TextTask").GetComponent<TextMeshProUGUI>();
            TextMeshProUGUI taskDes = _UITask.transform.Find("BottomUI/TaskDeatil/TextContentView/Viewport/Content/TextTask").GetComponent<TextMeshProUGUI>();
            GameObject itemPrefab = Resources.Load<GameObject>("UI/Prefabs/TaskUI/AlreadTaskItem");
            Int64 currentCapter = AutoConfig.Task.Get(receivedTask.ConfigId)._TaskSystem;//得到当前进行中任务的章节
            GameObject taskcomplete = _UITask.transform.Find("BottomUI/TaskDeatil/AlreadyTaskView/Viewport/Content").gameObject;
            GameObject rewardContent = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/RewardView/Viewport/Content").gameObject;
            Button currentTaskBtn = _UITask.transform.Find("BottomUI/TaskDeatil/CurrentTask").GetComponent<Button>();

            //如果当前进行中的任务章节ID小于Item的ID，那就是未解锁
            if (currentCapter < taskID)
            {
                return;
            }
            if (currentCapter == taskID)//当前进行中的任务章节ID等于Item的ID，那就是进行中
            {
                taskName.text = AutoConfig.Task.Get(receivedTask.ConfigId)._TaskName;
                taskDes.text = AutoConfig.Task.Get(receivedTask.ConfigId)._TaskDes;
                for (int i = rewardContent.transform.childCount - 1; i >= 0; i--)
                {
                    GameObject.Destroy(rewardContent.transform.GetChild(i).gameObject);
                }
                Int64[] rewardIDs = AutoConfig.Task.Get(receivedTask.ConfigId)._Resource;
                for (int i = 0; i < rewardIDs.Length; i++)
                {
                    Int64 rewardID = rewardIDs[i];
                    AutoConfig.Resource resource = AutoConfig.Resource.Get(rewardID);
                    GameObject rewardItem = GameObject.Instantiate(rewardItemPrefab, rewardContent.transform);
                    rewardItem.transform.Find("BG/Icon").GetComponent<Image>().sprite = Resources.Load<Sprite>(AutoConfig.Resource.Get(rewardID)._Icon);
                    rewardItem.transform.Find("BG/Num").GetComponent<TextMeshProUGUI>().text = AutoConfig.Resource.Get(rewardID)._Count.ToString();
                }
                for (int i = taskcomplete.transform.childCount - 1; i >= 0; i--)
                {
                    GameObject.Destroy(taskcomplete.transform.GetChild(i).gameObject);
                }
                Int64[] taskSon = AutoConfig.TaskSystem.Get(taskID)._TaskSon;
                for (int i = 0; i < taskSon.Length; i++)
                {
                    if (taskSon[i] < receivedTask.ConfigId)
                    {
                        GameObject item = GameObject.Instantiate(itemPrefab, taskcomplete.transform);
                        item.GetComponent<RectTransform>().offsetMax = new Vector2(0, 0);
                        item.GetComponent<RectTransform>().offsetMin = new Vector2(0, 0);
                        item.transform.localScale = Vector3.one;

                        item.transform.Find("TextCount").GetComponent<TextMeshProUGUI>().text = AutoConfig.Task.Get(taskSon[i])._TaskName;
                        Button itembutton = item.GetComponent<Button>();
                        itembutton.onClick.RemoveAllListeners();
                        int index = i;
                        itembutton.onClick.AddListener(() =>
                        {
                            Taskdetails(taskSon[index]);
                            btnGoToFinish.gameObject.SetActive(false);
                        });
                    }
                }
                currentTaskBtn.onClick.RemoveAllListeners();
                currentTaskBtn.onClick.AddListener(() =>
                {
                    Taskdetails(receivedTask.ConfigId);
                });
            }
            if (currentCapter > taskID)//如果当前进行中的任务章节ID大于Item的ID，那就是已完成
            {
                taskName.text = "任务完成";
                taskDes.text = "";
                for (int i = taskcomplete.transform.childCount - 1; i >= 0; i--)
                {
                    GameObject.Destroy(taskcomplete.transform.GetChild(i).gameObject);
                }
                for (int i = rewardContent.transform.childCount - 1; i >= 0; i--)
                {
                    GameObject.Destroy(rewardContent.transform.GetChild(i).gameObject);
                }
                Int64[] taskSon = AutoConfig.TaskSystem.Get(taskID)._TaskSon;
                for (int i = 0; i < taskSon.Length; i++)
                {
                    GameObject item = GameObject.Instantiate(itemPrefab, taskcomplete.transform);
                    item.GetComponent<RectTransform>().offsetMax = new Vector2(0, 0);
                    item.GetComponent<RectTransform>().offsetMin = new Vector2(0, 0);
                    item.transform.localScale = Vector3.one;

                    item.transform.Find("TextCount").GetComponent<TextMeshProUGUI>().text = AutoConfig.Task.Get(taskSon[i])._TaskName;
                    Button itembutton = item.GetComponent<Button>();
                    int index = i;
                    itembutton.onClick.RemoveAllListeners();
                    itembutton.onClick.AddListener(() =>
                    {
                        Taskdetails(taskSon[index]);
                    });
                }
            }
            AutoConfig.AutoEnum.eTaskEvent taskEvent = (AutoConfig.AutoEnum.eTaskEvent)receivedTask.TaskStatus;
            switch (taskEvent)
            {
                case AutoConfig.AutoEnum.eTaskEvent.Complete:
                    btnObtained.gameObject.SetActive(false);
                    btnGoToFinish.gameObject.SetActive(false);
                    btnGoToOBtain.gameObject.SetActive(true);
                    btnGoToReceive.gameObject.SetActive(false);
                    break;
                case AutoConfig.AutoEnum.eTaskEvent.Completed:
                    btnObtained.gameObject.SetActive(true);
                    btnGoToFinish.gameObject.SetActive(false);
                    btnGoToOBtain.gameObject.SetActive(false);
                    btnGoToReceive.gameObject.SetActive(false);
                    break;
                case AutoConfig.AutoEnum.eTaskEvent.PreRecive:
                    btnObtained.gameObject.SetActive(false);
                    btnGoToFinish.gameObject.SetActive(false);
                    btnGoToOBtain.gameObject.SetActive(false);
                    btnGoToReceive.gameObject.SetActive(true);
                    break;
                case AutoConfig.AutoEnum.eTaskEvent.Received:
                    btnObtained.gameObject.SetActive(false);
                    btnGoToFinish.gameObject.SetActive(true);
                    btnGoToOBtain.gameObject.SetActive(false);
                    btnGoToReceive.gameObject.SetActive(false);
                    break;
            }
        }
        void Taskdetails(Int64 taskID)
        {
            TextMeshProUGUI taskDes = _UITask.transform.Find("BottomUI/TaskDeatil/TextContentView/Viewport/Content/TextTask").GetComponent<TextMeshProUGUI>();
            GameObject rewardContent = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/RewardView/Viewport/Content").gameObject;


            taskDes.text = AutoConfig.Task.Get(taskID)._TaskDes;
            for (int i = rewardContent.transform.childCount - 1; i >= 0; i--)
            {
                GameObject.Destroy(rewardContent.transform.GetChild(i).gameObject);
            }
            Int64[] rewardIDs = AutoConfig.Task.Get(taskID)._Resource;
            for (int i = 0; i < rewardIDs.Length; i++)
            {
                Int64 rewardID = rewardIDs[i];
                AutoConfig.Resource resource = AutoConfig.Resource.Get(rewardID);
                GameObject rewardItem = GameObject.Instantiate(rewardItemPrefab, rewardContent.transform);
                rewardItem.transform.Find("BG/Icon").GetComponent<Image>().sprite = Resources.Load<Sprite>(AutoConfig.Resource.Get(rewardID)._Icon);
                rewardItem.transform.Find("BG/Num").GetComponent<TextMeshProUGUI>().text = AutoConfig.Resource.Get(rewardID)._Count.ToString();
            }
            ButtonControl(taskID);
        }
        void ButtonControl(Int64 taskID)
        {
            Button btnGoToOBtain = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/BtnLab/GoToObtain").GetComponent<Button>();
            Button btnGoToFinish = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/BtnLab/GotoFinish").GetComponent<Button>();
            Button btnGoToReceive = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/BtnLab/GotoReceive").GetComponent<Button>();
            GameObject btnObtained = _UITask.transform.Find("BottomUI/TaskDeatil/RewardBottom/BtnLab/Obtained").gameObject;


            if (receivedTask.ConfigId > taskID)
            {
                btnGoToFinish.gameObject.SetActive(false);
                btnGoToOBtain.gameObject.SetActive(false);
                btnGoToReceive.gameObject.SetActive(false);
                btnObtained.gameObject.SetActive(true);
            }
            else if (receivedTask.ConfigId == taskID)
            {
                switch (receivedTask.TaskStatus)
                {
                    case (Int32)AutoConfig.AutoEnum.eTaskEvent.Received://接受任务
                        btnGoToFinish.gameObject.SetActive(true);
                        btnGoToOBtain.gameObject.SetActive(false);
                        btnGoToReceive.gameObject.SetActive(false);
                        btnObtained.gameObject.SetActive(false);
                        break;
                    case (Int32)AutoConfig.AutoEnum.eTaskEvent.PreRecive://没接任务
                        btnGoToFinish.gameObject.SetActive(false);
                        btnGoToOBtain.gameObject.SetActive(false);
                        btnGoToReceive.gameObject.SetActive(true);
                        btnObtained.gameObject.SetActive(false);
                        break;
                    case (Int32)AutoConfig.AutoEnum.eTaskEvent.Complete://完成任务
                        btnGoToFinish.gameObject.SetActive(false);
                        btnGoToOBtain.gameObject.SetActive(true);
                        btnGoToReceive.gameObject.SetActive(false);
                        btnObtained.gameObject.SetActive(false);
                        break;
                }
            }

        }

        public oTask GetTask()
        {
            return receivedTask;
        }
    }
}

