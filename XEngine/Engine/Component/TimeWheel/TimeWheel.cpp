#include "TimeWheel.h"
#include "TimeBase.h"
#include "TimeGear.h"
#include "TimeBaseList.h"
#include <unordered_map>

namespace XEngine {

    struct TimerContext {
        const int _Id;
        void* const _Context;
        TimerContext(const int id, void* const context) : _Id(id), _Context(context) {}

        operator size_t() const {
            return _Id + (unsigned_int64)_Context;
        }

        bool operator ==(const TimerContext& target) const {
            return _Id == target._Id && _Context == target._Context;
        }
    };

    typedef std::unordered_map<TimerContext, TimeBase*, SafeTools::T_Hash<TimerContext>> TIMEBASE_MAP;
    typedef std::unordered_map<Api::iTimer*, TIMEBASE_MAP> TIME_MAP;
    static TIME_MAP s_TimerMap;

    iTimeWheel* TimeWheel::GetInstance() {
        static TimeWheel s_Wheel;
        return &s_Wheel;
    }

    bool TimeWheel::Initialize(Api::iEngine* const engine) {
        _TimeGears[4] = xnew TimeGear(TQTvnSize, 0);
        _TimeGears[3] = xnew TimeGear(TQTvnSize, _TimeGears[4]);
        _TimeGears[2] = xnew TimeGear(TQTvnSize, _TimeGears[3]);
        _TimeGears[1] = xnew TimeGear(TQTvnSize, _TimeGears[2]);
        _TimeGears[0] = xnew TimeGear(TQTvrSize, _TimeGears[1]);

        _Running = xnew TimeBaseList;
        _Suspended = xnew TimeBaseList;
        return true;
    }

    bool TimeWheel::Launch(Api::iEngine* const engine) {
        return true;
    }

    void TimeWheel::Release(Api::iEngine* const engine) {
        for (int i = 0; i < 5; ++i) {
            xdel _TimeGears[i];
            _TimeGears[i] = nullptr;
        }
        xdel _Running;
        xdel _Suspended;
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

        while (!_Running->Empty()) {
            TimeBase* base = _Running->PopFront();
            if (!base) {
                XERROR(engine, "where is timer base, _running %x", _Running);
            }

            base->OnTimer();
            if (!base->IsValid())
                Remove(base);
            else if (base->IsPaused())
                _Suspended->PushBack(base);
            else {
                base->AdjustExpire(_Jiff);
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
            base->SetExpire(_Jiff + delay / JIFF);
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

        base->Pause(_Jiff);
        if (base->IsValid() && !base->IsPolling()) {
            if (!base->GetList()) {
                //error(core::getInstance(), "base is not in a pause timer list, timer %x, id %d", timer, id);
            }

            base->GetList()->Remove(base);
            _Suspended->PushBack(base);
        }
    }

    void TimeWheel::ResumeTimer(Api::iTimer* timer, const int id, void* const context) {
        TimeBase* base = FindTimerBase(timer, id, context);
        if (base == nullptr) {
            //error(core::getInstance(), "where is timer base, timer %x, id %d", timer, id);
            return;
        }

        if (base->GetList() != _Suspended) {
            //error(core::getInstance(), "base %x is not in supended list, timer %x, id %d", base, timer, id);
        }

        if (!base->IsValid())
            return;

        base->Resume(_Jiff);
        if (base->IsValid() && !base->IsPolling()) {
            base->GetList()->Remove(base);
            Schedule(base);
        }
    }

    TimeBase* TimeWheel::CreateTimerBase(Api::iTimer* timer, const int id, void* const context, int count, int interval, const char* file, const int line) {
        TIME_MAP::iterator itor = s_TimerMap.find(timer);
        if (itor == s_TimerMap.end()) {
            s_TimerMap.insert(std::make_pair(timer, TIMEBASE_MAP()));
            itor = s_TimerMap.find(timer);
        }

        TimeBase* base = TimeBase::Create(timer, id, context, count, interval / JIFF, file, line);
        itor->second.insert(std::make_pair(TimerContext(id, context), base));
        return base;
    }

    void TimeWheel::RemoveTimerBase(TimeBase* base, Api::iTimer* timer, const int id, void* const context) {
        TIME_MAP::iterator itor = s_TimerMap.find(timer);
        if (itor != s_TimerMap.end()) {
            TIMEBASE_MAP::iterator ifind = itor->second.find(TimerContext(id, context));
            if (ifind != itor->second.end() && ifind->second == base) {
                itor->second.erase(ifind);
                return;
            }
        }
    }

    TimeBase* TimeWheel::FindTimerBase(Api::iTimer* timer, const int id, void* const context) {
        TIME_MAP::iterator itor = s_TimerMap.find(timer);
        if (itor != s_TimerMap.end()) {
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
        _Running->PushBack(base);
    }

    TimeBaseList* TimeWheel::FindTimerList(unsigned_int64 expire) {
        unsigned_int64 live = expire - _Jiff;
        TimeBaseList* list = 0;
        if (live < TQTvrSize)
            list = _TimeGears[0]->GetTimerList(expire & TQTvrMask);
        else if (live < (1 << (TQTvrBits + TQTvnBits)))
            list = _TimeGears[1]->GetTimerList((expire >> TQTvrBits) & TQTvnMask);
        else if (live < (1 << (TQTvrBits + 2 * TQTvnBits)))
            list = _TimeGears[2]->GetTimerList((expire >> (TQTvrBits + TQTvnBits)) & TQTvnMask);
        else if (live < (1 << (TQTvrBits + 3 * TQTvnBits)))
            list = _TimeGears[3]->GetTimerList((expire >> (TQTvrBits + 2 * TQTvnBits)) & TQTvnMask);
        else if ((long long)live < 0)
            list = _Running;
        else
            list = _TimeGears[4]->GetTimerList((expire >> (TQTvrBits + 3 * TQTvnBits)) & TQTvnMask);

        return list;
    }

    void TimeWheel::_Update_() {
        XASSERT(_TimeGears[0], "where is timer gear");
        _TimeGears[0]->CheckHighGear();
        ++_Jiff;
        _TimeGears[0]->Update();
    }

    void TimeWheel::Remove(TimeBase* base) {
        TIME_MAP::iterator itor = s_TimerMap.find(base->GetTimer());
        if (itor != s_TimerMap.end()) {
            TIMEBASE_MAP::iterator ifind = itor->second.find(TimerContext(base->_Id, base->_Context));
            if (ifind != itor->second.end() && ifind->second == base) {
                itor->second.erase(ifind);
            }
        }

        base->Release();
    }
}
