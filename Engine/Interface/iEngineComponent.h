#ifndef __iEngineComponent_h__
#define __iEngineComponent_h__

#include "iEngine.h"
#include "MultiSys.h"
#include <string>

#define SAFE_SET_CONST_VALUE(pConst, p) SafeMemory::Memcpy((void*)&(pConst), sizeof(pConst), &p, sizeof(p));

namespace XEngine {
    class Api::iEngine;
    class iEngineComponent {
    public:
        virtual ~iEngineComponent() {}

        virtual bool Initialize(Api::iEngine* const engine) = 0;
        virtual bool Launch(Api::iEngine* const engine) = 0;
        virtual void Release(Api::iEngine* const engine) = 0;

        virtual void EarlyUpdate(Api::iEngine* const engine) = 0;
        virtual void Update(Api::iEngine* const engine) = 0;
        virtual void LaterUpdate(Api::iEngine* const engine) = 0;
    };
}

#define XERROR(engine, format, ...) {\
    char log[1024] = {0};\
    XASSERT(false, "XEngine Error : %s:%d >>>> "#format, __FILE__, __LINE__, ##__VA_ARGS__);\
    sprintf_s(log, sizeof(log), format, ##__VA_ARGS__);\
    engine->LogSync("XEngine Error", log, __FILE__, __LINE__);\
}

#define XLOG(engine, format, ...) {\
    char log[1024] = {0};\
    sprintf_s(log, sizeof(log), #format, ##__VA_ARGS__);\
    engine->LogAsync("XEngine Log", log, __FILE__, __LINE__);\
}

#endif //__iEngineComponent_h__
