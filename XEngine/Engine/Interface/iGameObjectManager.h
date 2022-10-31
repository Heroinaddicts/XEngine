#ifndef __iGameObjectManager_h__
#define __iGameObjectManager_h__

#include "iGameObjectApi.h"
#include "iEngineComponent.h"

namespace XEngine {
    class iGameObjectManager : public iEngineComponent, public Api::iGameObjectApi {
    public:
        virtual ~iGameObjectManager() {}
    };
}

#endif //__iGameObjectManager_h__s
