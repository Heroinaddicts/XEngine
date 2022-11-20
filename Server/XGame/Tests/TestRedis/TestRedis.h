#ifndef __TestRedis_h__
#define __TestRedis_h__ 

#include "Header.h"

class TestRedis : public iTestRedis {
public:
    virtual ~TestRedis() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__TestRedis_h__
