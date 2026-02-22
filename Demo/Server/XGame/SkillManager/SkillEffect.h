#ifndef __SkillEffect_h__
#define __SkillEffect_h__

#include "Header.h"

class SkillEffect : public ISkillEffect {
public:
    virtual ~SkillEffect() {}

private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;
};

#endif //__SkillEffect_h__
