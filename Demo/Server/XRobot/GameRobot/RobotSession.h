#ifndef __RobotSession_h__
#define __RobotSession_h__

#include "Header.h"

class RobotSession : public IRobotSession, public Api::iTimer {
public:
    virtual ~RobotSession();

    static RobotSession* Create();
    static RobotSession* Query(const UInt64 sessionId);
    static RobotSession* QueryByAccount(const UInt64 account);
    static void Release(RobotSession* RobotSession);

    const UInt64 _SessionID;

    __forceinline UInt64 SessionID() const override {
        return _SessionID;
    }

    __forceinline UInt64 Account() const override {
        return _Account;
    }
    __forceinline UInt64 DeployCharacterID() const override {
        return _DeployCharacterID;
    }

    __forceinline const Vector2& Position() const override {
        return _Position;
    }
    __forceinline void SetPosition(const Vector2& pos) override {
        _Position = pos;
    }

    __forceinline int Angle() const override { return _Angle; }
    __forceinline void SetAngle(const int angle) override { _Angle = angle; }

private:
    RobotSession();
    friend XPool<RobotSession>;

    void OnConnected() override;
    void OnConnectFailed() override;
    void OnDisconnected() override;
    int OnReceive(const void* const content, const int size) override;


    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

private:
    UInt64 _LastTick;
    UInt64 _LastRecvTick;

private:
    UInt64 _Account;
    UInt64 _DeployCharacterID;
    Vector2 _Position;

    int _Angle;
};

#endif //__RobotSession_h__
