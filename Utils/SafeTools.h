#ifndef __SafeTools_h__
#define __SafeTools_h__

#include "MultiSys.h"

namespace XEngine {
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
}

#endif //__SafeTools_h__
