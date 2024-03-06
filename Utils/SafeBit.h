#ifndef __SafeBit_h__
#define __SafeBit_h__

#include "MultiSys.h"
#include <vector>
#include <bitset>

namespace XEngine {
    namespace SafeBit {
        typedef std::vector<bool> Bits;

        static Bits& operator<<(Bits& bits, const bool bit) {
            bits.push_back(bit);
            return bits;
        }

        __forceinline Bits CharToBits(Int8 c) {
            std::bitset<8> bitset(c);
            Bits bits;
            for (size_t i = 0; i < 8; ++i) {
                bits.push_back(bitset[7 - i]);
            }
            return bits;
        }

        // 从 Bits 转换为 Int8
        __forceinline Int8 BitsToInt8(const Bits& bits) {
            std::bitset<8> bitset;
            for (size_t i = 0; i < 8 && i < bits.size(); ++i) {
                bitset[7 - i] = bits[i];
            }
            return static_cast<Int8>(bitset.to_ulong());
        }

        // 从 Int32 转换为 Bits
        __forceinline Bits IntToBits(Int32 value) {
            std::bitset<32> bitset(value);
            Bits bits;
            for (size_t i = 0; i < 32; ++i) {
                bits.push_back(bitset[31 - i]);
            }
            return bits;
        }

        // 从 Bits 转换为 Int32
        __forceinline Int32 BitsToInt(const Bits& bits) {
            std::bitset<32> bitset;
            for (size_t i = 0; i < 32 && i < bits.size(); ++i) {
                bitset[31 - i] = bits[i];
            }
            return static_cast<Int32>(bitset.to_ulong());
        }

        // 从 Int64 转换为 Bits
        __forceinline Bits Int64ToBits(Int64 value) {
            std::bitset<64> bitset(value);
            Bits bits;
            for (size_t i = 0; i < 64; ++i) {
                bits.push_back(bitset[63 - i]);
            }
            return bits;
        }

        // 从 Bits 转换为 Int64
        __forceinline Int64 BitsToInt64(const Bits& bits) {
            std::bitset<64> bitset;
            for (size_t i = 0; i < 64 && i < bits.size(); ++i) {
                bitset[63 - i] = bits[i];
            }
            return static_cast<Int64>(bitset.to_ullong());
        }
    }
}

#endif //__SafeBit_h__
