#ifndef __SafeTools_h__
#define __SafeTools_h__

#include "MultiSys.h"
#include "SafeSystem.h"

namespace XEngine {
    namespace SafeTools {
        template <typename T>
        struct T_Hash {
            size_t operator()(const T& src) const {
                return (size_t)src;
            }
        };

        template <typename T>
        struct T_Compare {
            bool operator()(const T& src, const T& dst) const {
                return src == dst;
            }
        };

        static unsigned_int64 s_Seed = 0;
        inline void SetSeed(const unsigned_int64 seed) {
            s_Seed = seed;
        }

        inline void SetSeed() {
            SetSeed(SafeSystem::Time::GetMilliSecond());
        }

        inline int Rand(int range) {
            if (0 == range) {
                return 0;
            }
            if (s_Seed == 0) {
                s_Seed = SafeSystem::Time::GetMilliSecond();
            }

            s_Seed = (((s_Seed = s_Seed * 214013L + 2531011L) >> 16) & 0x7fff);
            return s_Seed % range;
        }
    }
}

#endif //__SafeTools_h__
