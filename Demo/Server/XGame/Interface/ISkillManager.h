#ifndef __ISkillManager_h__
#define __ISkillManager_h__

#include "iModule.h"
#include "Vector2.h"
UsingXEngine;

class IGameObject;

class ISkillManager : public Api::iModule {
public:
    virtual ~ISkillManager() {}

    virtual void SkillCast(IGameObject* const cast,
        const UInt64 skillGuid,
        const Int64 skillConfigId,
        const float angle = 0.0f,
        const Vector2& targetPos = Vector2(0.0f, 0.0f),
        const Vector2& placePos = Vector2(0.0f, 0.0f),
        const UInt64 targetGuid = 0) = 0;
};

#endif //__ISkillManager_h__
