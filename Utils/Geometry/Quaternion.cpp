#include "Quaternion.h"
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
        q._W = cy * cp * cr + sy * sp * sr;
        q._X = cy * cp * sr - sy * sp * cr;
        q._Y = sy * cp * sr + cy * sp * cr;
        q._Z = sy * cp * cr - cy * sp * sr;

        return q;
    }

    Vector3 Quaternion::EulerAngles() const {
        Vector3 angles;
        // roll (x-axis rotation)
        double sinr_cosp = 2 * (this->_W * this->_X + this->_Y * this->_Z);
        double cosr_cosp = 1 - 2 * (this->_X * this->_X + this->_Y * this->_Y);
        angles.x = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        double sinp = 2 * (this->_W * this->_Y - this->_Z * this->_X);
        if (std::abs(sinp) >= 1)
            angles.y = std::copysign(MATH_PI / 2, sinp); // use 90 degrees if out of range
        else
            angles.y = std::asin(sinp);

        // yaw (z-axis rotation)
        double siny_cosp = 2 * (this->_W * this->_Z + this->_X * this->_Y);
        double cosy_cosp = 1 - 2 * (this->_Y * this->_Y + this->_Z * this->_Z);
        angles.z = std::atan2(siny_cosp, cosy_cosp);
        return angles;
    }
}
