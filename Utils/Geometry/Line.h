#ifndef __Line_h__
#define __Line_h__

#include "Vector2.h"
#include "Vector3.h"

namespace XEngine {
    class Line2 {
    public:
        virtual ~Line2() {}

        Vector2 _Start;
        Vector2 _End;

        __forceinline Vector2 V() const {
            return _End - _Start;
        }
    };

    class Line3 {
    public:
        virtual ~Line3() {}

        Vector3 _Start;
        Vector3 _End;

        __forceinline Vector3 V() const {
            return _End - _Start;
        }
    };
}

#endif //__Line_h__
