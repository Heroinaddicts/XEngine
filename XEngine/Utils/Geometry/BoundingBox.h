#ifndef __BoundingBox_h__
#define __BoundingBox_h__

#include "Vector3.h"

namespace XEngine {
    class BoundingBox {
    public:
        virtual ~BoundingBox() {}
        BoundingBox() {}
        BoundingBox(const Vector3& min_, const Vector3& max_) : min(min_), max(max_) {}

        __forceinline void Expand(const Vector3& point) {
            min.x = xmin(min.x, point.x);
            min.y = xmin(min.y, point.y);
            min.z = xmin(min.z, point.z);

            max.x = xmax(max.x, point.x);
            max.y = xmax(max.y, point.y);
            max.z = xmax(max.z, point.z);
        }

        __forceinline void Expand(const BoundingBox& other) {
            min.x = xmin(min.x, other.min.x);
            min.y = xmin(min.y, other.min.y);
            min.z = xmin(min.z, other.min.z);

            max.x = xmax(max.x, other.max.x);
            max.y = xmax(max.y, other.max.y);
            max.z = xmax(max.z, other.max.z);
        }

        Vector3 min;
        Vector3 max;
    };
}


#endif //__BoundingBox_h__
