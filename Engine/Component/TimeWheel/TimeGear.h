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
        TimeBaseList* _TimeVec;//
        TimeGear* _Next;//
        int _CurrentMoveDst;//
        int _MaxMoveDst;//
    };
}

#endif //__TimeGear_h__
