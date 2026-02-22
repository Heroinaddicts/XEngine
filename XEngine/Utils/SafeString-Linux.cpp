#ifdef Linux
#include "SafeString.h"
#include <string>
#include <vector>
#include <iconv.h>
#include <cerrno>
#include <cstring>

namespace XEngine {
    __forceinline iconv_t OpenIconv(const char* tocode, const char* fromcode) {
        // 依次尝试：带 TRANSLIT、IGNORE、纯编码、以及 GB18030/CP936 兼容名
        const char* candidates[][2] = {
            { "GBK//TRANSLIT", fromcode },
            { "GBK//IGNORE",   fromcode },
            { "GBK",           fromcode },
            { "GB18030",       fromcode },
            { "CP936",         fromcode },   // 一些系统上 GBK 的别名
        };
        for (auto& c : candidates) {
            iconv_t cd = iconv_open(c[0], c[1]);
            if (cd != (iconv_t)-1) return cd;
        }
        return (iconv_t)-1;
    }

    __forceinline std::string IconvConvert(const std::string& input,
        const char* tocode,
        const char* fromcode) {
        iconv_t cd = OpenIconv(tocode, fromcode);
        if (cd == (iconv_t)-1) {
            // 打不开转换器：返回原文或空串看你需求，这里返回空串
            return {};
        }

        // 经验值：GBK 通常 <= UTF-8 字节数；但为安全，给 2x + 余量
        size_t in_left = input.size();
        const char* in = input.data();

        std::string output;
        output.resize(in_left * 2 + 32);
        char* out = output.data();
        size_t out_left = output.size();

        while (in_left > 0) {
            size_t r = iconv(cd,
#if defined(__GNUC__)
                const_cast<char**>(&in),
#else
                (char**)(&in),
#endif
                & in_left, &out, &out_left);

            if (r != (size_t)-1) break; // 转换完成

            if (errno == E2BIG) {
                // 输出缓冲区不够：扩容并继续
                const size_t used = static_cast<size_t>(out - output.data());
                output.resize(output.size() * 2 + in_left * 2 + 32);
                out = output.data() + used;
                out_left = output.size() - used;
                continue;
            }
            else if (errno == EILSEQ || errno == EINVAL) {
                // 非法/不完整的输入序列：跳过 1 字节并输出占位符
                if (in_left > 0) { ++in; --in_left; }
                if (out_left > 0) { *out++ = '?'; --out_left; }
                continue;
            }
            else {
                // 其它错误：放弃
                iconv_close(cd);
                return {};
            }
        }

        // flush shift state（一般无事，但按规范调用一次）
        (void)iconv(cd, nullptr, nullptr, &out, &out_left);

        const size_t used = static_cast<size_t>(out - output.data());
        output.resize(used);
        iconv_close(cd);
        return output;
    }

    // 你需要的接口
    std::string SafeString::UTF8ToGBK(const std::string& src) {
#if defined(__linux__) || defined(__APPLE__)
        return IconvConvert(src, "GBK", "UTF-8");
#else
        // 非 Linux/Unix 平台（如 Windows）请用 WideChar 路线：
        // MultiByteToWideChar(U TF-8) -> WideCharToMultiByte(CP_ACP/936)
        return {};
#endif
    }

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

#endif //Linux