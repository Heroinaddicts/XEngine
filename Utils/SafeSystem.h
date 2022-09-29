#ifndef __SafeSystem_h__
#define __SafeSystem_h__

#include "MultiSys.h"
#include <chrono>

namespace SafeSystem {
    void MillisecondSleep(const int millisecond);

    inline unsigned_int64 GetMicroSecond() {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    inline unsigned_int64 GetMilliSecond() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
}

#endif //__SafeSystem_h__
