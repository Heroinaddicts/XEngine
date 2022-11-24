#ifndef __TestProfile_h__
#define __TestProfile_h__ 

#include "Header.h"

class TestProfile : public iTestProfile {
public:
    virtual ~TestProfile() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__TestProfile_h__
