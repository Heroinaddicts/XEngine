#ifndef __RemoteServer_h__
#define __RemoteServer_h__

#include "Header.h"

class RemoteServer : public iModule {
public:
    virtual ~RemoteServer() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__RemoteServer_h__
