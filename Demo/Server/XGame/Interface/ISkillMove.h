#ifndef __ISkillMove_h__
#define __ISkillMove_h__

#include "iModule.h"
UsingXEngine;

class IGameObject;

class ISkillMove : public Api::iModule {
public:
    virtual ~ISkillMove() {}

    virtual UInt64 AddMove(IGameObject* const cast, const UInt64 skillGuid, const Int64 skillConfig, const Int64 moveID, const float angle, const UInt64 targetGuid = INVALID_GAMEOBJECT_ID) = 0;
};

#endif //__ISkillMove_h__
