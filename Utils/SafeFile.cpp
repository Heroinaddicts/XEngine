#include "SafeFile.h"
#include "SafeString.h"

#ifdef WIN32
#include <Shlwapi.h>
#include <direct.h>  
#include <io.h>  
#include <windows.h>
#include <locale.h>
#pragma comment(lib, "shlwapi.lib")
#endif //WIN32

namespace XEngine {
    namespace SafeFile {
        const char* GetApplicationPath() {
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
            return static_path->c_str();
        }
    }
}
