#ifndef __CharacterManager_h__
#define __CharacterManager_h__

#include "Header.h"

class CharacterManager : public ICharacterManager, public Api::iTimer {
public:
    virtual ~CharacterManager() {}

    // 通过 ICharacterManager 继承
    ICharacterDocker* CreateCharacterDocker(const UInt64 account, const char* file = __FILE__, const int line = __LINE__) override;
    ICharacterDocker* QueryCharacterDocker(const UInt64 account) override;
    void ReleaseCharacterDocker(const UInt64 account, const char* file = __FILE__, const int line = __LINE__) override;
    void RegistCharacterDockerEvent(const eCharacterDockerEvent ev, const fCharacterDockerEvent callback, const char* file = __FILE__, const int line = __LINE__) override;

    void EnterScene(IGameObject* const character, IScene* const scene);

    void SendProtobuf(const UInt64 characterId, const eServerID id, const ::google::protobuf::Message& pb) override;
    void Broadcast(const UInt64 characterId, const eServerID id, const ::google::protobuf::Message& pb, const bool excludeSelf = false) override;
    void OnTimer(const int id, const UInt64 context, const Int64 timestamp);
private:
    // 通过 ICharacterManager 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    static void OnNewPlayerCreated(IPlayer* const player, const UInt64 account);
	static void OnPlayerLoading(IPlayer* const player, const UInt64 account);
    static void OnPlayerRelease(IPlayer* const player, const UInt64 account);

    static void OnStateChange(IPlayer* const, const UInt64 account, const oStateChange& pb);
    static void OnEnterCopySceneTest(IPlayer* const, const UInt64 account, const oEnterCopySceneTest& test);

    static void OnSwitchCharacterReq(IPlayer* const player, const UInt64 account, const oSwitchCharacterReq& test);

    static void OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState state);
    static bool OnExitDeathJudge(const UInt64 guid, const AutoConfig::Enum::eState, const Vector2&, const float);

    static void OnEquipReq(IPlayer* const, const UInt64 account, const oEquipReq& req);
    static void OnUnEquipReq(IPlayer* const, const UInt64 account, const oUnEquipReq& req);

    static void OnAppear(IScene* const, const UInt64 guid, const std::vector<UInt64>&);
    static void OnDisappear(IScene* const, const UInt64 guid, const std::vector<UInt64>&);

    static void OnCharacterHpChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
    static bool OnCharacterHPChangeJudge(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
};

#endif //__CharacterManager_h__
