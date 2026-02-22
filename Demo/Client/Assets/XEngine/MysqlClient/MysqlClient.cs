#if UNITY_SERVER || UNITY_EDITOR || UNITY_STANDALONE
using MySqlConnector;
using System;

namespace XEngine
{
    class MysqlClient : XApi.IMysqlClient
    {
        public void Connect(string host, int port, string user, string passwd, string db, Action<XApi.IMysqlClient.IMysqlSession> callback)
        {
            XApi.TaskApi.StartTask(
                XApi.ITask.eTaskType.SeparateThread,
                (XApi.ITask.ITaskContext context) =>
                {
                    try
                    {
                        MySqlConnection con = new MySqlConnection($"host={host};port={port};database={db};user={user};password={passwd}");
                        if (con != null)
                        {
                            con.Open();
                            context.SetContext<MySqlConnection>(con);
                            context.SetCode(0);
                            XApi.Trace($"Connect mysql success {host}:{port}->{db}");
                        }
                    }
                    catch (MySqlException e)
                    {
                        XApi.Error($"Connect mysql error {e.Number} : {e.Message}");
                        context.SetCode(e.Number);
                    }
                },
                (XApi.ITask.ITaskContext context) =>
                {
                    if (context.GetContext<MySqlConnection>() != null)
                    {
                        callback(new MysqlSession(context.GetContext<MySqlConnection>()));
                    }
                    else
                    {
                        callback(null);
                    }
                }
            );
        }
    }
}
#endif //UNITY_SERVER || UNITY_EDITOR