#include "TimeGear.h"
#include "TimeBaseList.h"
#include "TimeWheel.h"

namespace XEngine {
    TimeGear::TimeGear(int maxMoveDst, TimeGear* nextGear)
        : _timer_vec(nullptr),
        _next(nextGear),
        _cur_move_dst(0),
        _max_move_dst(maxMoveDst) {

        _timer_vec = xnew TimeBaseList[_max_move_dst];
    }

    TimeGear::~TimeGear() {
        if (_timer_vec) {
            xdel[] _timer_vec;
            _timer_vec = nullptr;
        }
    }

    TimeBaseList* TimeGear::GetTimerList(int index) {
        XASSERT(index < _max_move_dst, "index out of range");
        return _timer_vec + index;
    }

    void TimeGear::CheckHighGear() {
        if (_cur_move_dst >= _max_move_dst)
            _cur_move_dst = 0;

        if (_cur_move_dst == 0) {
            if (_next) {
                _next->UpdateToLowGear();
            }
        }
    }

    void TimeGear::Update() {
        if (_cur_move_dst >= _max_move_dst)
            _cur_move_dst = 0;

        TimeBaseList* currentList = &_timer_vec[_cur_move_dst];
        if (!currentList) {
            _cur_move_dst++;
            return;
        }

        while (!currentList->Empty()) {
            TimeBase* base = currentList->PopFront();
            if (!base)
                continue;

            dynamic_cast<TimeWheel*>(TimeWheel::GetInstance())->MoveToRunning(base);
        }

        ++_cur_move_dst;
        if (_cur_move_dst == _max_move_dst)
            _cur_move_dst = 0;
    }

    void TimeGear::UpdateToLowGear() {
        if (_cur_move_dst >= _max_move_dst)
            _cur_move_dst = 0;

        if (_cur_move_dst == 0) {
            if (_next)
                _next->UpdateToLowGear();
        }

        TimeBaseList* currentList = &_timer_vec[_cur_move_dst];
        if (!currentList) {
            ++_cur_move_dst;
            return;
        }

        while (!currentList->Empty()) {
            TimeBase* base = currentList->PopFront();
            if (!base)
                continue;

            dynamic_cast<TimeWheel*>(TimeWheel::GetInstance())->Schedule(base);
        }

        ++_cur_move_dst;
    }
}
