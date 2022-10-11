#include "SafeSystem.h"

#ifdef WIN32
#include <windows.h>
#endif //WIN32

namespace XEngine {
    namespace SafeSystem {
        namespace Time {
            void MillisecondSleep(const int millisecond) {
#ifdef WIN32
                Sleep(millisecond);
#endif //WIN32

#ifdef Linux
                sleep(millisecond);
#endif //Linux
            }
        }

        namespace Process {
            unsigned_int64 GetCurrentProcessID() {
#ifdef WIN32
                return ::GetCurrentProcessId();
#endif //WIN32
            }

            unsigned_int64 GetCurrentThreadID() {
#ifdef WIN32
                return ::GetCurrentThreadId();
#endif //WIN32
            }
        }
    }
}

