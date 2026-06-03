#ifndef __RobotMovement_h__
#define __RobotMovement_h__

#include "Header.h"

class RobotMovement : public IRobotMovement, public Api::iTimer {
public:
    virtual ~RobotMovement() {}
private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;
    // Í¨¹ý IRobotMovement ¼Ì³Ð
    void MoveTo(const UInt64 characterId, const Vector2& pos, const fArrive& arrive) override;

    static void OnRobotCreated(IRobotSession* const session);
    static void OnRobotRelease(IRobotSession* const session);
};

#endif //__RobotMovement_h__
