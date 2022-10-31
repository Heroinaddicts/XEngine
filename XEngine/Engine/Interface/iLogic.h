#ifndef __iLogic_h__
#define __iLogic_h__

#include "iEngineComponent.h"

namespace XEngine {
    class iLogic : public iEngineComponent {
    public:
        virtual ~iLogic() {}

        virtual Api::iModule* FindModule(const std::string& name) = 0;
    };
}

#endif //__iLogic_h__
