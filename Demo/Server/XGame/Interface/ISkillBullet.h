#ifndef __ISkillBullet_h__
#define __ISkillBullet_h__

#include "iModule.h"
UsingXEngine;

class ISkillBullet : public Api::iModule {
public:
    virtual ~ISkillBullet() {}

    virtual UInt64 AddBullet(IGameObject* const launcher, const Int64 bulletID) = 0;
};

#endif //__ISkillBullet_h__
