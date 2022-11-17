#ifndef __GameObjectTest_h__
#define __GameObjectTest_h__ 

#include "Header.h"

class GameObjectTest : public iGameObjectTest {
public:
    virtual ~GameObjectTest() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__GameObjectTest_h__
