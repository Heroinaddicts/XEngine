using HttpDef;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization.Json;
using TLib;
using TNet;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using UnityEditor;

public class LoginScene : IScene
{
    public InputField accountField;
    [NaughtyAttributes.ReadOnly]
    public int account = 10001;

    void Start()
    {
        InputField.OnChangeEvent evnt = new InputField.OnChangeEvent();
        accountField.onValueChanged = evnt;
        evnt.AddListener((string s) =>
        {
            if(!string.IsNullOrEmpty(s))
            {
               account = Convert.ToInt32(s);
            }
        });
#if UNITY_EDITOR
        account = PlayerPrefs.GetInt("Account", 0);
        accountField.SetTextWithoutNotify(account.ToString());
#endif
    }

    protected override void __Awake()
    {
        
    }

    protected override void __Start()
    {
    }

    protected override void __OnEnable()
    {
        ConnectionManager.RegisterConnectionEvent(OnConnectionEvent);
        ConnectionManager.RegisterProto<oGATE_INFO>(eSPID.GateInfo, OnSPIDGateInfoPush);
        ConnectionManager.RegisterProto<oLOGIN_ASW>(eSPID.LoginAsw, OnSPIDLoginAsw);
        ConnectionManager.RegisterProto<oSCENE_INFO>(eSPID.SceneInfo, OnSPIDSceneInfo);
    }
    protected override void __OnDisable()
    {
        ConnectionManager.UnregisterEvent(OnConnectionEvent);
    }

    protected override void __OnUpdate()
    {

    }
    public void OnBtnLoginClick()
    {
#if UNITY_EDITOR
        PlayerPrefs.SetInt("Account", account);
        PlayerPrefs.Save();
#endif
        ConnectionManager.Connect(eConnectionType.GameSLB, Define.ServerHost.Balance.ip, Define.ServerHost.Balance.port);
    }

    protected override void OnResponse(in eRequestID rid, in bool isSuccess, in string data)
    {
        
    }

    void OnSPIDGateInfoPush(in Connection con, in oGATE_INFO push)
    {
        Debug.Log("Gate Info Ip : " + push.Ip + ", Port : " + push.Port);
        ConnectionManager.Connect(eConnectionType.GameGate, push.Ip, push.Port);
    }

    void OnSPIDLoginAsw(in Connection con, in oLOGIN_ASW push)
    {
        if(push.Code == eError.Success)
        {
            CharactorManager.Instance.LocalPlayerGID = push.Account;
            Debug.Log("Login Success, Account : " + push.Account);
        }
        else
        {
            Debug.LogError("login fail ErrorCode = " + push.Code.ToString());
        }
    }

    void OnSPIDSceneInfo(in Connection con, in oSCENE_INFO info)
    {
        Debug.Log("scene id = " + info.Info.ConfigId);
        var map_config = Module.GetModule<Config>().GetConfig().o_config_map_map[info.Info.ConfigId];
        Core.Instance.LoadScene(map_config.name);
    }

    void OnSPIDLoginAws(in Connection con, in oLOGIN_ASW aws)
    {
        if (aws.Code == eError.Success)
        {
            Debug.Log("Login Success, Account : " + aws.Account);
        } else
        {
            Debug.LogError("Login error");
        }
    }

    void OnConnectionEvent(in eConnectionEventType type, in Connection connection)
    {
        switch (type)
        {
            case eConnectionEventType.Connected:
                {
                    if (connection._type == eConnectionType.GameGate)
                    {
                        oLOGIN_REQ req = new oLOGIN_REQ();
                        req.Account = account;
                        connection.SendProto<oLOGIN_REQ>(eCPID.LoginReq, req);
                    }

                    break;
                }
            case eConnectionEventType.ConnectFailed:
                {
                    break;
                }
            case eConnectionEventType.Disconnected:
                {
                    break;
                }
        }
    }

#if UNITY_EDITOR
    [NaughtyAttributes.Button("登录")]
    void Login()
    {
        Core.Instance.LoadScene("FollowCam");
    }
#endif
}
