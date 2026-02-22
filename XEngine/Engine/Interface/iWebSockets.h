#ifndef __iWebSockets_h__
#define __iWebSockets_h__

#include "iEngineComponent.h"
#include "iWebSocketsApi.h"

namespace XEngine {
    class iWebSockets : public iEngineComponent, public Api::iWebSocketsApi {
    public:
        virtual ~iWebSockets() {}
    };
}

#endif //__iLogic_h__
