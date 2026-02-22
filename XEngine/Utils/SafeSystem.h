#ifndef __SafeSystem_h__
#define __SafeSystem_h__

#include "MultiSys.h"
#include <chrono>
#include <string>
#include <vector>
#include <set>
#include <fstream> 
#include <sstream>
#include <filesystem>

#include "SafeMemory.h"

namespace XEngine {
    namespace SafeSystem {
        namespace HardWare {
            int NumberOfCpuCores();
        }

        namespace Network {
            constexpr const UInt64 Byte = 1;
            constexpr const UInt64 KB = 1024;
            constexpr const UInt64 MB = 1024 * KB;
            constexpr const UInt64 GB = MB * 1024;
            constexpr const UInt64 TB = GB * 1024;
            constexpr const UInt64 PB = TB * 1024;
        }

        namespace Time {
            enum {
                Second = 1000,
                Minute = 60 * Second,
                Hour = 60 * Minute,
                Day = 24 * Hour
            };

            __forceinline bool SafeLocaltime(struct tm& tm, const time_t& tt) {
                const struct tm* p = localtime(&tt);
                if (nullptr == p) {
                    return false;
                }

                SafeMemory::Memcpy(&tm, sizeof(tm), p, sizeof(tm));
                return true;
            }

            void MillisecondSleep(const int millisecond);

            __forceinline UInt64 GetMicroSecond() {
                return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            }

            __forceinline UInt64 GetMilliSecond() {
                return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            }

            __forceinline std::string GetCurrentTimeString(const char* format = "%4d-%02d-%02d %02d:%02d:%02d") {
                char strtime[64] = { 0 };
                auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                struct tm tm;
                SafeLocaltime(tm, tt);
                sprintf_s(strtime, sizeof(strtime), format,
                    (int)tm.tm_year + 1900, (int)tm.tm_mon + 1, (int)tm.tm_mday,
                    (int)tm.tm_hour, (int)tm.tm_min, (int)tm.tm_sec);
                return strtime;
            }

            __forceinline std::string GetTimeStringByMillisecond(const Int64 tick, const char* format = "%4d-%02d-%02d %02d:%02d:%02d") {
                char strtime[128];
                auto mTime = std::chrono::milliseconds(tick);
                auto tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(mTime);
                auto tt = std::chrono::system_clock::to_time_t(tp);
                struct tm tm;
                SafeLocaltime(tm, tt);
                sprintf_s(strtime, sizeof(strtime), format,
                    (int)tm.tm_year + 1900, (int)tm.tm_mon + 1, (int)tm.tm_mday,
                    (int)tm.tm_hour, (int)tm.tm_min, (int)tm.tm_sec);
                return strtime;
            }
        }

        namespace Process {
            const UInt64 INVAILD_ID = -1;

            UInt64 GetCurrentProcessID();
            UInt64 GetCurrentThreadID();
            UInt64 LaunchProcess(const std::string& exPath, const std::string& args = "");

            struct ProcessInfo {
                UInt64 _ProcessID;
                std::string _ProcessName;
            };

            bool GetProcessInfo(const UInt64 handle, OUT ProcessInfo& info);
        }

        namespace File {
            const std::string& GetCurrentExecutablePath();
            const std::string& GetDirectoryOfCurrentExe();

            __forceinline std::string LoadFileAsString(const std::string& path) {
                std::ifstream file(path, std::ios::in | std::ios::binary);
                if (!file.is_open()) {
                    return "";
                }

                std::stringstream ss;
                ss << file.rdbuf();
                return ss.str();
            }

            __forceinline bool SaveStringToFile(const std::string& path, const std::string& content) {
                std::filesystem::path filePath(path);
                std::filesystem::path directory = filePath.parent_path();

                if (!std::filesystem::exists(directory)) {
                    std::error_code ec;
                    std::filesystem::create_directories(directory, ec);
                    if (ec) {
                        return false;
                    }
                }

                std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
                if (!file.is_open()) {
                    return false;
                }

                const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
                file.write(reinterpret_cast<const char*>(bom), sizeof(bom));

                file.write(content.c_str(), static_cast<std::streamsize>(content.size()));
                return true;
            }

            bool FileExists(const std::string& path);
            bool FolderExists(const std::string& path);
            bool CreateFolder(const std::string& path);
            bool DelFolder(const std::string& path);
            bool DelFile(const std::string& path);

            int GetFileInDirectory(const std::string& dir, const std::string& extension, OUT std::vector<std::string>& paths, OUT std::vector<std::string>& names, bool recursive = true, const std::set<std::string>& rejectDirs = {}, const std::set<std::string>& rejectExtension = {});
        }
    }
}

#endif //__SafeSystem_h__
