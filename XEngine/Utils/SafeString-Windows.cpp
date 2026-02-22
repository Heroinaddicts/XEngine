#ifdef WIN32
#include "SafeString.h"
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace XEngine {
    std::wstring SafeString::UnicodeEscapeToWString(const std::string& input) {
        std::wstring result;
        for (size_t i = 0; i < input.length(); ++i) {
            if (input[i] == '\\' && i + 1 < input.length() && input[i + 1] == 'u') {
                unsigned int codePoint = 0;
                sscanf(input.substr(i + 2, 4).c_str(), "%x", &codePoint);
                result += static_cast<wchar_t>(codePoint);
                i += 5;
            }
            else {
                result += static_cast<wchar_t>(input[i]);
            }
        }
        return result;
    }
}
#endif //WIN32