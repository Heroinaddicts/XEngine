#include "Quaternion.h"
#include "Vector.h"
#include <cmath>

#define MATH_PI 3.14159265358979323846   // pi

namespace XEngine {
    Quaternion Quaternion::Euler(const Vector3& v) {
        // Abbreviations for the various angular functions
        double cy = cos(v.z * 0.5f);
        double sy = sin(v.z * 0.5f);
        double cp = cos(v.y * 0.5f);
        double sp = sin(v.y * 0.5f);
        double cr = cos(v.x * 0.5f);
        double sr = sin(v.x * 0.5f);

        Quaternion q;
        q.w = cy * cp * cr + sy * sp * sr;
        q.x = cy * cp * sr - sy * sp * cr;
        q.y = sy * cp * sr + cy * sp * cr;
        q.z = sy * cp * cr - cy * sp * sr;

        return q;
    }

    Vector3 Quaternion::EulerAngles() const {
        Vector3 angles;
        // roll (x-axis rotation)
        double sinr_cosp = 2 * (this->w * this->x + this->y * this->z);
        double cosr_cosp = 1 - 2 * (this->x * this->x + this->y * this->y);
        angles.x = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        double sinp = 2 * (this->w * this->y - this->z * this->x);
        if (std::abs(sinp) >= 1)
            angles.y = std::copysign(MATH_PI / 2, sinp); // use 90 degrees if out of range
        else
            angles.y = std::asin(sinp);

        // yaw (z-axis rotation)
        double siny_cosp = 2 * (this->w * this->z + this->x * this->y);
        double cosy_cosp = 1 - 2 * (this->y * this->y + this->z * this->z);
        angles.z = std::atan2(siny_cosp, cosy_cosp);
        return angles;
    }
}
