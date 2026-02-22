#ifndef __Movement_h__
#define __Movement_h__

#include "Header.h"

class Movement : public IMovement, public Api::iTimer {
public:
    virtual ~Movement() {}
private:
    // 通过 IMovement 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    void MoveTo(IGameObject* const entity, const Vector2& position, const fArrived& callback) override;
    void StopMoveTo(IGameObject* const entity) override;

private:
    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;
    void OnEnd(const int id, const UInt64 context, bool nonviolent, const Int64 timestamp) override;

private:
    static void OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState state);

    static bool OnEnterMovingJugde(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& pos, const float angle);
    static void OnEnterMoving(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& pos, const float angle);
    static void OnExitMoving(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& pos, const float angle);
    static void GetBestSlideEdges(const Vector2& delta, int& first, int& second);

    static bool CanSlide(IGameObject* const unit, const ITileLayer* const tilemap, const Vector2& delta, Vector2& pos);

    static bool CalcPosition(const UInt64 guid, const UInt64 timestamp);
    static void UpdatePosition(const UInt64 guid, const UInt64 timestamp);
};

#endif //__Movement_h__
