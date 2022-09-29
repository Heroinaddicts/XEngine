#include "TimeBase.h"

namespace XEngine {
    static XPool<TimeBase> static_base_pool;

    TimeBase* TimeBase::Create(Api::iTimer* timer, const int id, void* const context, const int count, const int interval, const char* file, const int line) {
        return XPOOL_CREATE(static_base_pool, timer, id, context, count, interval, file, line);
    }

    void TimeBase::OnTimer() {
        XASSERT(_timer, "where is timer ???");
        const int64 tick = SafeSystem::GetMilliSecond();
        _polling = true;
        if (!_started) {
            _timer->OnStart(_id, _context, tick);
            _started = true;
            XASSERT(_valid, "wtf");
        }

        if (_valid) {
            _timer->OnTimer(_id, _context, tick);
            if (_count > 0) {
                _count--;
            }
        }

        _polling = false;
        _expire += _interval;

        if (_count == 0 && _valid) {
            _valid = false;
            _timer->OnEnd(_id, _context, true, tick);
        }
    }

    void TimeBase::AdjustExpire(unsigned_int64 now) {
        long long live = (long long)(_expire - now);
        if (live < 0 && abs(live) > _interval) {
            _expire += (abs(live) / _interval) * _interval;
        }
    }

    void TimeBase::ForceEnd() {
        XASSERT(_valid, "timer is already invalid");

        _valid = false;
        _timer->OnEnd(_id, _context, false, SafeSystem::GetMilliSecond());
    }

    void TimeBase::Pause(unsigned_int32 jiff) {
        if (_valid) {
            XASSERT(!_paused, "timer already is paused");
            _pause_tick = jiff;
            _paused = true;
            _timer->OnPause(_id, _context, SafeSystem::GetMilliSecond());
        }
    }

    void TimeBase::Resume(unsigned_int32 jiff) {
        if (_valid) {
            XASSERT(_paused, "timer is not paused");

            _expire = jiff + _expire - _pause_tick;
            _paused = false;
            _timer->OnResume(_id, _context, SafeSystem::GetMilliSecond());
        }
    }

    void TimeBase::Release() {
        XPOOL_RELEASE(static_base_pool, this);
    }

    TimeBase::TimeBase(Api::iTimer* timer, const int id, void* const context, int count, int interval, const char* file, const int line)
        : _timer(timer), _id(id), _interval(interval), _context(context), _file(file), _line(line) {
        _list = nullptr;
        _next = nullptr;
        _prev = nullptr;

        _valid = true;
        _polling = false;

        _expire = 0;
        _count = count;
        _started = false;

        _pause_tick = 0;
        _paused = false;
    }
}
