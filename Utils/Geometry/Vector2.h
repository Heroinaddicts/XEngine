#ifndef __Vector2_h__
#define __Vector2_h__

#include "Geometry.h"

namespace XEngine {
    struct Vector3;

    struct Vector2 {
        Vector2(const float x_, const float y_) : x(x_), y(y_) {}
        Vector2(const Vector2& v) : x(v.x), y(v.y) {}
        Vector2() : x(0), y(0) {}

        Vector2& operator=(const float* v) {
            x = v[0];
            y = v[1];
            return *this;
        }

        Vector2 operator-(const Vector2& v) const {
            return Vector2(x - v.x, y - v.y);
        }

        Vector2 operator+(const Vector2& v) const {
            return Vector2(x + v.x, y + v.y);
        }

        Vector3 ToV3();

        __forceinline float Length() const {
            return sqrt(x * x + y * y);
        }

        __forceinline float Angle() const {
            float angle = atan2(y, x) * 180.0f / XPI;
            if (angle < 0) angle += 360.0f;
            return angle;
        }

        float x;
        float y;
    };

    struct Vector2Int {
        Vector2Int(const int x_, const int y_) : x(x_), y(y_) {}
        Vector2Int(const Vector2Int& v) : x(v.x), y(v.y) {}
        Vector2Int() : x(0), y(0) {}

        Vector2Int& operator=(const int* v) {
            x = v[0];
            y = v[1];
            return *this;
        }

        bool operator<(const Vector2Int& target) const {
            return (x + y) < (target.x + target.y);
        }

        int x;
        int y;
    };

}

#endif //__Vector2_h__
