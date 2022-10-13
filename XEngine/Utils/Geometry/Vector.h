#ifndef __Vector_h__
#define __Vector_h__

namespace XEngine {
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
        Vector3(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {}
        Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
        Vector3() : x(0), y(0), z(0) {}


        Vector3& operator=(const float* v) {
            x = v[0];
            y = v[1];
            z = v[2];
            return *this;
        }

        Vector3 operator-(const Vector3& v) const {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }

        Vector3 operator+(const Vector3& v) const {
            return Vector3(x + v.x, y + v.y, z + v.z);
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
