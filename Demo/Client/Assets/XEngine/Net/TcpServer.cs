using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using XUtils;
using static XEngine.NetDefine;

namespace XEngine
{
    class TcpServer : XApi.INetwork.ITcpServer
    {
        string _Host;
        int _Port;
        UInt64 _Guid;
        object _Context;
        Action<XApi.INetwork.ITcpServer, XApi.INetwork.ITcpConnection> _AcceptCallback;
        Socket _Socket;
        Network _Network;

        static HashSet<TcpServer> s_ServerMap = new HashSet<TcpServer>();

        static XPool<TcpServer> s_Pool = new XPool<TcpServer>(
            () => new TcpServer(),
            (TcpServer t) =>
            {
                t._Guid = 0;
                t._Context = null;
                t._AcceptCallback = null;
            },
            16
        );

        TcpServer() { }

        public static TcpServer Create(UInt64 guid, in string host, in int port, Network nw)
        {
            TcpServer server = s_Pool.Get();
            server._Host = host;
            server._Port = port;
            server._Guid = guid;
            server._Context = null;
            server._AcceptCallback = null;
            server._Network = nw;
            if (server.Launch())
            {
                return server;
            }

            s_ServerMap.Add(server);
            s_Pool.Return(server);
            return null;
        }

        public static void Release(TcpServer sv)
        {
            sv.Close();
            s_ServerMap.Remove(sv);
        }

        public static void Clear()
        {
            foreach (var s in s_ServerMap)
            {
                s.Close();
                s_Pool.Return(s);
            }
        }

        public void Close()
        {
            if (null != _Socket)
            {
                _Socket.Close();
                _Socket = null;
            }
        }

        public bool Launch()
        {
            try
            {
                _Socket = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp)
                {
                    DualMode = true,
                    NoDelay = true
                };
                IPAddress addr = IPAddress.Parse(_Host);

                _Socket.Bind(new IPEndPoint(addr, _Port));
                _Socket.Listen(100);

                return AsyncAccept();
            }
            catch (SocketException e)
            {
                XApi.Error(e.Message);
                return false;
            }
        }

        bool AsyncAccept()
        {
            try
            {
                _Socket.BeginAccept(this.AsyncAcceptCallback, null);
                return true;
            }
            catch (SocketException e)
            {
                XApi.Error(e.Message);
                return false;
            }
        }

        void AsyncAcceptCallback(IAsyncResult ar)
        {
            try
            {
                Socket sock = _Socket.EndAccept(ar);
                _Network.PushEvent(eNetEventType.TcpAccept, this, CODE_SUCCESS, sock);
            }
            catch (SocketException e)
            {
                XApi.Error(e.Message);
            }
            finally
            {
                AsyncAccept();
            }
        }

        public void OnAccept(Socket sock)
        {
            if (sock == null)
            {
                XApi.Error("TcpServer::OnAccept socket is null");
                return;
            }

            TcpConnection con = TcpConnection.Create(sock, NetGUID.Generator(), _Network);
            if (con != null && _AcceptCallback != null)
            {
                _AcceptCallback(this, con);
            }
            else
            {
                sock.Close();
            }
        }

        public UInt64 Guid
        {
            get { return _Guid; }
        }

        public T GetContext<T>()
        {
            return (T)_Context;
        }

        public void SetContext<T>(in T context)
        {
            _Context = context;
        }

        public void SetOnConnectionCallback(Action<XApi.INetwork.ITcpServer, XApi.INetwork.ITcpConnection> callback)
        {
            _AcceptCallback = callback;
        }
    }
}
