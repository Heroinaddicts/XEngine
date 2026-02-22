#include "Vector2.h"
#include "Vector3.h"

namespace XEngine {
    Vector3 Vector2::ToV3() {
        return Vector3(this->x, 0, this->y);
    }
}
