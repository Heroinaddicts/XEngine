#ifndef __TestRedis_h__
#define __TestRedis_h__ 

#include "Header.h"

class TestRedis : public iTestRedis, public Api::iTimer {
public:
    virtual ~TestRedis() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);

    // Í¨¹ý iTimer ¼Ì³Ð
    virtual void OnStart(const int id, void* const context, const int64 timestamp) override;
    virtual void OnTimer(const int id, void* const context, const int64 timestamp) override;
    virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) override;
    virtual void OnPause(const int id, void* const context, const int64 timestamp) override;
    virtual void OnResume(const int id, void* const context, const int64 timestamp) override;
};

#endif //__TestRedis_h__
