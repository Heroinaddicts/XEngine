#ifndef __SafeString_h__
#define __SafeString_h__

#include "MultiSys.h"
#include <string>
#include <vector>

namespace XEngine {
    namespace SafeString {
        __forceinline bool Compare(const std::string& dest, const std::string& src) {
            return dest == src;
        }

        __forceinline int Replace(std::string& target, const std::string& src, const std::string& dst) {
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

        __forceinline int64 StringToInt64(const std::string& str) {
            return atoll(str.c_str());
        }

        __forceinline int StringToInt(const std::string& str) {
            return atol(str.c_str());
        }

        __forceinline float StringToFloat(const std::string& str) {
            return atof(str.c_str());
        }

        __forceinline bool StringToBool(const std::string& str) {
            return str == "true";
        }

        __forceinline std::string Int64ToString(const int64 value) {
            char str[128] = { 0 };
            sprintf_s(str, sizeof(str), "%llu", value);
            return str;
        }

        __forceinline std::string BoolToString(const bool res) {
            return res ? "true" : "false";
        }
    }
}

#endif //__SafeString_h__
