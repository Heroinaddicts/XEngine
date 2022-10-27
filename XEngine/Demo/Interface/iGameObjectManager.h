#ifndef __iGameObjectManager_h__
#define __iGameObjectManager_h__

#include "iComponent.h"
#include "GameObjectTemplate.h"

namespace XEngine {

    class iGameObject {
    public:
        virtual ~iGameObject() {}


    };

    class iGameObjectManager : public Api::iComponent {
    public:
        virtual ~iGameObjectManager() {}
    };
}

#endif //__iGameObjectManager_h__
