using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using XUtils;
using static XEngine.XApi.ITimer;

namespace XEngine
{
    struct TimerInfo : IEquatable<TimerInfo>
    {
        public readonly XApi.ITimer.fTimer _Delegate;
        public readonly object _Object;
        public readonly object _Context;

        public TimerInfo(XApi.ITimer.fTimer dg, object obj, object context)
        {
            _Delegate = dg;
            _Object = obj;
            _Context = context;
        }

        // 统一比较语义：值相等（与当前 Equals 保持一致）
        public bool Equals(TimerInfo other)
        {
            return _Delegate.Method.Equals(other._Delegate.Method)
                && Equals(_Delegate.Target, other._Delegate.Target)
                && Equals(_Object, other._Object);
        }

        // 删掉 EqualsOther，避免双语义
        public override bool Equals(object obj)
            => obj is TimerInfo o && Equals(o);

        // 用乘质数组合，冲突更低；与 Equals 完全一致的字段集
        public override int GetHashCode()
        {
            unchecked
            {
                int h = _Delegate.Method.GetHashCode();
                h = (h * 397) ^ _Delegate.Target.GetHashCode();
                h = (h * 397) ^ _Object.GetHashCode();
                return h;
            }
        }

        public void OnCallback(eState state, int count, long tick, bool murder)
            => _Delegate(state, count, tick, _Object, _Context, murder);

        public override string ToString()
            => $"{_Object} - {_Delegate.Method}";
    }


    class Intervaler
    {
        public readonly TimerInfo _Info;
        public readonly int _Count;
        public readonly int _Delay;
        public readonly int _Interval;
        public readonly string _File;
        public readonly int _Line;

        public int _ExecCount = 0;
        public bool _IsVaild = true;
        public Int64 _NextRunTick = 0;

        public Intervaler(TimerInfo info, int count, int delay, int interval, string file, int line)
        {
            _Info = info;
            _Count = count;
            _Delay = delay;
            _Interval = interval;
            _File = file;
            _Line = line;
        }

        public override string ToString()
        {
            return _Info.ToString();
        }
    }

    class Timer : XApi.ITimer
    {
        const int CONST_SLOT_SIZE = 3600;
        const int SLOT_TICK = 2; //一个时间刻度2毫秒
        public readonly List<Intervaler>[] _Slots = new List<Intervaler>[CONST_SLOT_SIZE];

        readonly Int64 _StartTimeTick;
        Int64 _LastTick;

        Dictionary<TimerInfo, Intervaler> _Timers = new Dictionary<TimerInfo, Intervaler>();
        public Timer()
        {
            for (int i = 0; i < CONST_SLOT_SIZE; i++)
            {
                _Slots[i] = new List<Intervaler>(500);
            }
            _StartTimeTick = SafeSystem.Time.GetMillisecondTick();
            _LastTick = _StartTimeTick;
        }

        int GetSlotOffset(in Int64 tick)
        {
            return (int)((tick - _StartTimeTick) / SLOT_TICK) % CONST_SLOT_SIZE;
        }

        public void StartTimer(in XApi.ITimer.fTimer fun, in object obj, in object context, in int delay, in int count, in int interval, [CallerFilePath] string file = "", [CallerLineNumber] int line = 0)
        {
            TimerInfo info = new TimerInfo(fun, obj, context);
            Intervaler intervaler = null;
            if (_Timers.TryGetValue(info, out intervaler))
            {
                if (intervaler._IsVaild)
                {
                    intervaler._Info.OnCallback(XApi.ITimer.eState.END, intervaler._ExecCount, DateTimeOffset.UtcNow.ToUnixTimeMilliseconds(), true);
                    intervaler._IsVaild = false;
                }
                _Timers.Remove(info);
            }

            int realDelay = delay <= 0 ? SLOT_TICK * 2 : delay;
            intervaler = new Intervaler(info, count, realDelay, interval, file, line);
            _Timers[info] = intervaler;
            intervaler._NextRunTick = SafeSystem.Time.GetMillisecondTick() + realDelay;
            int slot = GetSlotOffset(intervaler._NextRunTick);
            _Slots[slot].Add(intervaler);
        }

        public void StopTimer(in XApi.ITimer.fTimer fun, in object obj)
        {
            TimerInfo info = new TimerInfo(fun, obj, null);
            Intervaler intervaler = null;
            if (_Timers.TryGetValue(info, out intervaler))
            {
                if (intervaler._IsVaild)
                {
                    intervaler._IsVaild = false;
                    try
                    {
                        intervaler._Info.OnCallback(XApi.ITimer.eState.END, intervaler._ExecCount, DateTimeOffset.UtcNow.ToUnixTimeMilliseconds(), true);
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"{ex.Message}\n{ex.StackTrace}");
                    }
                }
                _Timers.Remove(info);
            }
        }

        public bool IsTimerExists(in XApi.ITimer.fTimer fun, in object obj)
        {
            return _Timers.ContainsKey(new TimerInfo(fun, obj, null));
        }

        public void Update()
        {
            Int64 tick = SafeSystem.Time.GetMillisecondTick();
            while (_LastTick <= tick)
            {
                int slot = GetSlotOffset(_LastTick);
                List<Intervaler> list = _Slots[slot];
                for (int i = list.Count - 1; i >= 0; i--)
                {
                    Intervaler intervaler = list[i];
                    if (!intervaler._IsVaild)
                    {
                        list.RemoveAt(i);
                        continue;
                    }

                    if (intervaler._NextRunTick > _LastTick + SLOT_TICK)
                        continue;

                    list.RemoveAt(i);
                    try
                    {
                        if (intervaler._ExecCount == 0)
                        {
                            intervaler._Info.OnCallback(XApi.ITimer.eState.START, intervaler._ExecCount, tick, false);
                        }

                        if (intervaler._IsVaild)
                        {
                            intervaler._Info.OnCallback(XApi.ITimer.eState.BEAT, intervaler._ExecCount, tick, false);

                            if (intervaler._IsVaild)
                            {
                                if (intervaler._Count != XApi.ITimer.Unlimited && intervaler._ExecCount == intervaler._Count - 1)
                                {
                                    if (intervaler._IsVaild)
                                    {
                                        intervaler._IsVaild = false;
                                        _Timers.Remove(intervaler._Info);
                                        intervaler._Info.OnCallback(XApi.ITimer.eState.END, intervaler._ExecCount, tick, false);
                                    }
                                }
                                else
                                {
                                    intervaler._NextRunTick = _LastTick + intervaler._Interval;
                                    _Slots[GetSlotOffset(intervaler._NextRunTick)].Add(intervaler);
                                }
                                intervaler._ExecCount++;
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        XApi.Error($"Timer Start From {intervaler._File}:{intervaler._Line} >> {ex.Message}\n{ex.StackTrace}");
                    }
                }
                _LastTick += SLOT_TICK;
            }
        }
    }
}
