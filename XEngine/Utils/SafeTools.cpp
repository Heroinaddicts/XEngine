#include "SafeTools.h"
#include <bitset>

namespace XEngine {
    namespace SafeTools {
        const std::string s_Base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::string Base64Encode(const std::string& src) {
            std::string encode;
            int paddingCount = 0;

            // 计算 padding 个数
            int len = src.length();
            if (len % 3 != 0) {
                paddingCount = 3 - (len % 3);
            }

            // 依次对三个字符一组进行编码
            for (int i = 0; i < len; i += 3) {
                int a = static_cast<int>(src[i]);
                int b = (i + 1 < len) ? static_cast<int>(src[i + 1]) : 0;
                int c = (i + 2 < len) ? static_cast<int>(src[i + 2]) : 0;

                int value = (a << 16) | (b << 8) | c;
                for (int j = 0; j < 4; j++) {
                    int index = (value >> (18 - j * 6)) & 0x3f;
                    encode += s_Base64Chars[index];
                }
            }

            // 添加 padding
            for (int i = 0; i < paddingCount; i++) {
                encode += '=';
            }

            return encode;
        }

        std::string Base64Decode(const std::string& src) {
            std::string decode;
            int paddingCount = 0;

            // 计算 padding 个数
            int len = src.length();
            if (len > 0 && src[len - 1] == '=') {
                paddingCount++;
            }
            if (len > 1 && src[len - 2] == '=') {
                paddingCount++;
            }

            // 依次对四个字符一组进行解码
            for (int i = 0; i < len; i += 4) {
                int a = s_Base64Chars.find(src[i]);
                int b = (i + 1 < len) ? s_Base64Chars.find(src[i + 1]) : 0;
                int c = (i + 2 < len) ? s_Base64Chars.find(src[i + 2]) : 0;
                int d = (i + 3 < len) ? s_Base64Chars.find(src[i + 3]) : 0;

                int value = (a << 18) | (b << 12) | (c << 6) | d;

                for (int j = 0; j < 3 && (i + j * 3) < (len - paddingCount); j++) {
                    char ch = static_cast<char>((value >> (16 - j * 8)) & 0xff);
                    decode += ch;
                }
            }

            return decode;
        }
    }
}
