#ifndef __iMemory_h__
#define __iMemory_h__


#include "iEngineComponent.h"

namespace XEngine {
    class iMemory : public iEngineComponent, public Api::iMemoryApi {
    public:
        virtual ~iMemory() {}
    };
}

#endif //__iMemory_h__
