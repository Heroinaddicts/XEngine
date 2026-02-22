#ifndef __SkillEvent_h__
#define __SkillEvent_h__

#include "Header.h"

class SkillEvent : public ISkillEvent {
public:
    virtual ~SkillEvent() {}

private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;
};

#endif //__SkillEvent_h__
