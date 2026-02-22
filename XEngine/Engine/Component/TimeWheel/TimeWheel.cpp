#include "TimeWheel.h"
#include "TimeBase.h"
#include "TimeGear.h"
#include "TimeBaseList.h"
#include <unordered_map>

namespace XEngine {
    TIME_MAP g_TimerMap;
    iTimeWheel* TimeWheel::GetInstance() {
        static TimeWheel s_Wheel;
        return &s_Wheel;
    }

    bool TimeWheel::Initialize(Api::iEngine* const engine) {
        _TimeGears[4] = txnew TimeGear(TQTvnSize, 0);
        _TimeGears[3] = txnew TimeGear(TQTvnSize, _TimeGears[4]);
        _TimeGears[2] = txnew TimeGear(TQTvnSize, _TimeGears[3]);
        _TimeGears[1] = txnew TimeGear(TQTvnSize, _TimeGears[2]);
        _TimeGears[0] = txnew TimeGear(TQTvrSize, _TimeGears[1]);

        _Running = txnew TimeBaseList;
        _Suspended = txnew TimeBaseList;
        return true;
    }

    bool TimeWheel::Launch(Api::iEngine* const engine) {
        return true;
    }

    void TimeWheel::Release(Api::iEngine* const engine) {
        for (int i = 0; i < 5; ++i) {
            txdel _TimeGears[i];
            _TimeGears[i] = nullptr;
        }
        txdel _Running;
        txdel _Suspended;
    }

    void TimeWheel::EarlyUpdate(Api::iEngine* const engine) {

    }

    void TimeWheel::Update(Api::iEngine* const engine) {
        static Int64 last = SafeSystem::Time::GetMilliSecond();
        Int64 tick = SafeSystem::Time::GetMilliSecond();

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
                base->Release(__FILE__, __LINE__);
            else if (base->IsPaused())
                _Suspended->PushBack(base);
            else {
                base->AdjustExpire(_Jiff);
                Schedule(base);
            }
        }
    }

    void TimeWheel::LaterUpdate(Api::iEngine* const engine) {

    }

    void TimeWheel::StartTimer(Api::iTimer* timer, const int id, const int delay, const int count, const int interval, const UInt64 context, const char* const file, const int line) {
        KillTimer(timer, id, context, file, line);
        TimeBase* base = CreateTimerBase(timer, id, context, count, interval >= JIFF ? interval : JIFF, file, line);
        base->SetExpire(_Jiff + delay / JIFF);
        Schedule(base);
    }

    void TimeWheel::KillTimer(Api::iTimer* timer, const int id, const UInt64 context, const char* file, const int line) {
        TimeBase* base = FindTimerBase(timer, id, context);
        if (base == nullptr || !base->IsValid()) {
            return;
        }

        base->ForceEnd(file, line);
        if (!base->IsPolling()) {
            if (!base->GetList()) {
                //XERROR("base is not in a timer list, base %x, timer %x, id %d", base, timer, id);
            }

            base->GetList()->Remove(base);
            Remove(base, file, line);
        }
        else {
            RemoveTimerBase(base, timer, id, context);
        }
    }

    void TimeWheel::PauseTimer(Api::iTimer* timer, const int id, const UInt64 context) {
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

    void TimeWheel::ResumeTimer(Api::iTimer* timer, const int id, const UInt64 context) {
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

    bool TimeWheel::IsExistsTimer(Api::iTimer* timer, const int id, const UInt64 context) {
        return FindTimerBase(timer, id, context) != nullptr;
    }

    void TimeWheel::Schedule(TimeBase* base) {
        TimeBaseList* list = FindTimerList(base->GetExpire());
        XASSERT(list, "find timer list failed");
        list->PushBack(base);
    }

    void TimeWheel::MoveToRunning(TimeBase* base) {
        _Running->PushBack(base);
    }

    TimeBaseList* TimeWheel::FindTimerList(UInt64 expire) {
        UInt64 live = expire - _Jiff;
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

    void TimeWheel::Remove(TimeBase* base, const char* file, const int line) {
        TIME_MAP::iterator itor = g_TimerMap.find(base->GetTimer());
        if (itor != g_TimerMap.end()) {
            TIMEBASE_MAP::iterator ifind = itor->second.find(TimerContext(base->_Id, base->_Context));
            if (ifind != itor->second.end() && ifind->second == base) {
                itor->second.erase(ifind);
                if (itor->second.empty()) {
                    g_TimerMap.erase(itor);
                }
            }
        }

        base->Release(file, line);
    }
}
