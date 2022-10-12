#include "TimeWheel.h"
#include "TimeBase.h"
#include "TimeGear.h"
#include "TimeBaseList.h"
#include <unordered_map>

namespace XEngine {

    struct TimerContext {
        const int _id;
        void* const _context;
        TimerContext(const int id, void* const context) : _id(id), _context(context) {}

        operator size_t() const {
            return _id + (unsigned_int64)_context;
        }

        bool operator ==(const TimerContext& target) const {
            return _id == target._id && _context == target._context;
        }
    };

    typedef std::unordered_map<TimerContext, TimeBase*, SafeTools::T_Hash<TimerContext>> TIMEBASE_MAP;
    typedef std::unordered_map<Api::iTimer*, TIMEBASE_MAP> TIME_MAP;
    static TIME_MAP static_timer_map;

    iTimeWheel* TimeWheel::GetInstance() {
        static TimeWheel static_wheel;
        return &static_wheel;
    }

    bool TimeWheel::Initialize(Api::iEngine* const engine) {
        _time_gears[4] = xnew TimeGear(tq_tvn_size, 0);
        _time_gears[3] = xnew TimeGear(tq_tvn_size, _time_gears[4]);
        _time_gears[2] = xnew TimeGear(tq_tvn_size, _time_gears[3]);
        _time_gears[1] = xnew TimeGear(tq_tvn_size, _time_gears[2]);
        _time_gears[0] = xnew TimeGear(tq_tvr_size, _time_gears[1]);

        _running = xnew TimeBaseList;
        _suspended = xnew TimeBaseList;
        return true;
    }

    bool TimeWheel::Launch(Api::iEngine* const engine) {
        return true;
    }

    void TimeWheel::Release(Api::iEngine* const engine) {
        for (int i = 0; i < 5; ++i) {
            xdel _time_gears[i];
            _time_gears[i] = nullptr;
        }
        xdel _running;
        xdel _suspended;
        xdel this;
    }

    void TimeWheel::EarlyUpdate(Api::iEngine* const engine) {

    }

    void TimeWheel::Update(Api::iEngine* const engine) {
        static int64 last = SafeSystem::Time::GetMilliSecond();
        int64 tick = SafeSystem::Time::GetMilliSecond();

        int count = (int)(tick - last) / JIFF;
        for (int i = 0; i < count; ++i)
            _Update_();
        last += count * JIFF;

        while (!_running->Empty()) {
            TimeBase* base = _running->PopFront();
            if (!base) {
                XERROR(engine, "where is timer base, _running %x", _running);
            }

            base->OnTimer();
            if (!base->IsValid())
                Remove(base);
            else if (base->IsPaused())
                _suspended->PushBack(base);
            else {
                base->AdjustExpire(_jiff);
                Schedule(base);
            }
        }
    }

    void TimeWheel::FixedUpdate(Api::iEngine* const engine) {

    }

    void TimeWheel::LaterUpdate(Api::iEngine* const engine) {

    }

    void TimeWheel::StartTimer(Api::iTimer* timer, const int id, const int delay, const int count, const int interval, void* const context, const char* const file, const int line) {
        TimeBase* base = FindTimerBase(timer, id, context);
        if (nullptr == base) {
            XASSERT(interval > 0 && delay >= 0, "wtf");
            base = CreateTimerBase(timer, id, context, count, interval >= JIFF ? interval : JIFF, file, line);
            base->SetExpire(_jiff + delay / JIFF);
            Schedule(base);
        }
    }

    void TimeWheel::KillTimer(Api::iTimer* timer, const int id, void* const context) {
        TimeBase* base = FindTimerBase(timer, id, context);
        if (base == nullptr || !base->IsValid()) {
            return;
        }

        base->ForceEnd();
        if (!base->IsPolling()) {
            if (!base->GetList()) {
                //XERROR("base is not in a timer list, base %x, timer %x, id %d", base, timer, id);
            }

            base->GetList()->Remove(base);
            Remove(base);
        }
        else {
            RemoveTimerBase(base, timer, id, context);
        }
    }

    void TimeWheel::PauseTimer(Api::iTimer* timer, const int id, void* const context) {
        TimeBase* base = FindTimerBase(timer, id, context);
        if (base == nullptr) {
            ////error(core::getInstance(), "where is timer base, timer %x, id %d", timer, id);
            return;
        }

        if (!base->IsValid())
            return;

        base->Pause(_jiff);
        if (base->IsValid() && !base->IsPolling()) {
            if (!base->GetList()) {
                //error(core::getInstance(), "base is not in a pause timer list, timer %x, id %d", timer, id);
            }

            base->GetList()->Remove(base);
            _suspended->PushBack(base);
        }
    }

    void TimeWheel::ResumeTimer(Api::iTimer* timer, const int id, void* const context) {
        TimeBase* base = FindTimerBase(timer, id, context);
        if (base == nullptr) {
            //error(core::getInstance(), "where is timer base, timer %x, id %d", timer, id);
            return;
        }

        if (base->GetList() != _suspended) {
            //error(core::getInstance(), "base %x is not in supended list, timer %x, id %d", base, timer, id);
        }

        if (!base->IsValid())
            return;

        base->Resume(_jiff);
        if (base->IsValid() && !base->IsPolling()) {
            base->GetList()->Remove(base);
            Schedule(base);
        }
    }

    TimeBase* TimeWheel::CreateTimerBase(Api::iTimer* timer, const int id, void* const context, int count, int interval, const char* file, const int line) {
        TIME_MAP::iterator itor = static_timer_map.find(timer);
        if (itor == static_timer_map.end()) {
            static_timer_map.insert(std::make_pair(timer, TIMEBASE_MAP()));
            itor = static_timer_map.find(timer);
        }

        TimeBase* base = TimeBase::Create(timer, id, context, count, interval / JIFF, file, line);
        itor->second.insert(std::make_pair(TimerContext(id, context), base));
        return base;
    }

    void TimeWheel::RemoveTimerBase(TimeBase* base, Api::iTimer* timer, const int id, void* const context) {
        TIME_MAP::iterator itor = static_timer_map.find(timer);
        if (itor != static_timer_map.end()) {
            TIMEBASE_MAP::iterator ifind = itor->second.find(TimerContext(id, context));
            if (ifind != itor->second.end() && ifind->second == base) {
                itor->second.erase(ifind);
                return;
            }
        }
    }

    TimeBase* TimeWheel::FindTimerBase(Api::iTimer* timer, const int id, void* const context) {
        TIME_MAP::iterator itor = static_timer_map.find(timer);
        if (itor != static_timer_map.end()) {
            TIMEBASE_MAP::iterator ifind = itor->second.find(TimerContext(id, context));
            if (ifind != itor->second.end()) {
                return ifind->second;
            }
        }

        return nullptr;
    }

    void TimeWheel::Schedule(TimeBase* base) {
        TimeBaseList* list = FindTimerList(base->GetExpire());
        XASSERT(list, "find timer list failed");
        list->PushBack(base);
    }

    void TimeWheel::MoveToRunning(TimeBase* base) {
        _running->PushBack(base);
    }

    TimeBaseList* TimeWheel::FindTimerList(unsigned_int64 expire) {
        unsigned_int64 live = expire - _jiff;
        TimeBaseList* list = 0;
        if (live < tq_tvr_size)
            list = _time_gears[0]->GetTimerList(expire & tq_tvr_mask);
        else if (live < (1 << (tq_tvr_bits + tq_tvn_bits)))
            list = _time_gears[1]->GetTimerList((expire >> tq_tvr_bits) & tq_tvn_mask);
        else if (live < (1 << (tq_tvr_bits + 2 * tq_tvn_bits)))
            list = _time_gears[2]->GetTimerList((expire >> (tq_tvr_bits + tq_tvn_bits)) & tq_tvn_mask);
        else if (live < (1 << (tq_tvr_bits + 3 * tq_tvn_bits)))
            list = _time_gears[3]->GetTimerList((expire >> (tq_tvr_bits + 2 * tq_tvn_bits)) & tq_tvn_mask);
        else if ((long long)live < 0)
            list = _running;
        else
            list = _time_gears[4]->GetTimerList((expire >> (tq_tvr_bits + 3 * tq_tvn_bits)) & tq_tvn_mask);

        return list;
    }

    void TimeWheel::_Update_() {
        XASSERT(_time_gears[0], "where is timer gear");
        _time_gears[0]->CheckHighGear();
        ++_jiff;
        _time_gears[0]->Update();
    }

    void TimeWheel::Remove(TimeBase* base) {
        TIME_MAP::iterator itor = static_timer_map.find(base->GetTimer());
        if (itor != static_timer_map.end()) {
            TIMEBASE_MAP::iterator ifind = itor->second.find(TimerContext(base->_id, base->_context));
            if (ifind != itor->second.end() && ifind->second == base) {
                itor->second.erase(ifind);
            }
        }

        base->Release();
    }
}
