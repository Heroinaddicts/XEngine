#include "SafeSystem.h"
#include "SafeString.h"
#include "SafeMemory.h"

#include <stdio.h>

#ifdef WIN32
#include <Shlwapi.h>
#include <direct.h>  
#include <io.h>  
#include <windows.h>
#include <locale.h>
#pragma comment(lib, "shlwapi.lib")
#endif //WIN32

#ifdef Linux
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <dirent.h>
#include <libgen.h>
#include <pthread.h>
#endif //Linux

namespace XEngine {
    namespace SafeSystem {
        namespace Time {
            void MillisecondSleep(const int millisecond) {
#ifdef WIN32
                Sleep(millisecond);
#endif //WIN32

#ifdef Linux
                usleep(millisecond * 1000);
#endif //Linux
            }
        }

        namespace Process {
            unsigned_int64 GetCurrentProcessID() {
#ifdef WIN32
                return ::GetCurrentProcessId();
#endif //WIN32

#ifdef Linux
                return ::getpid();
#endif //Linux
            }

            unsigned_int64 GetCurrentThreadID() {
#ifdef WIN32
                return ::GetCurrentThreadId();
#endif //WIN32

#ifdef Linux
                return syscall(SYS_gettid);
#endif //Linux
            }
        }

        namespace File {
            const std::string& GetApplicationPath() {
#define PATH_STRING_MAX_LEN 1024
                static std::string* static_path = nullptr;
#ifdef WIN32
                if (nullptr == static_path) {
                    char* temp = (char*)alloca(PATH_STRING_MAX_LEN);
                    char link[PATH_STRING_MAX_LEN];
                    ZeroMemory(link, sizeof(link));
                    memset(temp, 0, PATH_STRING_MAX_LEN);
                    GetModuleFileName(nullptr, temp, 256);
                    PathRemoveFileSpec(temp);
                    static_path = new std::string(temp);
                    SafeString::Replace(*static_path, "\\", "/");
                }
#endif //WIN32
#ifdef Linux
#define SYSTEM_PATH_LEN 2048
                if (nullptr == static_path) {
                    char buff[SYSTEM_PATH_LEN];
                    char link[SYSTEM_PATH_LEN];
                    SafeMemory::Memset(buff, sizeof(buff), 0, sizeof(buff));
                    SafeMemory::Memset(link, sizeof(link), 0, sizeof(link));

                    sprintf_s(link, sizeof(link), "/proc/self/exe");
                    int count = readlink(link, buff, sizeof(buff));
                    if (count >= sizeof(buff)) {
                        XASSERT(false, "system path error");
                        return nullptr;
                    }

                    const char * p = dirname(buff);
                    static_path = xnew std::string(p);
                }
#endif //Linux
                return *static_path;
            }

            bool FileExists(const std::string& path) {
#ifdef WIN32
                return _access(path.c_str(), 0) == 0;
#endif //WIN32
            }

            bool FolderExists(const std::string& path) {
#ifdef WIN32
                return::PathIsDirectory(path.c_str());
#endif //WIN32
            }

            bool CreateFolder(const std::string& path) {
#ifdef WIN32
                if (::PathIsDirectory(path.c_str())) {
                    return true;
                }
                return ::CreateDirectory(path.c_str(), 0);
#endif //WIN32
            }

            bool DelFolder(const std::string& path) {
                return remove(path.c_str()) == 0;
            }

            bool DelFile(const std::string& path) {
                return remove(path.c_str()) == 0;
            }

            int GetFileInDirectory(const std::string& dir, const std::string& extension, OUT std::vector<std::string>& paths, OUT std::vector<std::string>& names, bool recursive) {
                int count = 0;
#ifdef WIN32
                WIN32_FIND_DATA finder;
                HANDLE error;
                int filecount = 0;
                char path[512] = { 0 };

                sprintf_s(path, sizeof(path), "%s/*.*", dir.c_str());
                error = FindFirstFile(path, &finder);
                if (INVALID_HANDLE_VALUE == error) {
                    return false;
                }

                while (::FindNextFile(error, &finder)) {
                    if (strcmp(finder.cFileName, ".") == 0
                        || strcmp(finder.cFileName, "..") == 0) {
                        continue;
                    }

                    sprintf_s(path, "%s/%s", dir.c_str(), finder.cFileName);
                    if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && recursive) {
                        count += GetFileInDirectory(path, extension, paths, names, recursive);
                    }
                    else {
                        if (0 == strcmp(extension.c_str(), PathFindExtension(finder.cFileName))
                            || 0 == strcmp(extension.c_str(), "*")) {
                            paths.push_back(path);
                            PathRemoveExtension(finder.cFileName);
                            names.push_back(finder.cFileName);
                            count++;
                        }
                    }
                }
#endif //WIN32

                return count;
            }
        }
    }
}

