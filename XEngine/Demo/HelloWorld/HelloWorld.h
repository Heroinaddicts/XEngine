#ifndef __HelloWorld_h__
#define __HelloWorld_h__ 

#include "Header.h"

class HelloWorld : public iComponent {
public:
    virtual ~HelloWorld() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__HelloWorld_h__
