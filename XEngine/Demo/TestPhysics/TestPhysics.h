#ifndef __Test001_h__
#define __Test001_h__

#include "Header.h"

class TestPhysics : public iComponent, public iTimer {
public:
    virtual ~TestPhysics() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);

    // 通过 iTimer 继承
    virtual void OnStart(const int id, void* const context, const int64 timestamp) override;
    virtual void OnTimer(const int id, void* const context, const int64 timestamp) override;
    virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) override;
    virtual void OnPause(const int id, void* const context, const int64 timestamp) override;
    virtual void OnResume(const int id, void* const context, const int64 timestamp) override;
};

#endif //__Test001_h__
