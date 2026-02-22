using Google.Protobuf;
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;
using static XEngine.XApi.INetwork;

public interface IClient : IModule
{
    string GetIDFA();
    public enum eEvent
    {
        Login
    }
    void Regist<T>(eEvent ev, T callback) where T : Delegate;
    void Login(in eAccountType type);
    oPlayerInfoPush PlayerInfo { get; }

    void SendProtobuf<T>(in eClientID id, in T body) where T : Google.Protobuf.IMessage;

    public delegate void fOnProtobuf<T>(T body) where T : Google.Protobuf.IMessage, new();
    void RegistProtobuf<T>(eServerID id, in fOnProtobuf<T> callback) where T : Google.Protobuf.IMessage, new();

    UInt64 Account();
}

interface IProtoCall
{
    public void Call(byte[] data, int offset, int size);
}
class ProtoCall<T> : IProtoCall where T : Google.Protobuf.IMessage, new()
{
    readonly IClient.fOnProtobuf<T> _Callback;
    public void Call(byte[] data, int offset, int size)
    {
        T body = new T();
        try
        {
            body.MergeFrom(data, offset, size);
            _Callback(body);
        }
        catch (InvalidProtocolBufferException e)
        {
            XApi.Error(e.Message);
        }
    }

    public ProtoCall(IClient.fOnProtobuf<T> callback)
    {
        _Callback = callback;
    }
}

class Client : IClient
{
#if UNITY_EDITOR
    const string ServerHost =
    "127.0.0.1";
    //"172.16.0.22";
    //"game.game-ai.com.cn";
    const int ServerPort = 18888;
#else
    const string ServerHost = "game.game-ai.com.cn";
    const int ServerPort = 18888;
#endif //UNITY_EDITOR

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IClient, Client>();
    }

    IUICommon _UICommon;
    GameObject _BtnLogin;
    GameObject _PanelCreatePlayer;

    XApi.INetwork.ITcpConnection _Connection;
    Dictionary<eServerID, IProtoCall> _ProtoCalls = new Dictionary<eServerID, IProtoCall>();

    XEventPool<IClient.eEvent> _EventPool = new XEventPool<IClient.eEvent>();


    oPlayerInfoPush _PlayerInfo;
    public oPlayerInfoPush PlayerInfo => _PlayerInfo;

    IGameSceneManager _GameSceneManager;
    UInt64 _Account;
    public bool Initialize()
    {
        this.Regist<Action<string, int>>(IClient.eEvent.Login, (string a, int b) =>
        {
            XApi.Debug($"{a}:{b}");
        });

        _EventPool.Trigger(IClient.eEvent.Login, "hello", 123);
        Application.targetFrameRate = 60;
        Application.runInBackground = true;
        return true;
    }

    public bool Launch()
    {
        _UICommon = Launcher.GetModule<IUICommon>();
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();

        RegistProtobuf<oNewPlayer>(eServerID.NewPlayer, OnCreateNewPlayer);
        RegistProtobuf<oPlayerInfoPush>(eServerID.PlayerInfoPush, OnPlayerInfoPush);
        RegistProtobuf<oCreatePlayerAsw>(eServerID.CreatePlayerAsw, OnCreatePlayerAsw);
        RegistProtobuf<oLoginAsw>(eServerID.LoginAsw, OnLoginAsw);
        return true;
    }

    public void Release()
    {

    }

    private void OnCreatePlayerAsw(oCreatePlayerAsw data)
    {
        if (data.Code == eError.NicknameExists)
        {
            XApi.Trace("NickName Exists");
        }
    }

    void OnLoginAsw(oLoginAsw data)
    {
        if (data.Code != eError.Succeed)
        {
            XApi.Error($"Login Failed Code {data.Code}");
            return;
        }
        _Account = data.Account;
        XApi.Trace($"Login Succeed Account {_Account} Token {data.Token}");
    }

    public UInt64 Account()
    {
        return _Account;
    }

    private void OnPlayerInfoPush(oPlayerInfoPush data)
    {
        _PlayerInfo = data;
        _GameSceneManager.AsyncLoadScene("GameScene");
    }

    public void OnCreateNewPlayer(oNewPlayer data)
    {
        _BtnLogin = GameObjectCommon.GetGameObjectByPath("Canvas/BtnLogin");
        _PanelCreatePlayer = GameObject.FindFirstObjectByType<Canvas>().transform.Find("PanelCreatePlayer").gameObject;
        _PanelCreatePlayer.transform.Find("BtnConfirm").GetComponent<Button>().onClick.AddListener(OnBtnConfirmClick);
        _BtnLogin.SetActive(false);
        _PanelCreatePlayer.SetActive(true);
    }

    public void OnBtnConfirmClick()
    {
        string nickName = _PanelCreatePlayer.transform.Find("InputName").GetComponent<TMP_InputField>().text;
        if (nickName.Length < 2 || nickName.Length > 10)
        {
            return;
        }

        oCreatePlayerReq req = new oCreatePlayerReq();
        req.Nickname = nickName;
        SendProtobuf(eClientID.CreatePlayerReq, req);
    }
    public string GetIDFA()
    {
        return SystemInfo.deviceUniqueIdentifier;
    }

    public void Regist<T>(IClient.eEvent ev, T callback) where T : Delegate
    {
        _EventPool.Regist<T>(ev, callback);
    }

    public void Login(in eAccountType type)
    {
        if (_Connection != null)
        {
            _Connection.Close();
            _Connection = null;
        }

        XApi.NetworkApi.CreateTcpConnection(ServerHost, ServerPort, (XApi.INetwork.ITcpConnection con) =>
        {
            if (con == null)
            {
                _UICommon.AlertUI("提示", "连接服务器失败", "确认");
                XApi.Error($"Connect Server {ServerHost}:{ServerPort} Falid");
                return;
            }

            XApi.Trace($"Connect Server {ServerHost}:{ServerPort} Succeed");

            con.SetConnectCallback((bool succeed, ITcpConnection c) => {
                if (succeed)
                {
                    _Connection = con;
                    _Connection.SetDisconnectCallback((XApi.INetwork.ITcpConnection con) =>
                    {
                        _UICommon.AlertUI("提示", "与服务器断开连接", "确认", () =>
                        {
                            Application.Quit();
                        });
                        _Connection = null;
                    });

                    _Connection.SetReceiveCallback((XApi.INetwork.ITcpConnection con, byte[] data, int offset, int size) =>
                    {
                        if (size < sizeof(Int32) + sizeof(UInt16))
                        {
                            return 0;
                        }

                        int len = (int)XConvert.BytesToInt32(data, offset, sizeof(Int32));
                        if (len < sizeof(Int32) + sizeof(UInt16))
                        {
                            _Connection.Close();
                            return 0;
                        }

                        if (len > size)
                        {
                            return 0;
                        }

                        eServerID id = (eServerID)XConvert.BytesToUInt16(data, offset + sizeof(Int32), sizeof(UInt16));
                        //XApi.Debug($"Receive Server Message ID {id.ToString()}");
                        if (_ProtoCalls.ContainsKey(id))
                        {
                            try
                            {
                                _ProtoCalls[id].Call(data, offset + sizeof(Int32) + sizeof(UInt16), (int)len - (sizeof(Int32) + sizeof(UInt16)));
                            }
                            catch (Exception ex)
                            {
                                XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                            }
                        }

                        return len;
                    });

                    oLoginReq req = new oLoginReq();
                    req.AccountType = eAccountType.Trial;
                    req.Unionid = GetIDFA();
                    SendProtobuf(eClientID.LoginReq, req);
                } else
                {
                    _UICommon.AlertUI("提示", "连接服务器失败", "确认");
                    XApi.Error($"Connect Server {ServerHost}:{ServerPort} Falid");
                }
            });
        });
    }

    public void SendProtobuf<T>(in eClientID id, in T body) where T : IMessage
    {
        if (_Connection == null)
        {
            XApi.Error($"do not connected server yet");
            return;
        }

        byte[] data = body.ToByteArray();
        byte[] temp1 = XConvert.UInt16ToBytes((UInt16)id);
        UInt32 size = (UInt32)data.Length + sizeof(UInt32) + sizeof(UInt16);
        byte[] temp2 = XConvert.UInt32ToBytes(size);

        _Connection.Send(temp2, 0, temp2.Length, false);
        _Connection.Send(temp1, 0, temp1.Length, false);
        _Connection.Send(data, 0, data.Length);
    }

    public void RegistProtobuf<T>(eServerID id, in IClient.fOnProtobuf<T> callback) where T : IMessage, new()
    {
        if (_ProtoCalls.ContainsKey(id))
        {
            XApi.Error($"Double regist proto id {id}");
            return;
        }

        _ProtoCalls.Add(id, new ProtoCall<T>(callback));
    }
}
