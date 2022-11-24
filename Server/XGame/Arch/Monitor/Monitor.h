#ifndef __Monitor_h__
#define __Monitor_h__ 

#include "Header.h"

class Monitor : public iMonitor {
public:
    virtual ~Monitor() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__Monitor_h__
