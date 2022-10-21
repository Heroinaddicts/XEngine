#include "SafeSystem.h"
#include "SafeString.h"

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
            }

            unsigned_int64 GetCurrentThreadID() {
#ifdef WIN32
                return ::GetCurrentThreadId();
#endif //WIN32
            }
        }

        namespace File {
            const std::string& GetApplicationPath()
            {
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
                return *static_path;
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

