#ifndef __iHttp_h__
#define __iHttp_h__

#include "iEngineComponent.h"
#include "iHttpApi.h"

namespace XEngine {
    class iHttp : public iEngineComponent, public Api::iHttpApi {
    public:
        virtual ~iHttp() {}

    };
}

#endif //__iHttp_h__
