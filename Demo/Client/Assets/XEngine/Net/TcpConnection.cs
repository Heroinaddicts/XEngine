using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Threading;
using XUtils;
using static XEngine.XApi.INetwork;
using static XEngine.NetDefine;

namespace XEngine
{
    class TcpConnection : XApi.INetwork.ITcpConnection
    {
        static Dictionary<UInt64, TcpConnection> s_Dictionary = new Dictionary<UInt64, TcpConnection>();

        Socket _Socket;
        readonly UInt64 _Guid;
        readonly byte[] _RecvTemp = new byte[1024 * 128];
        readonly Network _Network;
        Flag _IsSending = new Flag();
        Flag _IsReving = new Flag();

        //callbacks
        Action<bool, ITcpConnection> _ConnectCallback = null;
        Action<ITcpConnection> _DisconnectCallback = null;
        Func<ITcpConnection, byte[], int, int, int> _RecvCallback = null;

        object _Context = null;

        SPSCBuffer _RecvBuffer = new SPSCBuffer(1024 * 1024 * 10);
        SPSCBuffer _SendBuffer = new SPSCBuffer(1024 * 1024 * 10);

        public static TcpConnection Create(Socket s, UInt64 guid, Network nw)
        {
            TcpConnection con = new TcpConnection(nw, s, guid);

            if (con.Initialize())
            {
                s_Dictionary.Add(guid, con);
                return con;
            }
            s.Close();
            return null;
        }

        TcpConnection(in Network network, in Socket s, in UInt64 guid)
        {
            _Network = network;
            _Socket = s;
            _Guid = guid;
        }

        public static void Clear()
        {
            foreach (var kv in s_Dictionary)
            {
                kv.Value.Close();
            }
        }

        public static void Release(TcpConnection con)
        {
            s_Dictionary.Remove(con.Guid);
            con.ReleaseSocket();
        }

        public static TcpConnection Query(UInt64 guid)
        {
            return s_Dictionary[guid];
        }

        private TcpConnection() { }

        void ReleaseSocket()
        {
            if (_Socket != null)
            {
                _Socket.Close();
                _Socket = null;
            }
        }

        public bool IsClosed()
        {
            return _Socket == null;
        }

        private bool Initialize()
        {
            bool ret = false;
            try
            {
                _Socket.BeginReceive(_RecvTemp, 0, _RecvTemp.Length, SocketFlags.None, this.ReceiveCallback, this);
                _IsReving._V = true;
                ret = true;
            }
            catch (Exception e)
            {
                ReleaseSocket();
                XApi.Error(e.ToString());
            }
            return ret;
        }

        void ReceiveCallback(IAsyncResult ar)
        {
            bool isDisconnect = false;
            try
            {
                int bytesRead = _Socket.EndReceive(ar);
                if (bytesRead > 0)
                {
                    _RecvBuffer.Write(_RecvTemp, 0, bytesRead);
                    _Network.PushEvent(eNetEventType.TcpRecv, this, CODE_SUCCESS);
                    _Socket.BeginReceive(_RecvTemp, 0, _RecvTemp.Length, SocketFlags.None, this.ReceiveCallback, this);
                }
                else
                {
                    isDisconnect = true;
                }
            }
            catch (SocketException e)
            {
                isDisconnect = true;
            }

            if (isDisconnect)
            {
                ReleaseSocket();
                _IsReving._V = false;
                lock (_IsSending)
                {
                    if (!_IsSending)
                    {
                        _Network.PushEvent(eNetEventType.TcpDisconnect, this, CODE_UNKNOWN);
                    }
                }
                XApi.Trace($"Connection {_Guid} closed");
            }
        }

        public void Close()
        {
            _DisconnectCallback?.Invoke(this);
            _DisconnectCallback = null;
            ReleaseSocket();
        }

        public void Send(byte[] data, int offset, int size, bool immediately)
        {
            if (null != _Socket)
            {
                _SendBuffer.Write(data, offset, size);
                if (immediately)
                {
                    _Network.PushNeedSendConnection(this);
                }
            }
        }

        void AsyncSendCallback(IAsyncResult ar)
        {
            bool isClose = false;
            try
            {
                int bytesSend = _Socket.EndSend(ar);
                if (bytesSend > 0)
                {
                    _SendBuffer.Out(bytesSend);
                    if (_SendBuffer.GetLength() > 0)
                    {
                        AsyncSend(true);
                    }
                    else
                    {
                        lock (_IsSending)
                        {
                            _IsSending._V = false;
                        }
                    }
                }
                else
                {
                    isClose = true;
                }
            }
            catch (SocketException ex)
            {
                XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                isClose = true;
            }
            catch (ObjectDisposedException ex)
            {
                XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                isClose = true;
            }
            catch (Exception ex)
            {
                XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                isClose = true;
            }

            if (isClose)
            {
                if (!Volatile.Read(ref _IsReving))
                {
                    ReleaseSocket();
                    _Network.PushEvent(eNetEventType.TcpDisconnect, this, CODE_UNKNOWN);
                }
            }
        }

        public void AsyncSend(bool through = false)
        {
            lock (_IsSending)
            {
                if (null == _Socket)
                {
                    return;
                }

                if (!_IsSending || through)
                {
                    try
                    {
                        if (_SendBuffer.GetLength() > 0)
                        {
                            _SendBuffer.Read(
                                (byte[] data, int offset, int len) =>
                                {
                                    _Socket.BeginSend(data, offset, len, SocketFlags.None, this.AsyncSendCallback, this);
                                    return 0;
                                }
                            );
                            _IsSending._V = true;
                        }
                    }
                    catch (SocketException e)
                    {
                        XApi.Error(e.ToString());
                        _IsSending._V = false;
                        if (!Volatile.Read(ref _IsReving))
                        {
                            ReleaseSocket();
                            _Network.PushEvent(eNetEventType.TcpDisconnect, this, CODE_UNKNOWN);
                        }
                    }
                }
            }
        }

        public void SetConnectCallback(Action<bool, ITcpConnection> callback)
        {
            _ConnectCallback = callback;
        }

        public void SetDisconnectCallback(Action<ITcpConnection> callback)
        {
            _DisconnectCallback = callback;
        }

        public void SetReceiveCallback(Func<ITcpConnection, byte[], int, int, int> callback)
        {
            _RecvCallback = callback;
        }

        public void DealRecv()
        {
            Int64 tick = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();

            while (!_Network.IsPause && _RecvBuffer.Read(
                (byte[] data, int offset, int len) =>
                {
                    return _RecvCallback(this, data, offset, len);
                }
            ))
            {
                if (DateTimeOffset.UtcNow.ToUnixTimeMilliseconds() - tick > 5 || _Network.IsPause)
                {
                    if (_RecvBuffer.GetLength() > 0)
                    {
                        _Network.PushEvent(eNetEventType.TcpRecv, this, CODE_SUCCESS);
                    }
                    break;
                }
            }
        }
        public void OnConnect(bool ret)
        {
            if (null != _ConnectCallback)
            {
                _ConnectCallback(ret, this);
            }
        }

        public void OnDisconnect()
        {
            _DisconnectCallback?.Invoke(this);
            _DisconnectCallback = null;
        }

        public void SetContext<T>(in T context)
        {
            _Context = context;
        }

        public T GetContext<T>()
        {
            return (T)_Context;
        }

        public UInt64 Guid
        {
            get { return _Guid; }
        }
    }
}
