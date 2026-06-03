#ifndef __RobotSkill_h__
#define __RobotSkill_h__

#include "Header.h"

class RobotSkill : public IRobotSkill, public Api::iTimer {
public:
    virtual ~RobotSkill() {}
private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

    static void OnRobotCreated(IRobotSession* const session);
    static void OnRobotRelease(IRobotSession* const session);
};

#endif //__RobotSkill_h__
