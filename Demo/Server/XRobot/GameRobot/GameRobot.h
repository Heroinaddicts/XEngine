#ifndef __GameRobot_h__
#define __GameRobot_h__

#include "Header.h"

class GameRobot : public IGameRobot, public Api::iTimer {
public:
    virtual ~GameRobot() {}

    // 通过 IGameRobot 继承
    IRobotSession* QuerySession(const UInt64 account) const override;
    void RegistEvent(const eEvent ev, const fEvent fun, const char* file, const int line) override;
    void RegistServerMessage(const UInt16 messageId, const fServerMessage call) override;
private:
    // 通过 IGameRobot 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;
    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

};

#endif //__GameRobot_h__
