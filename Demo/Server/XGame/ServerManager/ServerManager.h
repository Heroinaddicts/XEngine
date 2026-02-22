#ifndef __ServerManager_h__
#define __ServerManager_h__

#include "Header.h"

class ServerManager : public IServerManager {
public:
    virtual ~ServerManager() {}

    // 通过 IServerManager 继承
    UInt16 ServerID() const override;
    UInt64 GeneraterGUID() const override;
private:
    // 通过 IServerManager 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

};

#endif //__ServerManager_h__
