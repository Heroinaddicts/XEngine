#ifndef __RefreshRules_h__
#define __RefreshRules_h__

#include "Header.h"

class RefreshRules : public IRefreshRules, public Api::iTimer {
public:
    virtual ~RefreshRules() {}
private:
    // Í¨¹ý IRefreshRules ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

private:
    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

private:
    static void OnSceneCreate(IScene* const scene);
    static void OnSceneRelease(IScene* const scene);
};

#endif //__RefreshRules_h__
