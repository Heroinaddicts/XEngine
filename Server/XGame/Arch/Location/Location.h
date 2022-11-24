#ifndef __Location_h__
#define __Location_h__ 

#include "Header.h"

class Location : public iLocation {
public:
    virtual ~Location() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__Location_h__
