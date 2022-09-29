#ifndef __TimeGear_h__
#define __TimeGear_h__

#include "Header.h"

namespace XEngine {
    class TimeBaseList;
    class TimeGear {
    public:
        TimeGear(int maxMoveDst, TimeGear* nextGear);
        ~TimeGear();

        TimeBaseList* GetTimerList(int index);
        void CheckHighGear();
        void Update();
        void UpdateToLowGear();

    private:
        TimeBaseList* _timer_vec;//
        TimeGear* _next;//
        int _cur_move_dst;//
        int _max_move_dst;//
    };
}

#endif //__TimeGear_h__
