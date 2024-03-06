#ifndef __Vector3_h__
#define __Vector3_h__

#include "Geometry.h"

namespace XEngine {
    struct Face {
        unsigned int _U;
        unsigned int _V;
        unsigned int _W;
    };

    struct Vector2;

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

        bool operator==(const Vector3Int& v) const {
            return x == v.x && y == v.y && z == v.z;
        }

        int x;
        int y;
        int z;
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

        __forceinline bool operator==(const Vector3& target) const {
            return x == target.x && y == target.y && z == target.z;
        }

        __forceinline Vector3 operator*(const Vector3& other) const {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        __forceinline Vector3& operator=(const float* v) {
            x = v[0];
            y = v[1];
            z = v[2];
            return *this;
        }


        __forceinline Vector3 operator-(const Vector3& b) {
            return Vector3{ x - b.x, y - b.y, z - b.z };
        }

        __forceinline Vector3 Cross(const Vector3& a, const Vector3& b) {
            return Vector3{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
        }

        __forceinline float Dot(const Vector3& b) {
            return x * b.x + y * b.y + z * b.z;
        }

        Vector3 Abs() {
            return Vector3{ std::abs(x), std::abs(y), std::abs(z) };
        }

        float operator[](const int index) const {
            XASSERT(index == 0 || index == 1 || index == 2, "wtf");
            switch (index)
            {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:  memset(nullptr, 0, 5); return 0;
            }

            return 0;
        }

        Vector2 ToV2() const;

        __forceinline Vector3Int ToV3Int() const {
            return Vector3Int(x, y, z);
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
}

#endif //__Vector3_h__
