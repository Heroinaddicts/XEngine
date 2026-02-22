#ifndef __ISkillDamage_h__
#define __ISkillDamage_h__

#include "iModule.h"
UsingXEngine;

class IGameObject;

class ISkillDamage : public Api::iModule {
public:
    virtual ~ISkillDamage() {}

    virtual bool AddDamage(IGameObject* const damageDealer, IGameObject* const damageReceiver, const float damageAmount, const AutoConfig::Enum::eDamageType type) = 0;
};

#endif //__ISkillDamage_h__
