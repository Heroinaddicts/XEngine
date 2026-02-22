using System;
using System.Net;
using System.Net.Sockets;
using XUtils;
using static XEngine.XApi.INetwork;

namespace XEngine
{
    static class NetGUID {
        static UInt64 _GuidOffset = XRandom.RandomUInt64();

        public static UInt64 Generator() {
            return _GuidOffset ++;
        }
    }

    static class NetDefine
    {
        public const int UDP_RECIVE_BUFF_SIZE = 1472;

        public enum eNetEventType
        {
            Unknown = -1,
            TcpAccept,
            TcpConnect,
            TcpRecv,
            TcpDisconnect,
            UdpRecv
        }

        public const int CODE_UNKNOWN = -1;
        public const int CODE_SUCCESS = 0;

        public class NetEvent
        {
            public eNetEventType _Type;
            public ITcpSocket _S;
            public Socket _Socket;
            public int _Code;
            public Action<ITcpConnection> _CRet;
            public Action<ITcpServer> _SRet;
            public Udper _Udper;
            public int _UdpReceiveSize;
            public byte[] _UdpReceiveBuff;
            public IPv4Address _UdpRemoteAddress;
        }

        public static readonly XPool<NetEvent> s_EventPool = new XPool<NetEvent>(
            () => new NetEvent(), 
            (NetEvent t) =>
            {
                t._Type = eNetEventType.Unknown;
                t._S = null;
                t._Code = CODE_UNKNOWN;
                t._CRet = null;
                t._SRet = null;
            }, 
            1024
        );
        public class Flag
        {
            public bool _V = false;

            public static implicit operator bool(Flag f)
            {
                return f._V;
            }
        }

        public static IPEndPoint ParseEndPoint(in string ip, in int port)
        {
            if (string.IsNullOrWhiteSpace(ip))
                throw new ArgumentNullException(nameof(ip), "IP地址字符串不能为空");
            if (!IPAddress.TryParse(ip, out IPAddress ipAddress))
                throw new FormatException($"无效的IP地址格式: {ip}");
            if (port < IPEndPoint.MinPort || port > IPEndPoint.MaxPort)
                throw new ArgumentOutOfRangeException(
                    nameof(port),
                    $"端口号必须在{IPEndPoint.MinPort}到{IPEndPoint.MaxPort}之间，当前值: {port}"
                );
            return new IPEndPoint(ipAddress, port);
        }
    }
}
