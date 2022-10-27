#ifndef __Vector_h__
#define __Vector_h__

#include "Geometry.h"

namespace XEngine {
    struct Face {
        unsigned int _U;
        unsigned int _V;
        unsigned int _W;
    };

    struct Vector2 {
        Vector2(const float x_, const float y_) : _X(x_), _Y(y_) {}
        Vector2(const Vector2& v) : _X(v._X), _Y(v._Y) {}
        Vector2() : _X(0), _Y(0) {}

        Vector2& operator=(const float* v) {
            _X = v[0];
            _Y = v[1];
            return *this;
        }

        Vector2 operator-(const Vector2& v) const {
            return Vector2(_X - v._X, _Y - v._Y);
        }

        Vector2 operator+(const Vector2& v) const {
            return Vector2(_X + v._X, _Y + v._Y);
        }

        float _X;
        float _Y;
    };

    struct Vector3 {
        static const Vector3 zero;

        __forceinline Vector3(const float x_, const float y_, const float z_) : _X(x_), _Y(y_), _Z(z_) {}
        __forceinline Vector3(const Vector3& v) : _X(v._X), _Y(v._Y), _Z(v._Z) {}
        __forceinline Vector3() : _X(0), _Y(0), _Z(0) {}

        __forceinline float Length() const {
            return (float)sqrt(_X * _X + _Y * _Y + _Z * _Z);
        }

        __forceinline Vector3 Normalized(float epilon = Epsilon) const {
            float len = Length();
            if (len > epilon) {
                return (*this) / len;
            }
            return Vector3::zero;
        }

        __forceinline Vector3& operator=(const float* v) {
            _X = v[0];
            _Y = v[1];
            _Z = v[2];
            return *this;
        }

        __forceinline Vector3 operator-(const Vector3& v) const {
            return Vector3(_X - v._X, _Y - v._Y, _Z - v._Z);
        }

        __forceinline Vector3 operator+(const Vector3& v) const {
            return Vector3(_X + v._X, _Y + v._Y, _Z + v._Z);
        }

        __forceinline Vector3& operator *= (const float s) {
            _X *= s; _Y *= s; _Z *= s;
            return *this;
        }

        __forceinline Vector3 operator * (const float s) const {
            return Vector3(_X * s, _Y * s, _Z * s);
        }

        __forceinline Vector3 operator / (const float s) const {
            float invS = 1.0f / s;
            return Vector3(_X * invS, _Y * invS, _Z * invS);
        }

        __forceinline Vector3& operator /= (const float s) {
            float invS = 1.0f / s;
            _X *= invS; _Y *= invS; _Z *= invS;
            return *this;
        }

        float _X;
        float _Y;
        float _Z;
    };

    struct Vector2Int {
        Vector2Int(const int x_, const int y_, const int z_) : _X(x_), _Y(y_) {}
        Vector2Int(const Vector2Int& v) : _X(v._X), _Y(v._Y) {}
        Vector2Int() : _X(0), _Y(0) {}

        Vector2Int& operator=(const int* v) {
            _X = v[0];
            _Y = v[1];
            return *this;
        }

        int _X;
        int _Y;
    };

    struct Vector3Int {
        Vector3Int(const int x_, const int y_, const int z_) : _X(x_), _Y(y_), _Z(z_) {}
        Vector3Int(const Vector3Int& v) : _X(v._X), _Y(v._Y), _Z(v._Z) {}
        Vector3Int() : _X(0), _Y(0), _Z(0) {}

        Vector3Int& operator=(const int* v) {
            _X = v[0];
            _Y = v[1];
            _Z = v[2];
            return *this;
        }

        int _X;
        int _Y;
        int _Z;
    };
}

#endif //__Vector_h__
