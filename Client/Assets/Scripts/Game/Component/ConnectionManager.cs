using Google.Protobuf;
using ProtoAnalysis;
using System.Collections;
using System.Collections.Generic;
using TNet;
using UnityEngine;

public static class ConnectionManager
{
    static IDictionary<eConnectionType, Connection> _connetions = new Dictionary<eConnectionType, Connection>();

    static ConnectionManager()
    {
        RegisterConnectionEvent(OnConnectionEvent);
    }

    static void OnConnectionEvent(in eConnectionEventType type, in Connection connection)
    {
        switch(type)
        {
            case eConnectionEventType.Connected:
                {
                    Connection con;
                    if (_connetions.TryGetValue(connection._type, out con))
                    {
                        Debug.LogError("connection already exists, close it now, type " + connection._type);
                        con.Close();
                        _connetions.Remove(connection._type);
                    }

                    _connetions.Add(connection._type, connection);
                    break;
                }
            case eConnectionEventType.ConnectFailed:
                {
                    break;
                }
            case eConnectionEventType.Disconnected:
                {
                    Connection con;
                    if (!_connetions.TryGetValue(connection._type, out con))
                    {
                        Debug.LogError("connection does not exists, type " + connection._type);
                    }
                    else
                    {
                        _connetions.Remove(connection._type);
                    }
                    break;
                }
        }
    }

    public static void Connect(in eConnectionType type, in string host, in int port)
    {
        Connection con;
        if (_connetions.TryGetValue(type, out con))
        {
            Debug.LogError("connection already exists, close it now, type " + type);
            con.Close();
            _connetions.Remove(type);
        }

        con = new Connection(type);
        NetWorker.LaunchTcpClient(con, host, port);
    }

    public static Connection GetCon(in eConnectionType type)
    {
        Connection con;
        if (_connetions.TryGetValue(type, out con))
        {
            return con;
        }

        return null;
    }


    public delegate void delegate_event(in eConnectionEventType type, in Connection connection);
    static ISet<delegate_event> _events = new HashSet<delegate_event>();
    public static void RegisterConnectionEvent(in delegate_event ev)
    {
        if (!_events.Contains(ev))
        {
            _events.Add(ev);
        }
    }

    public static void UnregisterEvent(in delegate_event ev)
    {
        if (_events.Contains(ev))
        {
            _events.Remove(ev);
        }
    }

    public static void OnConnect(in bool ret, in Connection con)
    {
        foreach (delegate_event ev in _events)
        {
            ev(ret ? eConnectionEventType.Connected : eConnectionEventType.ConnectFailed, con);
        }
    }

    public static void OnDisconnected(in Connection con)
    {
        foreach (delegate_event ev in _events)
        {
            ev(eConnectionEventType.Disconnected, con);
        }
    }

    static IDictionary<int, iProto<Connection>> _messagers = new Dictionary<int, iProto<Connection>>();
    public static void RegisterProto<T>(in eSPID id, in tProto<T, Connection>.call caller) where T : IMessage, new()
    {
        if (_messagers.ContainsKey((int)id))
        {
            Debug.LogError("message call already exists, id : " + id.ToString());
        }
        else
        {
            _messagers.Add((int)id, new tProto<T, Connection>(caller));
        }
    }

    public static void UnregisterProto(in eSPID id)
    {
        if (_messagers.ContainsKey((int)id))
        {
            _messagers.Remove((int)id);
        }
    }

    public static void TriggerProto(in Connection c, in int id, in byte[] data, in int offset, in int len)
    {
        iProto<Connection> message;
        if (_messagers.TryGetValue(id, out message))
        {
            message.Deserialization(c, data, offset, len);
        }
    }

    public static void SendProto<T>(in eConnectionType _type, in eCPID id, in T body) where T: IMessage
    {
        if(_connetions.ContainsKey(_type))
        {
            _connetions[_type].SendProto<T>(id, body);
        }
        else
        {
            Debug.LogError("Connection InValid! type = " + _type.ToString());
        }
    }
}
