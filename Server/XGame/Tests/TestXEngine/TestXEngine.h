#ifndef __TestXEngine_h__
#define __TestXEngine_h__ 

#include "Header.h"

class TestXEngine : public iTestXEngine {
public:
    virtual ~TestXEngine() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__TestXEngine_h__
