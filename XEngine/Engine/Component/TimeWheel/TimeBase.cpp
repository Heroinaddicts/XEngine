#include "TimeBase.h"

namespace XEngine {
    static XPool<TimeBase> static_base_pool;

    TimeBase* TimeBase::Create(Api::iTimer* timer, const int id, void* const context, const int count, const int interval, const char* file, const int line) {
        return XPOOL_CREATE(static_base_pool, timer, id, context, count, interval, file, line);
    }

    void TimeBase::OnTimer() {
        XASSERT(_Timer, "where is timer ???");
        const int64 tick = SafeSystem::Time::GetMilliSecond();
        _Polling = true;
        if (!_Started) {
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
            _Timer->OnEnd(_Id, _Context, true, tick);
        }
    }

    void TimeBase::AdjustExpire(unsigned_int64 now) {
        long long live = (long long)(_Expire - now);
        if (live < 0 && abs(live) > _Interval) {
            _Expire += (abs(live) / _Interval) * _Interval;
        }
    }

    void TimeBase::ForceEnd() {
        XASSERT(_Valid, "timer is already invalid");

        _Valid = false;
        _Timer->OnEnd(_Id, _Context, false, SafeSystem::Time::GetMilliSecond());
    }

    void TimeBase::Pause(unsigned_int32 jiff) {
        if (_Valid) {
            XASSERT(!_Paused, "timer already is paused");
            _PauseTick = jiff;
            _Paused = true;
            _Timer->OnPause(_Id, _Context, SafeSystem::Time::GetMilliSecond());
        }
    }

    void TimeBase::Resume(unsigned_int32 jiff) {
        if (_Valid) {
            XASSERT(_Paused, "timer is not paused");

            _Expire = jiff + _Expire - _PauseTick;
            _Paused = false;
            _Timer->OnResume(_Id, _Context, SafeSystem::Time::GetMilliSecond());
        }
    }

    void TimeBase::Release() {
        XPOOL_RELEASE(static_base_pool, this);
    }

    TimeBase::TimeBase(Api::iTimer* timer, const int id, void* const context, int count, int interval, const char* file, const int line)
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
}
