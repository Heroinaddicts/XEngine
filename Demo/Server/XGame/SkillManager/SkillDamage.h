#ifndef __SkillDamage_h__
#define __SkillDamage_h__

#include "Header.h"

class SkillDamage : public ISkillDamage {
public:
    virtual ~SkillDamage() {}

private:
    bool AddDamage(IGameObject* const damageDealer, IGameObject* const damageReceiver, const float damageAmount, const AutoConfig::Enum::eDamageType type) override;

    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;
};

#endif //__SkillDamage_h__
