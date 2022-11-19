#ifndef __GameServer_h__
#define __GameServer_h__ 

#include "Header.h"

class GameServer : public iGameServer {
public:
    virtual ~GameServer() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__GameServer_h__
