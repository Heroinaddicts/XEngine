using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using XUtils;
//using static PlasticPipe.PlasticProtocol.Client.ConnectionCreator.PlasticProtoSocketConnection;
using static XEngine.NetDefine;
using static XEngine.XApi.INetwork;

namespace XEngine
{
    class Network : XApi.INetwork
    {
        SPSCQueue<NetEvent> _EventQueue = new SPSCQueue<NetEvent>();
        bool _Pause = false;
        List<TcpConnection> _ConnectionNeedSend = new List<TcpConnection>();
        public void PushNeedSendConnection(TcpConnection connection)
        {
            if (!_ConnectionNeedSend.Contains(connection))
            {
                _ConnectionNeedSend.Add(connection);
            }
        }

        bool _IsClear = false;

        public bool IsPause { get { return _Pause; } }
        public void PushEvent(eNetEventType ev, ITcpSocket s, int code, Socket sock = null)
        {
            var e = NetDefine.s_EventPool.Get();
            e._Type = ev;
            e._S = s;
            e._Code = code;
            e._Socket = sock;

            _EventQueue.Push(e);
        }
        public void PushEvent(Udper udper, in byte[] buff, in int size, in string ip, in int port)
        {
            var e = NetDefine.s_EventPool.Get();
            e._Type = eNetEventType.UdpRecv;
            e._Udper = udper;
            e._UdpReceiveBuff = buff;
            e._UdpReceiveSize = size;
            e._UdpRemoteAddress = new IPv4Address(ip, port);
            _EventQueue.Push(e);
        }

        public void CreateTcpConnection(string host, int port, Action<ITcpConnection> ret)
        {
            Socket s = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp)
            {
                DualMode = true,
                NoDelay = true,
                ReceiveBufferSize = 65535,
                SendBufferSize = 65535,
                SendTimeout = 5000,
                ReceiveTimeout = 5000
            };

            try
            {
                IPAddress[] addrs = Dns.GetHostAddresses(host);

                foreach (var addr in addrs)
                {
                    if (addr.AddressFamily != AddressFamily.InterNetwork &&
                        addr.AddressFamily != AddressFamily.InterNetworkV6)
                        continue;

                    var remoteEP = new IPEndPoint(addr, port);
                    try
                    {
                        s.BeginConnect(remoteEP, (ar) =>
                        {
                            var ev = NetDefine.s_EventPool.Get();
                            ev._Type = NetDefine.eNetEventType.TcpConnect;
                            ev._CRet = ret;
                            try
                            {
                                s.EndConnect(ar);
                                TcpConnection con = TcpConnection.Create(s, NetGUID.Generator(), this);
                                if (null != con)
                                {
                                    ev._Code = NetDefine.CODE_SUCCESS;
                                    ev._S = con;
                                }
                                else
                                {
                                    ev._Code = NetDefine.CODE_UNKNOWN;
                                }
                            }
                            catch (SocketException e)
                            {
                                ev._Code = e.ErrorCode;
                                s.Close();
                                s = null;
                            }
                            _EventQueue.Push(ev);
                        }, s);
                        break;
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"BeginConnect {remoteEP}:{ex.Message}");
                    }
                }
            }
            catch (SocketException ex)
            {
                s.Close();
                s = null;
                XApi.Error("Unexpected exception: " + ex.Message);
                ret(null);
            }
        }

        public ITcpServer LaunchTcpServer(string host, int port)
        {
            XApi.Trace($"Launch TcpServer {host}:{port}");
            return TcpServer.Create(NetGUID.Generator(), host, port, this);
        }

        public void Pause()
        {
            _Pause = true;
        }
        public void Resume()
        {
            _Pause = false;
        }
        public void Clear()
        {
            TcpConnection.Clear();
            TcpServer.Clear();
            _IsClear = true;
        }
        public void Update()
        {
            for (int i = 0; i < _ConnectionNeedSend.Count; i++)
            {
                var c = _ConnectionNeedSend[i];
                if (!c.IsClosed())
                {
                    c.AsyncSend();
                }
            }
            _ConnectionNeedSend.Clear();

            Int64 tick = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
            NetEvent ev = null;
            while (!_Pause && _EventQueue.Pull(out ev))
            {
                switch (ev._Type)
                {
                    case eNetEventType.TcpConnect:
                        {
                            if (ev._CRet != null)
                            {
                                TcpConnection con = (ev._S as TcpConnection);
                                ev._CRet(con);
                                if (null != con)
                                {
                                    con.OnConnect(true);
                                }
                            }
                        }
                        break;
                    case eNetEventType.TcpDisconnect:
                        {
                            TcpConnection con = (ev._S as TcpConnection);
                            con.OnDisconnect();
                            TcpConnection.Release(con);
                        }
                        break;
                    case eNetEventType.TcpRecv:
                        {
                            TcpConnection con = (ev._S as TcpConnection);
                            con.DealRecv();
                        }
                        break;
                    case eNetEventType.TcpAccept:
                        {
                            TcpServer ts = (ev._S as TcpServer);
                            ts.OnAccept(ev._Socket);
                        }
                        break;
                    case eNetEventType.UdpRecv:
                        {
                            if (!ev._Udper.IsClosed)
                                try
                                {
                                    ev._Udper.Receive(ev._Udper, ev._UdpRemoteAddress, ev._UdpReceiveBuff, 0, ev._UdpReceiveSize);
                                }
                                catch (Exception ex)
                                {
                                    XApi.Error(ex.Message);
                                }
                            break;
                        }
                    default:
                        break;
                }

                s_EventPool.Return(ev);
                if (DateTimeOffset.UtcNow.ToUnixTimeMilliseconds() - tick > 5)
                {
                    break;
                }
            }
        }

        public IUdper CreateUdper()
        {
            try
            {
                Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                Udper udper = new Udper(sock, this);
                return udper;
            }
            catch (Exception ex)
            {
                XApi.Error(ex.Message);
                return null;
            }
        }
    }
}
