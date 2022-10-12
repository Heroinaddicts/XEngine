#ifndef __SafeSystem_h__
#define __SafeSystem_h__

#include "MultiSys.h"
#include <chrono>
#include <string>
#include <vector>

namespace XEngine {
    namespace SafeSystem {
        namespace Time {
            void MillisecondSleep(const int millisecond);

            inline unsigned_int64 GetMicroSecond() {
                return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            }

            inline unsigned_int64 GetMilliSecond() {
                return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            }
        }

        namespace Process {
            const unsigned_int64 INVAILD_ID = -1;

            unsigned_int64 GetCurrentProcessID();
            unsigned_int64 GetCurrentThreadID();
        }

        namespace File {
            const std::string& GetApplicationPath();
            int GetFileInDirectory(const std::string& dir, const std::string& extension, OUT std::vector<std::string>& paths, OUT std::vector<std::string>& names, bool recursive = true);
        }
    }
}

#endif //__SafeSystem_h__
