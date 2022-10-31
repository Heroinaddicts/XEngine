#ifndef __GameObjectManager_h__
#define __GameObjectManager_h__ 

#include "Header.h"

class GameObjectManager : public iModule {
public:
    virtual ~GameObjectManager() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__GameObjectManager_h__
