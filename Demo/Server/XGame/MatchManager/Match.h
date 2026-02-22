#ifndef __Match_h__
#define __Match_h__

#include "Header.h"

class Match :public IMatch
{
public:
	~Match()
	{
		_Teams.clear();
	}
	Match(const UInt64 matchId, const Int64 sceneId)
		:IMatch(matchId), _SceneId(sceneId)
	{

	}
	std::vector<UInt64>& GetTeams() override;
	void AddTeam(const UInt64 guid) override;
	Int64 GetSceneId() override;
	bool IsMemberTeam(const UInt64 guid) override;
private:
	Int64 _SceneId;
	std::vector<UInt64> _Teams;
};

#endif //__Match_h__