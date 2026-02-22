#ifndef __RedisDef_h__
#define __RedisDef_h__
#include <cstdarg>
#include <string>
#include <vector>

namespace RedisDef {
    constexpr const char* AccountToken = "%lld-Token";
    constexpr const char* Nickname = "%s-Nickname";

    __forceinline std::string GenerateRedisKey(const char* keyTemplate, ...) {
        char buffer[512];
        va_list args;
        va_start(args, keyTemplate);
        int ret = vsnprintf(buffer, sizeof(buffer), keyTemplate, args);
        va_end(args);
        return buffer;
    }

}

#endif //__RedisDef_h__
