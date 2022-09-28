#ifndef __tgear_h__
#define __tgear_h__

#include "multisys.h"

namespace tcore {
    class tlist;
    class tgear {
    public:
        tgear(int maxMoveDst, tgear* nextGear);
        ~tgear();

        tlist * getTimerList(int index);
        void checkHighGear();
        void update();
        void updateToLowGear();

    private:
        tgear(tgear&);
        tgear& operator =(tgear&);

        tlist * _timerVec;//
        tgear * _nextGear;//
        int _curMoveDst;//
        int _maxMoveDst;//
    };
}

#endif //__tgear_h__
