#ifndef __IServerManager_h__
#define __IServerManager_h__

#include "iModule.h"
UsingXEngine;

class IServerManager : public Api::iModule {
public:
    virtual ~IServerManager() {}

    virtual UInt16 ServerID() const = 0;
    virtual UInt64 GeneraterGUID() const = 0;
};

#endif //__IServerManager_h__
