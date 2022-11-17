using Google.Protobuf;
using ProtoAnalysis;
using System;
using System.Collections;
using System.Collections.Generic;
using TCore.Api;
using TLib;
using UnityEngine;

public enum eConnectionEventType
{
    Connected,
    ConnectFailed,
    Disconnected
}

public enum eConnectionType
{
    GameSLB,//负载均衡
    GameGate,//GATE
}

public class Connection : iTcpSession
{
    public readonly eConnectionType _type;
    public Connection(in eConnectionType type)
    {
        _type = type;
    }


    public override void OnConnect(in bool r)
    {
        if (r)
            Debug.Log("connect success type " + _type);
        else
            Debug.LogError("connect failed type " + _type);


        ConnectionManager.OnConnect(r, this);
    }

    public override void OnDisconnected()
    {
        Debug.Log("OnDisconnected type " + _type);
        ConnectionManager.OnDisconnected(this);
    }

    const int header = sizeof(Int32) + sizeof(Int16);

    public override int OnRecv(in byte[] data, in int offset, in int length)
    {
        if (length < header)
        {
            return 0;
        }

        int len = SafeMemory.BytesToInt32(data, offset);
        if (len > length)
        {
            return 0;
        }

        int id = SafeMemory.BytesToInt16(data, offset + sizeof(Int32));
        ConnectionManager.TriggerProto(this, id, data, offset + sizeof(Int32) + sizeof(Int16), len - (sizeof(Int32) + sizeof(Int16)));
        return len;
    }

    public void SendProto<T>(in eCPID id, in T body) where T : IMessage
    {
        byte[] data = body.ToByteArray();

        int len = sizeof(Int32) + sizeof(Int16) + data.Length;

        Send(SafeMemory.Int32ToBytes(len), 0, sizeof(Int32), false);
        Send(SafeMemory.Int16ToBytes((Int16)id), 0, sizeof(Int16), false);
        Send(data, 0, data.Length);
    }
}
