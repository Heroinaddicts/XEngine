#ifndef __XBase64_h__
#define __XBase64_h__

#include <string>
#include <vector>
#include <stdexcept>

namespace XEngine {
    namespace XBase64 {
        static const char* _chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

        __forceinline bool _isBase64(char c) {
            return (c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9') ||
                c == '+' || c == '/';
        }

        // Base64 ±àÂë
        __forceinline std::string Encode(const std::string& input) {
            const unsigned char* bytes_to_encode = reinterpret_cast<const unsigned char*>(input.data());
            size_t in_len = input.size();
            std::string ret;
            ret.reserve(((in_len + 2) / 3) * 4);

            int i = 0;
            unsigned char char_array_3[3];
            unsigned char char_array_4[4];

            while (in_len--) {
                char_array_3[i++] = *(bytes_to_encode++);
                if (i == 3) {
                    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                    char_array_4[3] = char_array_3[2] & 0x3f;

                    for (int j = 0; j < 4; ++j)
                        ret += _chars[char_array_4[j]];
                    i = 0;
                }
            }

            if (i) {
                for (int j = i; j < 3; ++j)
                    char_array_3[j] = 0;

                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (int j = 0; j < i + 1; ++j)
                    ret += _chars[char_array_4[j]];

                while (i++ < 3)
                    ret += '=';
            }

            return ret;
        }

        // Base64 ½âÂë
        __forceinline std::string Decode(const std::string& input) {
            size_t in_len = input.size();
            if (in_len % 4 != 0)
                throw std::invalid_argument("Invalid Base64 input length");

            int i = 0;
            int in_ = 0;
            unsigned char char_array_4[4], char_array_3[3];
            std::string ret;
            ret.reserve((in_len / 4) * 3);

            while (in_len-- && (input[in_] != '=') && _isBase64(input[in_])) {
                char_array_4[i++] = input[in_]; in_++;
                if (i == 4) {
                    for (int j = 0; j < 4; ++j)
                        char_array_4[j] = static_cast<unsigned char>(strchr(_chars, char_array_4[j]) - _chars);

                    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                    for (int j = 0; j < 3; ++j)
                        ret += char_array_3[j];
                    i = 0;
                }
            }

            if (i) {
                for (int j = i; j < 4; ++j)
                    char_array_4[j] = 0;

                for (int j = 0; j < 4; ++j)
                    char_array_4[j] = static_cast<unsigned char>(strchr(_chars, char_array_4[j]) - _chars);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (int j = 0; j < i - 1; ++j)
                    ret += char_array_3[j];
            }

            return ret;
        }
    }
}


#endif //__XBase64_h__
