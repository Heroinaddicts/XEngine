using System;
using System.Runtime.CompilerServices;
using UnityEngine;
using static XEngine.XApi.INetwork.IUdper;

namespace XEngine
{
    public static class XApi
    {
        public interface INetwork
        {
            public struct IPv4 : IEquatable<IPv4>
            {
                public readonly byte _B1, _B2, _B3, _B4;
                public readonly string _String;

                public IPv4(in byte b1, in byte b2, in byte b3, in byte b4)
                {
                    _B1 = b1;
                    _B2 = b2;
                    _B3 = b3;
                    _B4 = b4;
                    _String = $"{_B1}.{_B2}.{_B3}.{_B4}";
                }

                public IPv4(in string ip)
                {
                    if (string.IsNullOrEmpty(ip))
                        throw new ArgumentNullException(nameof(ip), "IP string can not be null");

                    string[] parts = ip.Split('.');
                    if (parts.Length != 4)
                        throw new FormatException($"{ip} ipv4 string format error");

                    try
                    {
                        _B1 = byte.Parse(parts[0]);
                        _B2 = byte.Parse(parts[1]);
                        _B3 = byte.Parse(parts[2]);
                        _B4 = byte.Parse(parts[3]);
                    }
                    catch (OverflowException ex)
                    {
                        throw new FormatException($"{ip} ipv4 string format error");
                    }
                    catch (FormatException ex)
                    {
                        throw new FormatException($"{ip} ipv4 string format error");
                    }

                    _String = $"{_B1}.{_B2}.{_B3}.{_B4}";
                }

                public override string ToString() => _String;
                public bool Equals(IPv4 other)
                {
                    return _B1 == other._B1 &&
                           _B2 == other._B2 &&
                           _B3 == other._B3 &&
                           _B4 == other._B4;
                }

                public override bool Equals(object obj) => obj is IPv4 other && Equals(other);

                public override int GetHashCode()
                {
                    return (_B1 << 24) | (_B2 << 16) | (_B3 << 8) | _B4;
                }

                public static bool operator ==(IPv4 left, IPv4 right) => left.Equals(right);
                public static bool operator !=(IPv4 left, IPv4 right) => !left.Equals(right);
            }

            public struct IPv4Address : IEquatable<IPv4Address>
            {
                readonly IPv4 _IP;
                readonly int _Port;
                readonly string _String;

                public IPv4 IP => _IP;
                public int Port => _Port;
                public string String => _String;

                public IPv4Address(in string ip, in int port)
                {
                    _IP = new IPv4(ip);
                    _Port = port;
                    _String = $"{ip}:{_Port}";
                }

                public IPv4Address(in IPv4 ip, in int port)
                {
                    _IP = ip;
                    _Port = port;
                    _String = $"{ip.ToString()}:{_Port}";
                }

                public override string ToString() => _String;
                public bool Equals(IPv4Address other)
                {
                    return _IP.Equals(other._IP) && _Port == other._Port;
                }
                public override bool Equals(object obj)
                {
                    return obj is IPv4Address other && Equals(other);
                }
                public override int GetHashCode()
                {
                    return _IP.GetHashCode() ^ _Port.GetHashCode();
                }
                public static bool operator ==(IPv4Address left, IPv4Address right) => left.Equals(right);
                public static bool operator !=(IPv4Address left, IPv4Address right) => !left.Equals(right);
            }

            public interface IUdper
            {
                void Bind(in string ip, in int port);
                public delegate void fUdpReceive(IUdper udper, in IPv4Address address, in byte[] data, in int offset, in int length);
                void SendTo(in string host, in int port, in byte[] data, in int offset, in int length);

                public void SendTo(in IPv4Address address, in byte[] data, in int offset, in int length)
                {
                    SendTo(address.IP.ToString(), address.Port, data, offset, length);
                }

                void SetUdpReciver(in fUdpReceive reciver);
                void StartRecive();
                void Close();
            }
            public IUdper CreateUdper();

            public interface ITcpSocket
            {
                public UInt64 Guid { get; }

                public void SetContext<T>(in T context);
                public T GetContext<T>();
            }

            public interface ITcpConnection : ITcpSocket
            {
                public void Send(byte[] data, int offset, int size, bool immediately = true);
                public void SetReceiveCallback(System.Func<ITcpConnection, byte[], int, int, int> callback);
                public void SetConnectCallback(System.Action<bool, ITcpConnection> callback);
                public void SetDisconnectCallback(System.Action<ITcpConnection> callback);
                public void Close();
            }

            public interface ITcpServer : ITcpSocket
            {
                public void SetOnConnectionCallback(System.Action<ITcpServer, ITcpConnection> callback);
                public void Close();
            }

            public void CreateTcpConnection(string host, int port, Action<ITcpConnection> ret);
            public ITcpServer LaunchTcpServer(string host, int port);
            public void Pause();
            public void Resume();
            public void Clear();
        }

        static Network s_NetworkInstance = null;
        public static INetwork NetworkApi
        {
            get
            {
                if (s_NetworkInstance == null)
                {
                    s_NetworkInstance = new Network();
                }

                return s_NetworkInstance;
            }
        }

        public interface ILog
        {
            public void Debug(string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0);
            public void Trace(string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0);
            public void Error(string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0);
        }

        static ILog s_LogInstance = null;
        public static ILog LogApi
        {
            get
            {
                if (s_LogInstance == null)
                {
                    s_LogInstance = new Logger();
                }
                return s_LogInstance;
            }
        }

        public static void Debug(string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0) { LogApi.Debug(log, file, line); }
        public static void Trace(string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0) { LogApi.Trace(log, file, line); }
        public static void Error(string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0) { LogApi.Error(log, file, line); }

        public static string EscapeSql(string input)
        {
            return input?.Replace("'", "''") ?? string.Empty;
        }

#if UNITY_SERVER || UNITY_EDITOR || UNITY_STANDALONE
        public interface IMysqlClient
        {

            public const int MYSQL_CODE_SUCCEED = 0;
            public const int MYSQL_CODE_ERROR_UNKNOWN = -1;
            public interface IMysqlTable
            {
                byte[] GetBlob(in int row, in int column);
                bool GetBool(in int row, in int column);
                float GetFloat(in int row, in int column);
                int GetInt32(in int row, in int column);
                Int64 GetInt64(in int row, in int column);
                UInt64 GetUInt64(in int row, in int column);
                string GetString(in int row, in int column);

                int RowCount();
            }

            public interface IMysqlSession
            {
                void AsyncQuery(string sql, Action<Int64, IMysqlTable> ret);
                IMysqlTable SyncQuery(string sql);
                void Close();
            }

            public void Connect(string host, int port, string user, string passwd, string db, Action<IMysqlSession> callback);
        }

        static IMysqlClient s_MysqlClient = null;
        public static IMysqlClient MysqlClientApi
        {
            get
            {
                if (s_MysqlClient == null)
                {
                    s_MysqlClient = new MysqlClient();
                }
                return s_MysqlClient;
            }
        }

        public interface IRedisClient
        {
            public const int REDIS_CODE_SUCCEED = 0;
            public const int REDIS_CODE_ERROR_UNKNOWN = -1;
            public interface IRedisSession
            {
                void AsyncSet(string key, string value, Action<bool> callback);
                void AsyncGet(string key, Action<bool, string> callback);
            }

            void ConnectRedis(string host, int port, string password, Action<IRedisSession> callback);
        }

        static IRedisClient s_RedisClient = null;
        public static IRedisClient RedisClientApi
        {
            get
            {
                if (s_RedisClient == null)
                {
                    s_RedisClient = new RedisClient();
                }
                return s_RedisClient;
            }
        }
#endif //UNITY_SERVER

        public interface ITask
        {

            public const int InvaildMask = 0;
            public enum eTaskType
            {
                OrderedQueue = 0,  //丢入队列里排队等待线程池处理执行
                SeparateThread = 1 //立即启动新线程处理
            }

            public interface ITaskContext
            {
                public Int64 GetCode();
                public T GetContext<T>();

                public void SetCode(Int64 code);
                public void SetContext<T>(T context);
            }

            public void StartTask(eTaskType taskType, Action<ITaskContext> task, Action<ITaskContext> callback, int mask = InvaildMask);

            public interface ITaskGroup
            {
                public void AddTask(eTaskType taskType, Action<ITaskContext> task, Action<ITaskContext> callback);
            }

            public ITaskGroup CreateTaskGroup(Action completedCallback);
        }

        static Task s_Task = null;
        public static ITask TaskApi
        {
            get
            {
                if (s_Task == null)
                {
                    s_Task = new Task();
                }
                return s_Task;
            }
        }

        public interface ITimer
        {
            public enum eState
            {
                START,
                BEAT,
                END
            }

            public const int Unlimited = 0;

            public delegate void fTimer(in eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder);
            public void StartTimer(in fTimer fun, in object obj, in object context, in int delay, in int count, in int interval, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0); //FixedTimer, delay和interval的单位毫秒
            public void StopTimer(in fTimer fun, in object obj);
            public bool IsTimerExists(in fTimer fun, in object obj);
        }

        static Timer s_Timer = null;
        public static ITimer TimerApi
        {
            get
            {
                if (s_Timer == null)
                {
                    s_Timer = new Timer();
                }
                return s_Timer;
            }
        }

        public static void Update()
        {
            s_Task?.Update();
            s_Timer?.Update();
        }

        public static void FixedUpdate()
        {
            s_NetworkInstance?.Update();
        }
    }
}
