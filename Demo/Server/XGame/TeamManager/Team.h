#ifndef __Team_h__
#define __Team_h__

#include "Header.h"

class Team : public ITeam {
public:
    virtual ~Team() {
        for (auto it = _Members.begin(); it != _Members.end(); it++) {
            g_TeamEvent.Trigger(ITeamManager::eEvent::TeamMemberRemoved, this, _LeaderGuid, *it);
        }
        g_TeamEvent.Trigger(ITeamManager::eEvent::TeamRelease, this, _LeaderGuid, 0);
    }

    static Team* QueryTeamByEntityGuid(const UInt64 guid) {
        auto itor = s_EntityGuidTeamMap.find(guid);
        if (itor == s_EntityGuidTeamMap.end()) {
            return nullptr;
        }
        return itor->second;
    }

    static Team* QueryTeam(const UInt64 teamID) {
        auto itor = s_TeamMap.find(teamID);
        if (itor == s_TeamMap.end()) {
            return nullptr;
        }
        return itor->second;
    }

    Team(const UInt64 teamID, const UInt64 leaderGuid)
        : ITeam(teamID), _LeaderGuid(leaderGuid) {
        g_TeamEvent.Trigger(ITeamManager::eEvent::TeamCreated, this, leaderGuid, 0);
        s_TeamMap.insert({ teamID, this });
        AddMember(leaderGuid);
    }

    __forceinline UInt64 GetLeaderGuid() const override {
        return _LeaderGuid;
    }

    __forceinline const std::unordered_set<UInt64>& GetMembers() const override {
        return _Members;
    }

    void SendMessage(const UInt64 messageId, const void* const data, const int size) const override;

    void AddMember(const UInt64 guid) override;
    void RemoveMember(const UInt64 guid) override;
    void ChangeLeader(const UInt64 guid) override;
    bool IsTeamFighting() override;
private:
    std::unordered_set<UInt64> _Members;
    UInt64 _LeaderGuid;

    static std::unordered_map<UInt64, Team*> s_EntityGuidTeamMap;
    static std::unordered_map<UInt64, Team*> s_TeamMap;
};

#endif //__Team_h__
