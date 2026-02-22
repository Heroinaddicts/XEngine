#if UNITY_SERVER || UNITY_EDITOR || UNITY_STANDALONE
using MySqlConnector;
using System;
using System.Data;
using static XEngine.XApi;
using static XEngine.XApi.ITimer;

namespace XEngine
{
    class MysqlTable : XApi.IMysqlClient.IMysqlTable
    {
        public readonly DataTable _DataTable = new DataTable();

        public byte[] GetBlob(in int row, in int column)
        {
            return _DataTable.Rows[row][column] as byte[];
        }

        public bool GetBool(in int row, in int column)
        {
            return Convert.ToBoolean(_DataTable.Rows[row][column]);
        }

        public float GetFloat(in int row, in int column)
        {
            return (float)Convert.ToDouble(_DataTable.Rows[row][column]);
        }

        public Int32 GetInt32(in int row, in int column)
        {
            return Convert.ToInt32(_DataTable.Rows[row][column]);
        }

        public Int64 GetInt64(in int row, in int column)
        {
            return Convert.ToInt64(_DataTable.Rows[row][column]);
        }

        public UInt64 GetUInt64(in int row, in int column)
        {
            return Convert.ToUInt64(_DataTable.Rows[row][column]);
        }

        public string GetString(in int row, in int column)
        {
            return _DataTable.Rows[row][column] as string;
        }

        public int RowCount()
        {
            return _DataTable.Rows.Count;
        }
    }

    class MysqlSession : XApi.IMysqlClient.IMysqlSession
    {
        MySqlConnection _Con = null;

        public MysqlSession(MySqlConnection con)
        {
            _Con = con;
            //XApi.TimerApi.LauncherTimer(this.HeartBeat, this, this, 60, XApi.ITimer.Unlimited, 60);
        }

        ~MysqlSession()
        {
            //XApi.TimerApi.StopFixedStep(this.HeartBeat, this);
        }

        void HeartBeat(in eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
        {
            if (state == eState.BEAT)
            {
                //这里要补心跳
            }
        }

        public void Close()
        {
            _Con.Close();
            _Con = null;
        }

        public void AsyncQuery(string sql, Action<Int64, XApi.IMysqlClient.IMysqlTable> ret)
        {
            XApi.Debug(sql);
            XApi.TaskApi.StartTask(XApi.ITask.eTaskType.OrderedQueue,
                (XApi.ITask.ITaskContext context) =>
                {
                    try
                    {
                        using (MySqlDataAdapter adapter = new MySqlDataAdapter(sql, _Con))
                        {
                            MysqlTable tab = new MysqlTable();
                            adapter.Fill(tab._DataTable);
                            context.SetContext(tab);
                            context.SetCode(IMysqlClient.MYSQL_CODE_SUCCEED);
                        }
                    }
                    catch (MySqlException ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                        context.SetCode(ex.Number);
                    }
                    catch (InvalidCastException ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                        context.SetCode(IMysqlClient.MYSQL_CODE_ERROR_UNKNOWN);
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                        context.SetCode(IMysqlClient.MYSQL_CODE_ERROR_UNKNOWN);
                    }
                },
                (XApi.ITask.ITaskContext context) =>
                {
                    ret(context.GetCode(), context.GetContext<XApi.IMysqlClient.IMysqlTable>());
                },
                1
            );
        }

        public IMysqlClient.IMysqlTable SyncQuery(string sql)
        {
            try
            {
                using (MySqlDataAdapter adapter = new MySqlDataAdapter(sql, _Con))
                {
                    MysqlTable tab = new MysqlTable();
                    adapter.Fill(tab._DataTable);
                    return tab;
                }
            }
            catch (MySqlException ex)
            {
                XApi.Error($"{ex.Message}\n{ex.StackTrace}");
            }
            catch (InvalidCastException ex)
            {
                XApi.Error($"{ex.Message}\n{ex.StackTrace}");
            }
            catch (Exception ex)
            {
                XApi.Error($"{ex.Message}\n{ex.StackTrace}");
            }

            return null;
        }
    }
}
#endif //UNITY_SERVER || UNITY_EDITOR
