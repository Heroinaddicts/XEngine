#ifndef __iModule_h__
#define __iModule_h__

#include "MultiSys.h"
#include "iEngine.h"
#include <string>

namespace XEngine {
    namespace Api {
        class iModule {
        public:
            virtual ~iModule() {}
            iModule() : _Name(""), _Next(nullptr) {}

            virtual bool Initialize(iEngine* const engine) = 0;
            virtual bool Launch(iEngine* const engine) = 0;
            virtual bool Destroy(iEngine* const engine) = 0;

            virtual void Update(iEngine* const engine) {};
            virtual void FixedUpdate(iEngine* const engine) {};

            const char* const _Name;
            iModule* const _Next;
        };
    }
}

#define GET_MODULES_FUNC_NAME "GetModules"

#ifdef WIN32
#define DLL_INSTANCE \
    static XEngine::Api::iModule * static_modules = nullptr; \
    extern "C" __declspec(dllexport) XEngine::Api::iModule * __cdecl GetModules() {\
        return static_modules; \
    }
#else
#define DLL_INSTANCE \
    static XEngine::Api::iModule * static_modules = nullptr; \
    extern "C" XEngine::Api::iModule * GetModules() { \
        return static_modules; \
    }
#endif //WIN32

#define CREATE_MODULE(Module) \
class Factory##Module {\
public:\
    Factory##Module(XEngine::Api::iModule * & pModule) {\
        XEngine::Api::iModule * p = xnew Module();\
        const char * temp = #Module; \
        memcpy((void *)&(p->_Name), &temp, sizeof(const char *));\
        memcpy((void *)&(p->_Next), &pModule, sizeof(pModule));\
        pModule = p;\
    }\
};\
Factory##Module factroy##Module(static_modules);

#endif //__iModule_h__
