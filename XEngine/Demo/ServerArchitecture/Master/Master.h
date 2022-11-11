#ifndef __Master_h__
#define __Master_h__ 

#include "Header.h"

class Master : public iMaster {
public:
    virtual ~Master() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__Master_h__
