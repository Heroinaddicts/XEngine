#include "SafeSystem.h"
#include "SafeString.h"
#include "SafeMemory.h"

#include <stdio.h>
#include <thread>

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
#include <spawn.h>
#include <pthread.h>
#endif //Linux

namespace XEngine {
    namespace SafeSystem {
        namespace Time {
            void MillisecondSleep(const int millisecond) {
                std::this_thread::sleep_for(std::chrono::microseconds(millisecond * 1000));
            }
        }

        namespace Process {
            UInt64 GetCurrentProcessID() {
#ifdef WIN32
                return ::GetCurrentProcessId();
#endif //WIN32

#ifdef Linux
                return ::getpid();
#endif //Linux
            }

            UInt64 GetCurrentThreadID() {
#ifdef WIN32
                return ::GetCurrentThreadId();
#endif //WIN32

#ifdef Linux
                return syscall(SYS_gettid);
#endif //Linux
            }

            UInt64 LaunchProcess(const std::string& exPath, const std::string& args) {
#ifdef WIN32
                STARTUPINFOA si;
                SafeMemory::Memset(&si, sizeof(si), 0, sizeof(si));
                si.cb = sizeof(si);

                PROCESS_INFORMATION pi;
                SafeMemory::Memset(&pi, sizeof(pi), 0, sizeof(pi));

                bool ret = CreateProcessA(exPath.c_str(), (char*)args.c_str(), nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);

                if (!ret) {
                    return UInt64(0);
                }

                return (UInt64)pi.hProcess;
#endif //WIN32
#ifdef Linux
                char* argv[] = { (char*)args.c_str(), nullptr };
                pid_t pid;
                int ret = posix_spawnp(&pid, SafeSystem::File::GetCurrentExecutablePath().c_str(), nullptr, nullptr, argv, nullptr);
                if (0 == ret) {
                    return pid;
                }
                return INVAILD_ID;
#endif //Linux
            }
        }

        namespace File {
#define PATH_STRING_MAX_LEN 1024

            const std::string& GetCurrentExecutablePath() {
                static std::string* s_Path = nullptr;
                if (nullptr == s_Path) {
#ifdef WIN32
                    char temp[PATH_STRING_MAX_LEN];
                    GetModuleFileNameA(nullptr, temp, sizeof(temp));
                    s_Path = xnew std::string(temp);
                    SafeString::Replace(*s_Path, "\\", "/");
#endif //WIN32
                }

                return *s_Path;
            }

            const std::string& GetDirectoryOfCurrentExe() {
                static std::string* s_Path = nullptr;
                if (nullptr == s_Path) {
#ifdef WIN32
                    char* temp = (char*)alloca(PATH_STRING_MAX_LEN);
                    char link[PATH_STRING_MAX_LEN];
                    ZeroMemory(link, sizeof(link));
                    memset(temp, 0, PATH_STRING_MAX_LEN);
                    GetModuleFileNameA(nullptr, temp, PATH_STRING_MAX_LEN);
                    PathRemoveFileSpecA(temp);
                    s_Path = new std::string(temp);
                    SafeString::Replace(*s_Path, "\\", "/");
#endif //WIN32
#ifdef Linux
#define SYSTEM_PATH_LEN 2048
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

                    const char* p = dirname(buff);
                    s_Path = xnew std::string(p);
#endif //Linux
                }
                return *s_Path;
            }

            bool FileExists(const std::string& path) {
#ifdef WIN32
                return _access(path.c_str(), 0) == 0;
#endif //WIN32
            }

            bool FolderExists(const std::string& path) {
#ifdef WIN32
                return::PathIsDirectoryA(path.c_str());
#endif //WIN32
            }

            bool CreateFolder(const std::string& path) {
#ifdef WIN32
                if (::PathIsDirectoryA(path.c_str())) {
                    return true;
                }
                return ::CreateDirectoryA(path.c_str(), 0);
#endif //WIN32
            }

            bool DelFolder(const std::string& path) {
                return remove(path.c_str()) == 0;
            }

            bool DelFile(const std::string& path) {
                return remove(path.c_str()) == 0;
            }

            __forceinline const char* GetFilenameExt(const char* filename) {
                const char* dot = strrchr(filename, '.');
                if (!dot || dot == filename) return "";
                return dot;
            }

            int GetFileInDirectory(const std::string& dir, const std::string& extension, OUT std::vector<std::string>& paths, OUT std::vector<std::string>& names, bool recursive, const std::set<std::string>& rejectDirs, const std::set<std::string>& rejectExtension) {
                int count = 0;
#ifdef WIN32
                WIN32_FIND_DATAA finder;
                HANDLE error;
                int filecount = 0;
                char path[512] = { 0 };

                sprintf_s(path, sizeof(path), "%s/*.*", dir.c_str());
                error = FindFirstFileA(path, &finder);
                if (INVALID_HANDLE_VALUE == error) {
                    return false;
                }

                while (::FindNextFileA(error, &finder)) {
                    if (strcmp(finder.cFileName, ".") == 0
                        || strcmp(finder.cFileName, "..") == 0) {
                        continue;
                    }

                    sprintf_s(path, "%s/%s", dir.c_str(), finder.cFileName);
                    if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && recursive) {
                        if (rejectDirs.find(finder.cFileName) != rejectDirs.cend()) {
                            continue;
                        }

                        count += GetFileInDirectory(path, extension, paths, names, recursive);
                    }
                    else {
                        if (0 == strcmp(extension.c_str(), PathFindExtensionA(finder.cFileName))
                            || 0 == strcmp(extension.c_str(), "*")) {
                            if (rejectExtension.find(PathFindExtensionA(finder.cFileName)) != rejectExtension.cend()) {
                                continue;
                            }

                            paths.push_back(path);
                            PathRemoveExtensionA(finder.cFileName);
                            names.push_back(finder.cFileName);
                            count++;
                        }
                    }
                }
#endif //WIN32

#ifdef Linux
                DIR* dp;
                struct dirent* dirp;
                struct stat st;

                /* open dirent directory */
                if ((dp = opendir(dir.c_str())) == nullptr) {
                    perror("opendir");
                    return 0;
                }

                /**
                * read all files in this dir
                **/
                while ((dirp = readdir(dp)) != nullptr) {
                    char fullname[255];
                    memset(fullname, 0, sizeof(fullname));

                    /* ignore hidden files */
                    if (dirp->d_name[0] == '.')
                        continue;

                    strncpy(fullname, dir.c_str(), sizeof(fullname));
                    strncat(fullname, "/", sizeof(fullname));
                    strncat(fullname, dirp->d_name, sizeof(fullname));
                    /* get dirent status */
                    if (stat(fullname, &st) == -1) {
                        perror("stat");
                        fputs(fullname, stderr);
                        return 0;
                    }

                    /* if dirent is a directory, call itself */
                    if (S_ISDIR(st.st_mode)) {
                        if (recursive) {
                            count += GetFileInDirectory(dir, extension, paths, names, recursive);
                        }
                    }
                    else {
                        /* display file name with proper tab */
                        //printf("%s/%s\n", dirname, dirp->d_name);
                        if (strcmp(GetFilenameExt(dirp->d_name), extension.c_str()) == 0) {
                            paths.push_back(std::string(dir.c_str()) + "/" + dirp->d_name);
                            char name[255];
                            strncpy(name, dirp->d_name, sizeof(name));
                            char* dot = strrchr(name, '.');
                            XASSERT(dot, "wtf");
                            *dot = 0;
                            names.push_back(name);
                            count++;
                        }
                    }
                }
#endif //Linux
                return count;
            }
        }
    }
}

