using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.CompilerServices;
using System.Threading;
using UnityEngine;
using XUtils;

namespace XEngine
{
    struct LogInfo
    {
        public readonly string _Log;
        public readonly string _File;
        public readonly int _Line;
        public LogInfo(string log, string file, int line)
        {
            _Log = log; _File = file; _Line = line;
        }
    }

    class Logger : XApi.ILog
    {
        SPSCQueue<LogInfo> _LogQueue = new SPSCQueue<LogInfo>();
        StreamWriter _AsyncWriter;
        StreamWriter _SyncWriter;

        public Logger()
        {
            XUtilsSetting.SetLogFun(this.Trace);
            XUtilsSetting.SetErrorFun(this.Error);
            string logDir = Path.Combine(Application.persistentDataPath, "log");
            UnityEngine.Debug.Log($"Log path {logDir}");
            // 如果没有就创建
            if (!Directory.Exists(logDir))
            {
                Directory.CreateDirectory(logDir);
            }

            Int64 tick = SafeSystem.Time.GetMillisecondTick();
            string logFile = Path.Combine(logDir, $"log_{tick}.log");
            string errorFile = Path.Combine(logDir, $"error_{tick}.log");

            _AsyncWriter = new StreamWriter(logFile);
            _SyncWriter = new StreamWriter(errorFile);

            new Thread(this.Run).Start();
        }
        public void Debug(string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0)
        {
#if UNITY_EDITOR
            string temp = $"{DateTime.Now} Debug Log {file}:{line}=>{log}";
            UnityEngine.Debug.Log(temp);
            _LogQueue.Push(new LogInfo(temp, file, line));
#endif //UNITY_EDITOR
        }

        public void Trace(string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0)
        {
            string temp = $"{DateTime.Now} Trace Log {file}:{line}=>{log}";
#if UNITY_EDITOR
            UnityEngine.Debug.Log(temp);
#endif //UNITY_EDITOR
            _LogQueue.Push(new LogInfo(temp, file, line));
        }

        public void Error(string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0)
        {
            string temp = $"{DateTime.Now} Error Log {file}:{line}=>{log}";
#if UNITY_EDITOR
            UnityEngine.Debug.LogError(temp);
#endif //UNITY_EDITOR
            _SyncWriter.WriteLine(temp);
            _SyncWriter.Flush();
        }

        void Run()
        {
            LogInfo info;
            while (true)
            {
                if (_LogQueue.Pull(out info))
                {
                    string log = $"{info._File}:{info._Line} >> {info._Log}\n";
                    _AsyncWriter.Write(log);
                }
                else
                {
                    _AsyncWriter.Flush();
                    Thread.Sleep(10);
                }
            }
        }
    }
}
