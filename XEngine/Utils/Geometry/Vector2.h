#ifndef __Vector2_h__
#define __Vector2_h__

#include "Geometry.h"

namespace XEngine {
    struct Vector3;

    struct Vector2 {
        __forceinline Vector2(const float x_, const float y_) : x(x_), y(y_) {}
        __forceinline Vector2(const Vector2& v) : x(v.x), y(v.y) {}
        __forceinline Vector2() : x(0), y(0) {}
        __forceinline Vector2(const int def) : x((float)def), y((float)def) {}
        __forceinline Vector2(const float def) : x(def), y(def) {}

        __forceinline static Vector2 CalcPosition(const Vector2& origin, const float angle, const float distance) {
            float angleInRadians = angle * (3.141592653589793f / 180.0f);
            float dx = std::cos(angleInRadians) * distance;
            float dy = std::sin(angleInRadians) * distance;
            return Vector2(origin.x + dx, origin.y + dy);
        }

        Vector2& operator=(const float* v) {
            x = v[0];
            y = v[1];
            return *this;
        }

        Vector2& operator=(const Vector2& target) {
            x = target.x;
            y = target.y;
            return *this;
        }

        Vector2& operator+=(const Vector2& target) {
            this->x += target.x;
            this->y += target.y;
            return *this;
        }

        Vector2 operator-(const Vector2& v) const {
            return Vector2(x - v.x, y - v.y);
        }

        Vector2 operator+(const Vector2& v) const {
            return Vector2(x + v.x, y + v.y);
        }

        Vector2 operator*(const float d) const {
            return Vector2(x * d, y * d);
        }

        Vector3 ToV3();

        __forceinline float Length() const {
            return (float)sqrt(x * x + y * y);
        }

        __forceinline float LengthSquared() const {
            return x * x + y * y;
        }

        __forceinline float Dot(const Vector2& v) const {
            return x * v.x + y * v.y;
        }

        __forceinline float Angle() const {
            float angle = (float)atan2(y, x) * 180.0f / XPI;
            if (angle < 0) angle += 360.0f;
            return angle;
        }

        __forceinline Vector2 Normalized() const {
            float lenSq = x * x + y * y;
            if (lenSq < 1e-8f)
                return { 0, 0 };
            float invLen = 1.0f / std::sqrt(lenSq);
            return { x * invLen, y * invLen };
        }

        float x;
        float y;
    };

    struct Vector2Int {
        Vector2Int(const int x_, const int y_) : x(x_), y(y_) {}
        Vector2Int(const Vector2Int& v) : x(v.x), y(v.y) {}
        Vector2Int() : x(0), y(0) {}

        Vector2Int operator-(const Vector2Int& v) const {
            return Vector2Int(x - v.x, y - v.y);
        }

        Vector2Int operator+(const Vector2Int& v) const {
            return Vector2Int(x + v.x, y + v.y);
        }

        __forceinline float Length() const {
            return (float)sqrt(x * x + y * y);
        }

        bool operator==(const Vector2Int& v) const {
            return x == v.x && y == v.y;
        }

        bool operator<(const Vector2Int& target) const {
            return (x < target.x) || (x == target.x && y < target.y);
        }

        std::size_t operator()() const noexcept {
            return ((UInt64)x) << 32 | y;
        }

        bool operator()(const Vector2Int& b) const noexcept {
            return x == b.x && y == b.y;
        }
        int x;
        int y;
    };
}

namespace std {
    template<>
    struct hash<XEngine::Vector2Int> {
        size_t operator()(const XEngine::Vector2Int& v) const noexcept {
            uint32_t ux = static_cast<uint32_t>(v.x);
            uint32_t uy = static_cast<uint32_t>(v.y);
            return (static_cast<uint64_t>(ux) << 32) | uy;
        }
    };
}

#endif //__Vector2_h__
