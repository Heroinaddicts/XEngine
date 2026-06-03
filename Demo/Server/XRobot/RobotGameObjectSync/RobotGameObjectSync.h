#ifndef __RobotGameObjectSync_h__
#define __RobotGameObjectSync_h__

#include "Header.h"

class RobotGameObjectSync : public IRobotGameObjectSync {
public:
    virtual ~RobotGameObjectSync() {}
private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

private:
    static void OnGameObjectAppearPush(IRobotSession* const, const UInt64 sessionId, const oGameObjectAppearPush& push);
    static void OnGameObjectDisappearPush(IRobotSession* const, const UInt64 sessionId, const oGameObjectDisappearPush& push);
    static void OnCharacterInfoPush(IRobotSession* const, const UInt64 sessionId, const oCharacterInfoPush& push);
};

#endif //__RobotGameObjectSync_h__
