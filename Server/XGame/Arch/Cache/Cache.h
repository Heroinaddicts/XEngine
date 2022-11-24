#ifndef __Cache_h__
#define __Cache_h__ 

#include "Header.h"

class Cache : public iCache {
public:
    virtual ~Cache() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__Cache_h__
