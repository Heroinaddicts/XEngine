#ifndef __iXZip_h__
#define __iXZip_h__

#include "iEngineComponent.h"
#include "iZipApi.h"

namespace XEngine {
    class iXZip : public iEngineComponent, public Api::iZipApi {
    public:
        virtual ~iXZip() {}
    };
}

#endif //__iLogic_h__
