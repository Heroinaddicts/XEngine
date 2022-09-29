#ifndef __Vector3_h__
#define __Vector3_h__

namespace XEngine {
    struct Vector3 {
        Vector3(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {}
        Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

        Vector3& operator=(const float* v) {
            x = v[0];
            y = v[1];
            z = v[2];
            return *this;
        }

        float x;
        float y;
        float z;
    };
}

#endif //__Vector3_h__
