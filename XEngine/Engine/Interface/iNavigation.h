#ifndef __iNavigation_h__
#define __iNavigation_h__

#include "iNavigationApi.h"
#include "iEngineComponent.h"

namespace XEngine {
    class iNavigation : public iEngineComponent, public Api::iNavigationApi {
    public:
        virtual ~iNavigation() {}
    };
}

#endif //__iNavigation_h__
