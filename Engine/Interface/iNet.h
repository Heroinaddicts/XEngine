#ifndef __iNet_h__
#define __iNet_h__

#include "iEngineComponent.h"
#include "iNetApi.h"

namespace XEngine {
    class iNet : public iEngineComponent, public Api::iNetApi {
    public:
        virtual ~iNet() {}
    };
}

#endif //__iLogic_h__
