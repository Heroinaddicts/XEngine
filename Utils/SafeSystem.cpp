#include "SafeSystem.h"

#ifdef WIN32
#include <windows.h>
#endif //WIN32

namespace XEngine {
    namespace SafeSystem {
        void MillisecondSleep(const int millisecond) {
#ifdef WIN32
            Sleep(millisecond);
#endif //WIN32

#ifdef Linux
            sleep(millisecond);
#endif //Linux
        }
    }
}

