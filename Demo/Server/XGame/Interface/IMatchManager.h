#ifndef __IMatchManager_h__
#define __IMatchManager_h__

#include "iModule.h"

UsingXEngine;

class IGameObject;
class IMatch {
public:
    virtual ~IMatch() {}
    IMatch(const UInt64 matchID) : _MatchID(matchID) {}

    virtual void AddTeam(const UInt64 guid) = 0;

    virtual std::vector<UInt64>& GetTeams() = 0;

    virtual Int64 GetSceneId() = 0;
    virtual bool IsMemberTeam(const UInt64 guid) = 0;

    const UInt64 _MatchID;
};

class IMatchManager : public Api::iModule {
public:
    virtual ~IMatchManager() {}

    virtual bool IsMatching(const UInt64 guid) = 0;
};

#endif //__IMatchManager_h__
