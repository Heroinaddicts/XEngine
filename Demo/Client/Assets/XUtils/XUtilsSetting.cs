using System;
using System.Runtime.CompilerServices;

namespace XUtils
{
    public static class XUtilsSetting
    {
        static Action<string, string, int> s_Log = null;
        static Action<string, string, int> s_Error = null;
        public static void SetLogFun(Action<string, string, int> fun)
        {
            s_Log = fun;
        }

        public static void SetErrorFun(Action<string, string, int> fun)
        {
            s_Error = fun;
        }

        public static void Log(in string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0)
        {
            s_Log?.Invoke(log, file, line);
        }

        public static void Error(in string log, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0)
        {
            s_Error?.Invoke(log, file, line);
        }
    }
}
