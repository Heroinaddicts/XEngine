#ifndef __TeamManager_h__
#define __TeamManager_h__

#include "Header.h"

class TeamManager : public ITeamManager {
public:
    virtual ~TeamManager() {}

    // 通过 ITeamManager 继承
    void RegistEvent(const eEvent ev, const fTeamEvent callback, const char* file, const int line) override;
    ITeam* CreateTeam(const UInt64 leaderGuid) override;
    void ReleaseTeam(const UInt64 teamID) override;
    ITeam* QueryTeam(const UInt64 guid) const override;
    ITeam* QueryTeamByEntityGuid(const UInt64 guid) const override;

private:
    // 通过 ITeamManager 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

private:
    static void OnTeamInviteReq(IPlayer* const, const UInt64 account, const oTeamInviteReq& pb);
    static void OnTeamInviteReply(IPlayer* const, const UInt64 account, const oTeamInviteReply& pb);
    static void OnTeamApplyReq(IPlayer* const, const UInt64 account, const oTeamApplyReq& pb);
    static void OnTeamApplyReply(IPlayer* const, const UInt64 account, const oTeamApplyReply& pb);
    static void OnTeamQuitReq(IPlayer* const, const UInt64 account, const oTeamQuitReq& pb);

    static void OnCharacterDockerLoaded(ICharacterDocker* const docker, IGameObject* const gameObject);
    static void OnCharacterLoaded(ICharacterDocker* const docker, IGameObject* const character);
    static void OnCharacterCreated(ICharacterDocker* const docker, IGameObject* const character);
    static void OnCharacterRelease(ICharacterDocker* const docker, IGameObject* const character);

    static void OnTeamMemberAttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
};

#endif //__TeamManager_h__
