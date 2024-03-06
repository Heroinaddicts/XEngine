#ifndef __Triangle_h__
#define __Triangle_h__

#include "Vector3.h"

namespace XEngine {
    struct Triangle {
        Vector3 v1;
        Vector3 v2;
        Vector3 v3;

        Triangle() {}
        Triangle(const Vector3& v1_, const Vector3& v2_, const Vector3& v3_) : v1(v1_), v2(v2_), v3(v3_) {}
    };
}

#endif //__Triangle_h__
