#ifndef __ISkillLauncher_h__
#define __ISkillLauncher_h__

#include "iModule.h"
UsingXEngine;

class IGameObject;

class ISkillLauncher : public Api::iModule {
public:
	virtual ~ISkillLauncher() {};

	virtual UInt64 AddLauncher(IGameObject* const cast, const Int64 launcherID, const float angle, const Vector2& targetPos, const Vector2& placePos, const UInt64 targetGuid) = 0;
};

#endif //__ISkillLauncher_h__

