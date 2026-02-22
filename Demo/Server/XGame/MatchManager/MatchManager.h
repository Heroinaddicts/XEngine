#ifndef __MatchManager_h__
#define __MatchManager_h__

#include "Header.h"

class MatchManager : public IMatchManager, public Api::iTimer {
public:
    virtual ~MatchManager() {}
private:
    // Í¨¹ý IMonsterManager ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    bool IsMatching(const UInt64 guid) override;
    bool IsMatchTimeOut();
    Int64 GetMatchSceneId(const UInt64 guid);

    void TryMatch(const Int64 sceneId);

    IMatch* CreateMatch(const Int64 sceneId);
    void ReleaseMatch(const UInt64 guid);
    IMatch* QueryMatch(const UInt64 guid);
    void TimeOutMatchSuccess(const Int64 sceneId, const UInt64 guid);
    void MatchSuccess(const Int64 sceneId, std::vector<UInt64> matchedPlayersOrTeams);

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

    static void OnMatchApplyReq(IPlayer* const player, const UInt64 account, const oMatchApplyReq& pb);
    static void OnMatchCancleReq(IPlayer* const player, const UInt64 account, const oMatchCancleReq& pb);
};

#endif //__MonsterManager_h__
