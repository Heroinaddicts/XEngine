using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Threading;

namespace XUtils
{
    public class XEventPool<E>
        where E : Enum
    {
        struct DebugInfo
        {
            public readonly string _File;
            public readonly int _Line;

            public DebugInfo(in string file, in int line)
            {
                _File = file;
                _Line = line;
            }
        }

        Dictionary<E, Dictionary<Delegate, DebugInfo>> _Events = new Dictionary<E, Dictionary<Delegate, DebugInfo>>();

        public readonly string _File;
        public readonly int _Line;
        public readonly string _Member;

        public XEventPool([CallerFilePath] string file = "", [CallerLineNumber] int line = 0, [CallerMemberName] string member = "")
        {
            _File = file;
            _Line = line;
            _Member = member;
        }

        public void Regist<T>(in E e, in T t, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0, [CallerMemberName] string member = "") where T : Delegate
        {
            Dictionary<Delegate, DebugInfo> dic = null;
            if (!_Events.TryGetValue(e, out dic))
            {
                dic = new Dictionary<Delegate, DebugInfo>();
                _Events[e] = dic;
            }

            if (dic.ContainsKey(t))
            {
                XUtilsSetting.Log($"Double Regist {e}:{t.Method.Name} at {file}:{line}");
                return;
            }

            dic.Add(t, new DebugInfo(file, line));
        }

        public void CancelRegist<T>(in E e, in T t, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0, [CallerMemberName] string member = "") where T : Delegate
        {
            Dictionary<Delegate, DebugInfo> dic = null;
            if (_Events.TryGetValue(e, out dic))
            {
                dic.Remove(t as Delegate);
                XUtilsSetting.Log($"CancelRegist {e}:{t.Method.Name} at {file}:{line}");
            }
        }

        public void Trigger(in E e, params object[] args)
        {
            Dictionary<Delegate, DebugInfo> dic = null;
            if (_Events.TryGetValue(e, out dic))
            {
                Delegate[] list = dic.Keys.ToArray();
                for (int i = 0; i < list.Length; i++)
                {
                    try
                    {
                        list[i].DynamicInvoke(args);
                    }
                    catch (Exception ex)
                    {
                        XUtilsSetting.Error($"Event {dic[list[i]]._File}:{dic[list[i]]._Line} >>  {ex.Message}\n{ex.StackTrace}");
                    }
                }
            }
        }
    }
}
