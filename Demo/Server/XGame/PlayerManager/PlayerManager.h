#ifndef __PlayerManager_h__
#define __PlayerManager_h__

#include "Header.h"

class Player;
class PlayerManager : public IPlayerManager, public Api::iTimer {
public:
    virtual ~PlayerManager() {}

    // 通过 IPlayerManager 继承
    void RegistPlayerEvent(const ePlayerEvent ev, const fPlayerEvent fun) override;
    IPlayer* QueryPlayer(const UInt64 account) const override;
private:
    // 通过 IPlayerManager 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

    static void OnAccountConnect(const UInt64 account);
    static void OnAccountDisconnect(const UInt64 account);
    static void OnCreatePlayerReq(IPlayer* const nothing, const UInt64 account, const oCreatePlayerReq& createPlayer);

    static void OnDelayTestReq(IPlayer* const nothing, const UInt64 account, const oDelayTestReq& req);

    static void OnCharacterDockerCreated(ICharacterDocker* docker, IGameObject* character);
    static void OnCharacterDockerLoaded(ICharacterDocker* docker, IGameObject* character);
    static void OnDefaultCharacterCreated(ICharacterDocker* const docker, IGameObject* const character);
    static void OnDeployCharacter(ICharacterDocker* const docker, IGameObject* const character);
private:
    static XEventPool<IPlayerManager::ePlayerEvent, IPlayer*, const UInt64> s_EventPool;

private:
    void RegistPlayerMessage(const UInt16 messageId, const IPlayerManager::fPlayerMessageCall fun) override;
    void SendMessageToPlayer(const UInt64 account, const UInt16 messageId, const void* data, const int size) const override;
    void Broadcast(std::vector<UInt64>& list, const UInt16 messageId, const void* const data, const int size) const override;
};

#endif //__PlayerManager_h__
