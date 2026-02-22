#ifndef __SafeMemory_h__
#define __SafeMemory_h__

#include "MultiSys.h"
#include <string.h>

namespace XEngine {
    namespace SafeMemory {
        const int Kb = 1024;
        const int Mb = 1024 * Kb;
        const int Gb = 1024 * Mb;

        __forceinline void Memset(void* __restrict __dest, size_t __max, int val, size_t __n) {
            XASSERT(__n <= __max, "SafeMemory Memset Over Flow");
            memset(__dest, val, (__max >= __n) ? (__n) : (__max));
        }

        __forceinline void Memcpy(void* __restrict __dest, size_t __max, const void* __restrict __src, size_t __n) {
            XASSERT(__n <= __max, "Memcpy Over Flow");
            memcpy(__dest, __src, (__max >= __n) ? (__n) : (__max));
        }

        __forceinline void Memmove(void* __restrict __dest, size_t __max, const void* __restrict __src, size_t __n) {
            XASSERT(__n <= __max, "Memmove Over Flow");
            memmove(__dest, __src, (__max >= __n) ? (__n) : (__max));
        }

        __forceinline bool Strcmp(const char* __restrict __dest, const char* __restrict __src) {
            return !strcmp(__dest, __src);
        }

        void* AlignedMalloc(size_t size, size_t alignment);
        void AlignedFree(void* p);
    }
}

#endif //__SafeMemory_h__
