using AutoConfig;
using GameUnit;
using System;
using System.Collections.Generic;
using System.Xml;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using XEngine;
using XGame;
using static GameUnitApi;

public interface ITaskManager : IModule
{
    public void AcceptTask(in Int64 taskID);
    public void SubmitTask(in Int64 taskID);
    public void CompleteTask(in Int64 taskID);
}

class TaskManager : ITaskManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<ITaskManager, TaskManager>();
    }
    IClient _Client;


    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        return true;
    }

    public void Release()
    {

    }
    public void AcceptTask(in Int64 taskID)
    {
        oAcceptTaskreq acceptTaskreq = new oAcceptTaskreq();
        acceptTaskreq.TaskId = taskID;
        _Client.SendProtobuf(eClientID.AcceptTaskReq, acceptTaskreq);
    }

    public void SubmitTask(in Int64 taskID)
    {
        oSubmitTaskReq submitTaskreq = new oSubmitTaskReq();
        submitTaskreq.TaskId = taskID;
        _Client.SendProtobuf(eClientID.SubmitTaskReq, submitTaskreq);
    }

    public void CompleteTask(in Int64 taskID)
    {
        oCompleteTask completeTaskreq = new oCompleteTask();
        completeTaskreq.TaskId = taskID;
        _Client.SendProtobuf(eClientID.CompleteTask, completeTaskreq);
    }
}
