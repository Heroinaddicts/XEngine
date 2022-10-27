#ifndef __iComponent_h__
#define __iComponent_h__

#include "MultiSys.h"
#include "iEngine.h"
#include <string>

namespace XEngine {
    namespace Api {
        class iComponent {
        public:
            virtual ~iComponent() {}
            iComponent() : _Name(""), _Next(nullptr) {}

            virtual bool Initialize(iEngine* const engine) = 0;
            virtual bool Launch(iEngine* const engine) = 0;
            virtual bool Destroy(iEngine* const engine) = 0;

            const char* const _Name;
            iComponent* const _Next;
        };
    }
}


#ifdef WIN32
#define DLL_INSTANCE \
    static XEngine::Api::iComponent * static_components = nullptr; \
    extern "C" __declspec(dllexport) XEngine::Api::iComponent * __cdecl GetComponents() {\
        return static_components; \
    }
#else
#define DLL_INSTANCE \
    static XEngine::Api::iComponent * static_components = nullptr; \
    extern "C" XEngine::Api::iComponent * GetComponents() { \
        return static_components; \
    }
#endif //WIN32

#define CREATE_COMPONENT(Component) \
class Factory##Component {\
public:\
    Factory##Component(XEngine::Api::iComponent * & pComponent) {\
        XEngine::Api::iComponent * p = xnew Component();\
        const char * temp = #Component; \
        memcpy((void *)&(p->_Name), &temp, sizeof(const char *));\
        memcpy((void *)&(p->_Next), &pComponent, sizeof(pComponent));\
        pComponent = p;\
    }\
};\
Factory##Component factroy##Component(static_components);

#endif //__iComponent_h__
