#ifndef __SafeTools_h__
#define __SafeTools_h__

#include "MultiSys.h"
#include "SafeSystem.h"

namespace XEngine {
    namespace SafeTools {
        template <typename T, size_t(*fun)(const T& src)>
        struct T_Hash {
            size_t operator()(const T& src) const {
                return fun(src);
            }
        };

        template <typename T, bool(*fun)(const T& src, const T& dst)>
        struct T_Compare {
            bool operator()(const T& src, const T& dst) const {
                return fun(src, dst);
            }
        };

        static UInt64 s_Seed = 0;
        inline void SetSeed(const UInt64 seed) {
            s_Seed = seed;
        }

        inline void SetSeed() {
            SetSeed(SafeSystem::Time::GetMilliSecond());
        }

        inline UInt32 Rand(UInt32 range = 0xffff) {
            if (0 == range) {
                return 0;
            }
            if (s_Seed == 0) {
                s_Seed = SafeSystem::Time::GetMicroSecond();
            }

            s_Seed = (((s_Seed = s_Seed * 214013L + 2531011L) >> 16) & 0x7fff);
            return s_Seed % range;
        }

        std::string Base64Encode(const std::string& src);
        std::string Base64Decode(const std::string& src);
    }
}

#endif //__SafeTools_h__
