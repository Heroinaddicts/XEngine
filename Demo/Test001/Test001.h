#ifndef __Test001_h__
#define __Test001_h__

#include "Header.h"

class Test001 : public iComponent {
public:
    virtual ~Test001() {}
    DEFINE_COMPONENT_CONSTRUCTOR(Test001);
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__Test001_h__
