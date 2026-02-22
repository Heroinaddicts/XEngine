#ifndef __iTaskManager_h__
#define __iTaskManager_h__

#include "iEngineComponent.h"
#include "iTaskApi.h"

namespace XEngine {
    class iTaskManager : public iEngineComponent, public Api::iTaskApi {
    public:
        virtual ~iTaskManager() {}
    };
}

#endif //__iTaskManager_h__
