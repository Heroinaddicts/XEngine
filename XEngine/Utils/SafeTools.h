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

        static unsigned_int64 static_seed = 0;
        inline void SetSeed(const unsigned_int64 seed) {
            static_seed = seed;
        }

        inline void SetSeed() {
            SetSeed(SafeSystem::Time::GetMilliSecond());
        }

        inline int Rand(int range) {
            if (0 == range) {
                return 0;
            }
            if (static_seed == 0) {
                static_seed = SafeSystem::Time::GetMilliSecond();
            }

            static_seed = (((static_seed = static_seed * 214013L + 2531011L) >> 16) & 0x7fff);
            return static_seed % range;
        }
    }
}

#endif //__SafeTools_h__
