#include "TimeBase.h"

namespace XEngine {
    static XPool<TimeBase> s_TimeBasePool;

    TimeBase* TimeBase::Create(Api::iTimer* timer, const int id, const UInt64 context, const int count, const int interval, const char* file, const int line) {
        TimeBase* base = XPOOL_CREATE(s_TimeBasePool, timer, id, context, count, interval, file, line);
        if (timer->_TimerLog)
            TraceLog(g_Engine, "Create Time base %llx %s:%d", base, file, line);
        return base;
    }

    void TimeBase::OnTimer() {
        XASSERT(_Timer, "where is timer ???");
        const Int64 tick = SafeSystem::Time::GetMilliSecond();
        _Polling = true;
        if (!_Started) {
            if (_Timer->_TimerLog)
                TraceLog(g_Engine, "%llx OnStart %d, %llu base %llx", _Timer, _Id, _Context, this);
            _Timer->OnStart(_Id, _Context, tick);
            _Started = true;
            XASSERT(_Valid, "wtf");
        }

        if (_Valid) {
            _Timer->OnTimer(_Id, _Context, tick);
            if (_Count > 0) {
                _Count--;
            }
        }

        _Polling = false;
        _Expire += _Interval;

        if (_Count == 0 && _Valid) {
            _Valid = false;
            RemoveTimerBase(this, _Timer, _Id, _Context);
            if (_Timer->_TimerLog)
                TraceLog(g_Engine, "%llx OnEnd %d, %llu base %llx", _Timer, _Id, _Context, this);
            _Timer->OnEnd(_Id, _Context, true, tick);
        }
    }

    void TimeBase::AdjustExpire(UInt64 now) {
        long long live = (long long)(_Expire - now);
        if (live < 0 && abs(live) > _Interval) {
            _Expire += (abs(live) / _Interval) * _Interval;
        }
    }

    void TimeBase::ForceEnd(const char* file, const int line) {
        XASSERT(_Valid, "timer is already invalid");

        _Valid = false;
        if (_Timer->_TimerLog)
            TraceLog(g_Engine, "%llx ForceEnd %d, %llu base %llx  %s:%d", _Timer, _Id, _Context, this, file, line);
        _Timer->OnEnd(_Id, _Context, false, SafeSystem::Time::GetMilliSecond());
    }

    void TimeBase::Pause(UInt64 jiff) {
        if (_Valid) {
            XASSERT(!_Paused, "timer already is paused");
            _PauseTick = jiff;
            _Paused = true;
            _Timer->OnPause(_Id, _Context, SafeSystem::Time::GetMilliSecond());
        }
    }

    void TimeBase::Resume(UInt64 jiff) {
        if (_Valid) {
            XASSERT(_Paused, "timer is not paused");

            _Expire = jiff + _Expire - _PauseTick;
            _Paused = false;
            _Timer->OnResume(_Id, _Context, SafeSystem::Time::GetMilliSecond());
        }
    }

    void TimeBase::Release(const char* file, const int line) {
        if (this->_Timer->_TimerLog)
            TraceLog(g_Engine, "Release Time base %llx %s:%d", this, file, line);
        XPOOL_RELEASE(s_TimeBasePool, this);
    }

    TimeBase::TimeBase(Api::iTimer* timer, const int id, const UInt64 context, int count, int interval, const char* file, const int line)
        : _Timer(timer), _Id(id), _Interval(interval), _Context(context)
#ifdef _DEBUG
        , _File(file), _Line(line)
#endif //_DEBUG
    {
        _List = nullptr;
        _Next = nullptr;
        _Prev = nullptr;

        _Valid = true;
        _Polling = false;

        _Expire = 0;
        _Count = count;
        _Started = false;

        _PauseTick = 0;
        _Paused = false;
    }

    TimeBase* CreateTimerBase(Api::iTimer* timer, const int id, const UInt64 context, int count, int interval, const char* file, const int line) {
        TIME_MAP::iterator itor = g_TimerMap.find(timer);
        if (itor == g_TimerMap.end()) {
            g_TimerMap.insert(std::make_pair(timer, TIMEBASE_MAP()));
            itor = g_TimerMap.find(timer);
        }

        TimeBase* base = TimeBase::Create(timer, id, context, count, interval / JIFF, file, line);
        itor->second.insert(std::make_pair(TimerContext(id, context), base));
        return base;
    }

    void RemoveTimerBase(TimeBase* base, Api::iTimer* timer, const int id, const UInt64 context) {
        TIME_MAP::iterator itor = g_TimerMap.find(timer);
        if (itor != g_TimerMap.end()) {
            TIMEBASE_MAP::iterator ifind = itor->second.find(TimerContext(id, context));
            if (ifind != itor->second.end() && ifind->second == base) {
                itor->second.erase(ifind);
                if (itor->second.empty()) {
                    g_TimerMap.erase(itor);
                }
                return;
            }
        }
    }

    TimeBase* FindTimerBase(Api::iTimer* timer, const int id, const UInt64 context) {
        TIME_MAP::iterator itor = g_TimerMap.find(timer);
        if (itor != g_TimerMap.end()) {
            TIMEBASE_MAP::iterator ifind = itor->second.find(TimerContext(id, context));
            if (ifind != itor->second.end()) {
                return ifind->second;
            }
        }

        return nullptr;
    }
}
