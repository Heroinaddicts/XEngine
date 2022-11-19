#ifndef __Balance_h__
#define __Balance_h__ 

#include "Header.h"

class Balance : public iBalance {
public:
    virtual ~Balance() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__Balance_h__
