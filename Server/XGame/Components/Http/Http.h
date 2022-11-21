#ifndef __Http_h__
#define __Http_h__ 

#include "Header.h"

class Http : public iHttp {
public:
    virtual ~Http() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__Http_h__
