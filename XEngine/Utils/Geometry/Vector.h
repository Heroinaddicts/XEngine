#ifndef __Vector_h__
#define __Vector_h__

#include "Geometry.h"

namespace XEngine {
    struct Face {
        unsigned int u; //������
        unsigned int v; // ��ͼ������
        unsigned int w;	// ���������
    };

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

        float x;
        float y;
    };

    struct Vector3 {
        static const Vector3 zero;

        __forceinline Vector3(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {}
        __forceinline Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
        __forceinline Vector3() : x(0), y(0), z(0) {}

        __forceinline float Length() const {
            return (float)sqrt(x * x + y * y + z * z);
        }

        __forceinline Vector3 Normalized(float epilon = Epsilon) const {
            float len = Length();
            if (len > epilon) {
                return (*this) / len;
            }
            return Vector3::zero;
        }

        __forceinline Vector3& operator=(const float* v) {
            x = v[0];
            y = v[1];
            z = v[2];
            return *this;
        }

        __forceinline Vector3 operator-(const Vector3& v) const {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }

        __forceinline Vector3 operator+(const Vector3& v) const {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }

        __forceinline Vector3& operator *= (const float s) {
            x *= s; y *= s; z *= s;
            return *this;
        }

        __forceinline Vector3 operator * (const float s) const {
            return Vector3(x * s, y * s, z * s);
        }

        __forceinline Vector3 operator / (const float s) const {
            float invS = 1.0f / s;
            return Vector3(x * invS, y * invS, z * invS);
        }

        __forceinline Vector3& operator /= (const float s) {
            float invS = 1.0f / s;
            x *= invS; y *= invS; z *= invS;
            return *this;
        }

        float x;
        float y;
        float z;
    };

    struct Vector2Int {
        Vector2Int(const int x_, const int y_, const int z_) : x(x_), y(y_) {}
        Vector2Int(const Vector2Int& v) : x(v.x), y(v.y) {}
        Vector2Int() : x(0), y(0) {}

        Vector2Int& operator=(const int* v) {
            x = v[0];
            y = v[1];
            return *this;
        }

        int x;
        int y;
    };

    struct Vector3Int {
        Vector3Int(const int x_, const int y_, const int z_) : x(x_), y(y_), z(z_) {}
        Vector3Int(const Vector3Int& v) : x(v.x), y(v.y), z(v.z) {}
        Vector3Int() : x(0), y(0), z(0) {}

        Vector3Int& operator=(const int* v) {
            x = v[0];
            y = v[1];
            z = v[2];
            return *this;
        }

        int x;
        int y;
        int z;
    };
}

#endif //__Vector_h__
