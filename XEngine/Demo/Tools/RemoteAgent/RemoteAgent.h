#ifndef __RemoteAgent_h__
#define __RemoteAgent_h__

#include "Header.h"

class RemoteAgent : public iComponent {
public:
    virtual ~RemoteAgent() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__RemoteAgent_h__
