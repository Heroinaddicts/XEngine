#ifndef __Gate_h__
#define __Gate_h__ 

#include "Header.h"

class Gate : public iGate {
public:
    virtual ~Gate() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);

private:
    static void OnNodeSessionAppear(iNodeSession* const session);
    static void OnNodeSessionDisappear(iNodeSession* const session);

    static iNodeSession* s_LobbyNodeSession;
};

#endif //__Gate_h__
