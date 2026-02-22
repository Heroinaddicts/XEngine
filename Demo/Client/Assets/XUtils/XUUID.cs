using System;

namespace XUtils
{
    public static class XUUID
    {
        private static UInt16 s_Sequence = 0;
        private static UInt64 s_LastTimestamp = 0;
        private static readonly object s_Lock = new object();
        private static readonly UInt64 s_Epoch = (UInt64)(new DateTime(2024, 1, 1, 0, 0, 0, DateTimeKind.Utc).Ticks / TimeSpan.TicksPerMillisecond);

        static UInt64 GetCurrentTimestamp()
        {
            return (UInt64)DateTimeOffset.UtcNow.ToUnixTimeMilliseconds() - s_Epoch;
        }

        public static UInt64 GenerateUUID(UInt16 mask)
        {
            lock (s_Lock)
            {
                UInt64 currentMillis = GetCurrentTimestamp();

                if (currentMillis == s_LastTimestamp)
                {
                    s_Sequence++;
                    if (s_Sequence > 255)
                    {
                        // 超过每毫秒序列上限，强制等到下一个毫秒
                        while (currentMillis <= s_LastTimestamp)
                        {
                            currentMillis = GetCurrentTimestamp();
                        }
                        s_Sequence = 0;
                        s_LastTimestamp = currentMillis;
                    }
                }
                else
                {
                    s_Sequence = 0;
                    s_LastTimestamp = currentMillis;
                }

                UInt64 uuid = (currentMillis << 24) | ((UInt64)mask << 8) | s_Sequence;
                return uuid;
            }
        }
    }
}
