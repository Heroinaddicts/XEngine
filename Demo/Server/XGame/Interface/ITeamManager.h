#ifndef __ITeamManager_h__
#define __ITeamManager_h__

#include "iModule.h"
#include <unordered_set>
#include "google/protobuf/message.h"

UsingXEngine;

class IGameObject;
class ITeam {
public:
    virtual ~ITeam() {}
    ITeam(const UInt64 teamID) : _TeamID(teamID) {}

    const UInt64 _TeamID;

    virtual UInt64 GetLeaderGuid() const = 0;

    virtual void AddMember(const UInt64 guid) = 0;
    virtual void RemoveMember(const UInt64 guid) = 0;
    virtual void ChangeLeader(const UInt64 guid) = 0;
    virtual bool IsTeamFighting() = 0;

    virtual const std::unordered_set<UInt64>& GetMembers() const = 0;

    bool IsMember(const UInt64 guid) const {
        return GetMembers().find(guid) != GetMembers().end();
    }

    __forceinline void SendProtobuf(const UInt16 messageId, const ::google::protobuf::Message& pb) const {
        const int size = pb.ByteSize();
        void* temp = alloca(size);
        if (pb.SerializePartialToArray(temp, size)) {
            SendMessage(messageId, temp, size);
        }
        else {
            XASSERT(false, "wtf");
        }
    }

    virtual void SendMessage(const UInt64 messageId, const void* const data, const int size) const = 0;
};

class ITeamManager : public Api::iModule {
public:
    virtual ~ITeamManager() {}

    enum class eEvent {
        TeamCreated = 1,//第一个UInt64参数是LeaderGuid，第二个UInt64参数无意义，填0
        TeamRelease = 2,//第一个UInt64参数是LeaderGuid，第二个UInt64参数无意义，填0
        TeamMemberAdded = 3,//第一个UInt64参数是LeaderGuid，第二个UInt64参数是被添加的成员Guid
        TeamMemberRemoved = 4,//第一个UInt64参数是LeaderGuid，第二个UInt64参数是被移除的成员Guid
        TeamLeaderChanged = 5//第一个UInt64参数是原LeaderGuid，第二个UInt64参数是新LeaderGuid
    };

    typedef void(*fTeamEvent)(ITeam* const team, const UInt64, const UInt64);

    virtual void RegistEvent(const eEvent ev, const fTeamEvent callback, const char* file, const int line) = 0;

    virtual ITeam* CreateTeam(const UInt64 leaderGuid) = 0;
    virtual void ReleaseTeam(const UInt64 teamID) = 0;

    virtual ITeam* QueryTeam(const UInt64 guid) const = 0;
    virtual ITeam* QueryTeamByEntityGuid(const UInt64 guid) const = 0;

    __forceinline bool IsSameTeam(const UInt64 guid1, const UInt64 guid2) const {
        return QueryTeamByEntityGuid(guid1) == QueryTeamByEntityGuid(guid2);
    }
};

#endif //__ITeamManager_h__
