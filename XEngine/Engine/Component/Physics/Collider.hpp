#ifndef __Collider_hpp__
#define __Collider_hpp__

#include "Header.h"

namespace XEngine {
    class Collider : public Api::iCollider {
    public:
        virtual ~Collider() {}

        virtual Api::iPhysxContext* const GetPhysxContext() const {
            return _Context;
        }

        Collider(Api::iPhysxContext* context) : _Context(context) {}

        Api::iPhysxContext* const _Context;
    };
}

#endif //__Collider_hpp__
