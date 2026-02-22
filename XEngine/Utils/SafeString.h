#ifndef __SafeString_h__
#define __SafeString_h__

#include "MultiSys.h"
#include <codecvt>
#include <string>
#include <vector>
#include <locale>
#include <climits>   // 包含 INT_MIN、INT_MAX、UINT_MAX 等
#include <cstdint>   // 包含 int32_t、uint64_t 等标准类型
#include <cstdio>    // 包含 sprintf_s（或 Linux 下的 snprintf）

namespace XEngine {
    namespace SafeString {
        __forceinline std::string WStringToUTF8(const std::wstring& wstr) {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            return conv.to_bytes(wstr);
        }

        std::string UTF8ToGBK(const std::string& src);

        std::wstring UnicodeEscapeToWString(const std::string& input);


        __forceinline bool Compare(const std::string& dest, const std::string& src) {
            return dest == src;
        }

        __forceinline bool StringContain(const std::string& src, const std::string& str) {
            return src.find(str) != std::string::npos;
        }
        __forceinline int StringMatch(const std::string& src, const std::string& dst) {
            if (dst.empty()) {
                return 0;
            }
            size_t pos = src.find(dst);
            return (pos != std::string::npos) ? static_cast<int>(pos) : -1;
        }

        __forceinline std::string Replace(const std::string& target, const std::string& src, const std::string& dst) {
            int count = 0;
            std::string::size_type pos = 0;
            std::string::size_type srcLen = src.size();
            std::string::size_type desLen = dst.size();

            std::string res = target;

            pos = res.find(src, pos);
            while ((pos != std::string::npos)) {
                res.replace(pos, srcLen, dst);
                pos = res.find(src, (pos + desLen));
            }

            return res;
        }

        __forceinline int Replace2(std::string& target, const std::string& src, const std::string& dst) {
            int count = 0;
            std::string::size_type pos = 0;
            std::string::size_type srcLen = src.size();
            std::string::size_type desLen = dst.size();
            pos = target.find(src, pos);
            while ((pos != std::string::npos)) {
                target.replace(pos, srcLen, dst);
                pos = target.find(src, (pos + desLen));
                count++;
            }

            return count;
        }

        __forceinline int Split(const std::string& src, const std::string& commas, std::vector<std::string>& res) {
            std::string::size_type pos_1 = 0;
            std::string::size_type pos_2 = 0;

            while (pos_2 != src.npos) {
                pos_1 = src.find_first_not_of(commas, pos_2);
                if (pos_1 == src.npos) {
                    break;
                }

                pos_2 = src.find_first_of(commas, pos_1);
                res.push_back(src.substr(pos_1, pos_2 - pos_1));
            }

            return res.size();
        }

        __forceinline bool StringStartWith(const std::string& src, const std::string& sub) {
            if (src.length() < sub.length()) {
                return false;
            }

            for (int i = 0; i < sub.length(); i++) {
                if (src.c_str()[i] != sub.c_str()[i]) {
                    return false;
                }
            }

            return true;
        }

        __forceinline UInt16 StringToUInt16(const std::string& str) {
            char* endptr = nullptr;
            unsigned long value = std::strtoul(str.c_str(), &endptr, 10);
            if (endptr == str.c_str() || *endptr != '\0' || value > UINT16_MAX) {
                XASSERT(false, "wtf");
                return 0;
            }
            return static_cast<UInt16>(value);
        }

        __forceinline Int16 StringToInt16(const std::string& str) {
            char* endptr = nullptr;
            long value = std::strtol(str.c_str(), &endptr, 10);
            if (endptr == str.c_str() || *endptr != '\0' || value < INT16_MIN || value > INT16_MAX) {
                XASSERT(false, "wtf");
                return 0;
            }
            return static_cast<Int16>(value);
        }

        __forceinline Int32 StringToInt32(const std::string& str) {
            long value = std::atoi(str.c_str());
            if (value < INT_MIN || value > INT_MAX) {
                XASSERT(false, "wtf");
                return 0;
            }
            return static_cast<Int32>(value);
        }

        __forceinline UInt32 StringToUInt32(const std::string& str) {
            char* endptr = nullptr;
            unsigned long value = std::strtoul(str.c_str(), &endptr, 10);
            if (endptr == str.c_str() || *endptr != '\0' || value > UINT_MAX) {
                XASSERT(false, "wtf");
                return 0;
            }
            return static_cast<UInt32>(value);
        }

        __forceinline Int64 StringToInt64(const std::string& str) {
            char* endptr = nullptr;
            long long value = std::strtoll(str.c_str(), &endptr, 10);
            if (endptr == str.c_str() || *endptr != '\0') {
                XASSERT(false, "wtf");
                return 0;
            }
            return static_cast<Int64>(value);
        }

        __forceinline UInt64 StringToUInt64(const std::string& str) {
            char* endptr = nullptr;
            unsigned long long value = std::strtoull(str.c_str(), &endptr, 10);
            if (endptr == str.c_str() || *endptr != '\0') {
                XASSERT(false, "wtf");
                return 0;
            }
            return static_cast<UInt64>(value);
        }

        __forceinline std::string ToLower(const std::string& original) {
            std::string lowerStr;
            lowerStr.reserve(original.size());
            for (char c : original) {
                lowerStr += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
            return lowerStr;
        }

        __forceinline int StringToInt(const std::string& str) {
            return atol(str.c_str());
        }

        __forceinline float StringToFloat(const std::string& str) {
            return (float)atof(str.c_str());
        }

        __forceinline double StringToDouble(const std::string& str) {
            return std::stod(str);
        }

        __forceinline std::string DoubleToString(const double value) {
            return std::to_string(value);
        }

        __forceinline bool StringToBool(const std::string& str) {
            return str == "true";
        }

        __forceinline std::string UInt32ToString(const UInt32 value) {
            char str[128] = { 0 };
            sprintf_s(str, sizeof(str), "%u", value);
            return str;
        }

        __forceinline std::string Int32ToString(const Int32 value) {
            char str[128] = { 0 };
            sprintf_s(str, sizeof(str), "%d", value);
            return str;
        }

        __forceinline std::string UInt64ToString(const UInt64 value) {
            char str[128] = { 0 };
            sprintf_s(str, sizeof(str), "%llu", value);
            return str;
        }

        __forceinline std::string Int64ToString(const Int64 value) {
            char str[128] = { 0 };
            sprintf_s(str, sizeof(str), "%lld", value);
            return str;
        }

        __forceinline std::string FloatToString(const float value) {
            char str[128] = { 0 };
            sprintf_s(str, sizeof(str), "%f", value);
            return str;
        }

        __forceinline std::string BoolToString(const bool res) {
            return res ? "true" : "false";
        }
    }
}


static std::string& operator<<(std::string& src, const std::string& append) {
    src += append;
    return src;
}

static std::string& operator<<(std::string& src, const UInt32 append) {
    src += XEngine::SafeString::Int64ToString(append);
    return src;
}

static std::string& operator<<(std::string& src, const Int32 append) {
    src += XEngine::SafeString::Int64ToString(append);
    return src;
}

static std::string& operator<<(std::string& src, const Int64 append) {
    src += XEngine::SafeString::Int64ToString(append);
    return src;
}

static std::string& operator<<(std::string& src, const UInt64 append) {
    src += XEngine::SafeString::Int64ToString(append);
    return src;
}

static std::string& operator<<(std::string& src, const float append) {
    src += XEngine::SafeString::FloatToString(append);
    return src;
}
#endif //__SafeString_h__
