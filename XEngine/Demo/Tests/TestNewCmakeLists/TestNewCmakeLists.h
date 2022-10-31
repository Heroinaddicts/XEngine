#ifndef __TestNewCmakeLists_h__
#define __TestNewCmakeLists_h__ 

#include "Header.h"

class TestNewCmakeLists : public iModule {
public:
    virtual ~TestNewCmakeLists() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__TestNewCmakeLists_h__
