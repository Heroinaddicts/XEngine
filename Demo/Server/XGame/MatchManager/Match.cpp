#include "Match.h"

void Match::AddTeam(const UInt64 guid)
{
	_Teams.push_back(guid);
}

std::vector<UInt64>& Match::GetTeams()
{
	return _Teams;
}

Int64 Match::GetSceneId()
{
	return _SceneId;
}

bool Match::IsMemberTeam(const UInt64 guid)
{
	bool isMemberTeam = false;
	for (auto team : _Teams)
	{
		if (team == guid)
		{
			isMemberTeam = true;
			break;
		}
	}
	
	return isMemberTeam;
}