#ifndef __RobotNpc_h__
#define __RobotNpc_h__

#include "Header.h"

class RobotNpc : public IRobotNpc {
public:
    virtual ~RobotNpc() {}

    __forceinline const Vector2& Position() const override {
        return _Position;
    }

    RobotNpc(const UInt64 configId, const UInt64 sceneId, const Vector2& pos) : IRobotNpc(configId, sceneId), _Position(pos) {}
private:
    Vector2 _Position;
};

#endif //__RobotNpc_h__
