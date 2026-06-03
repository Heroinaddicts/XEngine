#ifndef __RobotNpcManager_h__
#define __RobotNpcManager_h__

#include "Header.h"

class RobotNpcManager : public IRobotNpcManager {
public:
    virtual ~RobotNpcManager() {}

    const std::unordered_map<Int64, IRobotNpc* const>& GetNpc(const UInt64 sceneId) const override;
private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    static void OnNpcInfoPush(IRobotSession* session, const UInt64 sessionId, const oNpcInfoPush& psh);
};

#endif //__RobotNpcManager_h__
