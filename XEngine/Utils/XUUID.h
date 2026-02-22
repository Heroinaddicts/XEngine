#ifndef __XUUID_h__
#define __XUUID_h__

#include <iomanip>
#include <random>
#include <sstream>
namespace XEngine {

    // 生成一个随机的 16 位十六进制数
    __forceinline std::string GenerateHEX(const int length) {
        std::random_device rd; // 随机设备
        std::mt19937 gen(rd()); // 随机数生成器
        std::uniform_int_distribution<> dis(0, 15); // 均匀分布

        std::stringstream ss;
        for (int i = 0; i < length; ++i) {
            ss << std::hex << dis(gen); // 生成十六进制字符
        }
        return ss.str();
    }

    // 生成一个类似 UUID 的字符串
    __forceinline std::string GenerateUUID() {
        std::string uuid = GenerateHEX(8) + "-" + // 8 位
            GenerateHEX(4) + "-" + // 4 位
            GenerateHEX(4) + "-" + // 4 位
            GenerateHEX(4) + "-" + // 4 位
            GenerateHEX(12); // 12 位
        return uuid;
    }
}

#endif //__XUUID_h__
