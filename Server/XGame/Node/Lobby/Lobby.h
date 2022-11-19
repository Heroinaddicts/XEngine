#ifndef __Lobby_h__
#define __Lobby_h__ 

#include "Header.h"

class Lobby : public iLobby {
public:
    virtual ~Lobby() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
};

#endif //__Lobby_h__
