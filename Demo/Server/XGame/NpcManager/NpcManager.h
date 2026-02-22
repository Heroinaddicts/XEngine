#ifndef __NpcManager_h__
#define __NpcManager_h__

#include "Header.h"

class NpcManager : public INpcManager {
public:
    virtual ~NpcManager() {}
private:
    // Í¨¹ý INpcManager ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;


private:
    static void OnSceneCreate(IScene* const);
    static void OnSceneRelease(IScene* const);

    static void OnPlayerCreated(IPlayer* const player, const UInt64 account);

    static void OnEnterScene(IScene* const, const UInt64 guid);
    static void OnLeaveScene(IScene* const, const UInt64 guid);
};

#endif //__NpcManager_h__
