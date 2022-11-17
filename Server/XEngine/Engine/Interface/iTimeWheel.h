#ifndef __iTimeWheel_h__
#define __iTimeWheel_h__

#include "iTimerApi.h"
#include "iEngineComponent.h"

namespace XEngine {
    class iTimeWheel : public iEngineComponent, public Api::iTimerApi {
    public:
        virtual ~iTimeWheel() {}
    };
}

#endif //__iTimeWheel_h__
