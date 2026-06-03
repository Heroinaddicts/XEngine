#ifndef __RobotSceneManager_h__
#define __RobotSceneManager_h__

#include "Header.h"

class RobotSceneManager : public IRobotSceneManager {
public:
    virtual ~RobotSceneManager() {}
    // 通过 IRobotSceneManager 继承
    IRobotScene* QuerySceneByEntityGuid(const UInt64 sceneId) const override;
    IRobotScene* QueryCharacterScene(const UInt64 characterId) const override;
private:
    // 通过 IRobotSceneManager 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    static void OnSceneInfoPush(IRobotSession* const session, const UInt64 sessionId, const oSceneInfoPush& push);

};

#endif //__RobotSceneManager_h__
