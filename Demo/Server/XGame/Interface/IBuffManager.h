#ifndef __IBuffManager_h__
#define __IBuffManager_h__

#include "iModule.h"
UsingXEngine;
#include <map>

class IGameObject;

class IBuffManager : public Api::iModule {
public:
    virtual ~IBuffManager() {}

    virtual UInt64 AddBuff(IGameObject* const caster, IGameObject* const entity, const Int64 buffConfigID) = 0;
};

#endif //__IBuffManager_h__
