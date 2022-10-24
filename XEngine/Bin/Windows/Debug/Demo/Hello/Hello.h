#ifndef __Hello_h__
#define __Hello_h__ 

#include "Header.h"

class Hello : public iComponent {
public:
    virtual ~Hello() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__Hello_h__
