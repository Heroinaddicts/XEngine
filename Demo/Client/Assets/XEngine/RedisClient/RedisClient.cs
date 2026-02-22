#if UNITY_SERVER || UNITY_EDITOR ||  UNITY_STANDALONE
using StackExchange.Redis;
using System;

namespace XEngine
{
    class RedisSession : XApi.IRedisClient.IRedisSession
    {
        static Int64 s_SessionIndexOffset = 0;

        readonly ConnectionMultiplexer _Con;
        readonly IDatabase _DB;
        readonly Int64 _Index;

        public RedisSession(ConnectionMultiplexer con, IDatabase db)
        {
            _Con = con;
            _DB = db;
            _Index = s_SessionIndexOffset++;
        }

        public void AsyncGet(string key, Action<bool, string> callback)
        {
            XApi.TaskApi.StartTask(XApi.ITask.eTaskType.OrderedQueue,
                (XApi.ITask.ITaskContext context) =>
                {
                    try
                    {
                        string value = _DB.StringGet(key);
                        context.SetContext(value);
                        context.SetCode(XApi.IRedisClient.REDIS_CODE_SUCCEED);
                    }
                    catch (RedisException ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                        context.SetCode(XApi.IRedisClient.REDIS_CODE_ERROR_UNKNOWN);
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                        context.SetCode(XApi.IRedisClient.REDIS_CODE_ERROR_UNKNOWN);
                    }
                },
                (XApi.ITask.ITaskContext context) =>
                {
                    if (context.GetCode() == XApi.IRedisClient.REDIS_CODE_SUCCEED)
                    {
                        callback(true, context.GetContext<string>());
                    }
                    else
                    {
                        callback(false, null);
                    }
                }
            );
        }

        public void AsyncSet(string key, string value, Action<bool> callback)
        {
            XApi.TaskApi.StartTask(XApi.ITask.eTaskType.OrderedQueue,
                (XApi.ITask.ITaskContext context) =>
                {
                    try
                    {
                        bool ret = _DB.StringSet(key, value);
                        context.SetCode(ret ? XApi.IRedisClient.REDIS_CODE_SUCCEED : XApi.IRedisClient.REDIS_CODE_ERROR_UNKNOWN);
                    }
                    catch (RedisException ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                        context.SetCode(XApi.IRedisClient.REDIS_CODE_ERROR_UNKNOWN);
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                        context.SetCode(XApi.IRedisClient.REDIS_CODE_ERROR_UNKNOWN);
                    }
                },
                (XApi.ITask.ITaskContext context) =>
                {
                    if (context.GetCode() == XApi.IRedisClient.REDIS_CODE_SUCCEED)
                    {
                        callback(true);
                    }
                    else
                    {
                        callback(false);
                    }
                }
            );
        }
    }

    class RedisClient : XApi.IRedisClient
    {
        public void ConnectRedis(string host, int port, string password, Action<XApi.IRedisClient.IRedisSession> callback)
        {
            XApi.TaskApi.StartTask(XApi.ITask.eTaskType.SeparateThread,
                (XApi.ITask.ITaskContext context) =>
                {
                    try
                    {
                        ConfigurationOptions options = new ConfigurationOptions
                        {
                            EndPoints = { $"{host}:{port}" },
                            Password = password,
                            ConnectRetry = 3,
                            ConnectTimeout = 3000,
                            SyncTimeout = 2000,
                            AbortOnConnectFail = false
                        };

                        ConnectionMultiplexer con = ConnectionMultiplexer.Connect($"{host}:{port}");
                        IDatabase db = con.GetDatabase();
                        context.SetContext(new RedisSession(con, db));
                    }
                    catch (RedisException ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                    }
                },
                (XApi.ITask.ITaskContext context) =>
                {
                    RedisSession session = context.GetContext<RedisSession>();
                    callback(session);
                }
            );
        }
    }
}
#endif //UNITY_SERVER || UNITY_EDITOR
