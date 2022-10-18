#ifndef __Quaternion_h__
#define __Quaternion_h__

#include "Vector.h"

namespace XEngine {
    class Quaternion {
    public:
        virtual ~Quaternion() {}
        Quaternion() :x(0), y(0), z(0), w(0) {}
        Quaternion(const Quaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
        Quaternion(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

        static Quaternion Euler(const Vector3& v);
        Vector3 EulerAngles() const;

        float x;
        float y;
        float z;
        float w;
    };
}

#endif //__Quaternion_h__
