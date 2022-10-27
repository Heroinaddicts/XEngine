#ifndef __Quaternion_h__
#define __Quaternion_h__

#include "Vector.h"

namespace XEngine {
    class Quaternion {
    public:
        virtual ~Quaternion() {}
        Quaternion() :_X(0), _Y(0), _Z(0), _W(0) {}
        Quaternion(const Quaternion& q) : _X(q._X), _Y(q._Y), _Z(q._Z), _W(q._W) {}
        Quaternion(float x_, float y_, float z_, float w_) : _X(x_), _Y(y_), _Z(z_), _W(w_) {}

        static Quaternion Euler(const Vector3& v);
        Vector3 EulerAngles() const;

        float _X;
        float _Y;
        float _Z;
        float _W;
    };
}

#endif //__Quaternion_h__
