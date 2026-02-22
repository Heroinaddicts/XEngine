using System;
using System.Collections.Generic;
using System.IO;

namespace XUtils
{
    public static class SafeSystem
    {
        public static class HighResolutionTime
        {
            // Unix 纪元时间（1970-01-01 00:00:00 UTC）
            private static readonly DateTime _unixEpoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);

            /// <summary>
            /// 获取从 Unix 纪元（1970-01-01 00:00:00 UTC）到当前时间的微秒数
            /// </summary>
            public static Int64 GetMicrosecondsSinceEpoch()
            {
                // 计算当前 UTC 时间与 Unix 纪元的时间差
                TimeSpan timeSinceEpoch = DateTime.UtcNow - _unixEpoch;
                // 转换为微秒（1 秒 = 1,000,000 微秒）
                return (Int64)(timeSinceEpoch.TotalSeconds * 1000000);
            }
        }

        public static class Time
        {
            public const int Second = 1000;
            public const int Minute = 60 * Second;
            public const int Hour = 60 * Minute;
            public const int Day = 24 * Second;

            public static Int64 GetMillisecondTick()
            {
                return DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
            }

            public static UInt32 GetUtcMilliSeconds32()
            {
                long epochTicks = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc).Ticks;
                long nowTicks = DateTime.UtcNow.Ticks;
                long totalMilliseconds = (nowTicks - epochTicks) / 10000;
                return (uint)totalMilliseconds;
            }

        }
        public static class File
        {
            public static int GetFiles(in string directory, out List<string> paths, out List<string> names, in string ext, List<string> exts = null)
            {
                paths = null;
                names = null;
                if (!Directory.Exists(directory))
                    return 0;

                string[] files = Directory.GetFiles(directory, "*." + ext, SearchOption.TopDirectoryOnly);
                if (files.Length > 0)
                {
                    paths = new List<string>();
                    names = new List<string>();
                    foreach (var file in files)
                    {
                        paths.Add(file.Replace("\\", "/")); // 统一斜杠
                        names.Add(Path.GetFileNameWithoutExtension(file));
                        if (null != exts)
                        {
                            exts.Add(Path.GetExtension(file).TrimStart('.'));
                        }
                    }
                }

                return paths.Count;
            }
        }
    }

}
