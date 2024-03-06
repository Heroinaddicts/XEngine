#include "SafeString.h"
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace XEngine {
    std::string SafeString::GBKToUTF8(const std::string& src) {
        int len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, nullptr, 0);
        wchar_t* wstr = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, wstr, len);
        len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
        char* str = (char*)malloc(len + 1);
        memset(str, 0, len + 1);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, nullptr, nullptr);
        std::string ret(str);
        free(str);
        free(wstr);
        return ret;
    }


    std::string SafeString::UTF8ToGBK(const std::string& src) {
        int wideStrLength = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, nullptr, 0);

        // ·ÖÅä¿í×Ö·û»º³åÇø
        wchar_t* wideString = new wchar_t[wideStrLength];
        // ½«UTF-8×Ö·û´®×ª»»Îª¿í×Ö·û×Ö·û´®
        MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, wideString, wideStrLength);
        // ¼ÆËãGBK×Ö·û´®µÄ³¤¶È
        int gbkStrLength = WideCharToMultiByte(CP_ACP, 0, wideString, -1, nullptr, 0, nullptr, nullptr);
        // ·ÖÅäGBK×Ö·û´®»º³åÇø
        char* gbkString = new char[gbkStrLength];
        // ½«¿í×Ö·û×Ö·û´®×ª»»ÎªGBK×Ö·û´®
        WideCharToMultiByte(CP_ACP, 0, wideString, -1, gbkString, gbkStrLength, nullptr, nullptr);
        // ÊÍ·ÅÄÚ´æ
        delete[] wideString;
        std::string ret(gbkString);
        delete[] gbkString;
        return ret;
    }
}
