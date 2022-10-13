#ifndef __iPhysics_h__
#define __iPhysics_h__

#include "iEngineComponent.h"
#include "iPhysicsApi.h"

namespace XEngine {
    class iPhysics : public iEngineComponent, public Api::iPhysicsApi {
    public:
        virtual ~iPhysics() {}
    };
}

#endif //__iPhysics_h__
