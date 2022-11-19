#ifndef __SafeSystem_h__
#define __SafeSystem_h__

#include "MultiSys.h"
#include <chrono>
#include <string>
#include <vector>

#include "SafeMemory.h"

namespace XEngine {
    namespace SafeSystem {
        namespace Time {
            enum {
                Second = 1000,
                Minute = 60 * Second,
                Hour = 60 * Minute,
                Day = 24 * Hour
            };

            inline bool SafeLocaltime(struct tm& tm, const time_t& tt) {
                const struct tm* p = localtime(&tt);
                if (nullptr == p) {
                    return false;
                }

                SafeMemory::Memcpy(&tm, sizeof(tm), p, sizeof(tm));
                return true;
            }

            void MillisecondSleep(const int millisecond);

            inline unsigned_int64 GetMicroSecond() {
                return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            }

            inline unsigned_int64 GetMilliSecond() {
                return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            }

            inline std::string GetCurrentTimeString(const char* format = "%4d-%02d-%02d %02d:%02d:%02d") {
                char strtime[64] = { 0 };
                auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                struct tm tm;
                SafeLocaltime(tm, tt);
                sprintf_s(strtime, sizeof(strtime), format,
                    (int)tm.tm_year + 1900, (int)tm.tm_mon + 1, (int)tm.tm_mday,
                    (int)tm.tm_hour, (int)tm.tm_min, (int)tm.tm_sec);
                return strtime;
            }
        }

        namespace Process {
            const unsigned_int64 INVAILD_ID = -1;

            unsigned_int64 GetCurrentProcessID();
            unsigned_int64 GetCurrentThreadID();
            unsigned_int64 LaunchProcess(const std::string& exPath, const std::string& args = "");
        }

        namespace File {
            const std::string& GetCurrentExecutablePath();
            const std::string& GetsPathOfCurrentExecutable();

            bool FileExists(const std::string& path);
            bool FolderExists(const std::string& path);
            bool CreateFolder(const std::string& path);
            bool DelFolder(const std::string& path);
            bool DelFile(const std::string& path);

            int GetFileInDirectory(const std::string& dir, const std::string& extension, OUT std::vector<std::string>& paths, OUT std::vector<std::string>& names, bool recursive = true);
        }
    }
}

#endif //__SafeSystem_h__
