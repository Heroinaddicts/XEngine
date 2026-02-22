#include "Team.h"

std::unordered_map<UInt64, Team*> Team::s_EntityGuidTeamMap;
std::unordered_map<UInt64, Team*> Team::s_TeamMap;

void Team::SendMessage(const UInt64 messageId, const void* const data, const int size) const {
    for (auto it = _Members.begin(); it != _Members.end(); it++) {
        IGameObject* member = g_GameWorld->QueryGameObject(*it);
        if (member->Type() == eGameObjectType::Character) {
            g_PlayerManager->SendMessageToPlayer(member->GetUInt64(Character::AccountUInt64), messageId, data, size);
        }
    }
}

void Team::AddMember(const UInt64 guid) {
    XASSERT(_Members.find(guid) == _Members.end(), "guid %llu already in team %llu", guid, _TeamID);
    IGameObject* member = g_GameWorld->QueryGameObject(guid);
    member->SetUInt64(Entity::TeamGuidUInt64, _TeamID);
    member->SetBool(Entity::IsTeamLeaderBool, guid == _LeaderGuid);
    _Members.insert(guid);
    s_EntityGuidTeamMap.insert({ guid, this });
    g_TeamEvent.Trigger(ITeamManager::eEvent::TeamMemberAdded, this, _LeaderGuid, guid);
}

void Team::RemoveMember(const UInt64 guid) {
    XASSERT(_Members.find(guid) != _Members.end(), "guid %llu not in team %llu", guid, _TeamID);
    IGameObject* member = g_GameWorld->QueryGameObject(guid);
    member->SetUInt64(Entity::TeamGuidUInt64, INVALID_GAMEOBJECT_ID);
    member->SetBool(Entity::IsTeamLeaderBool, false);
    _Members.erase(guid);
    s_EntityGuidTeamMap.erase(guid);
    if (guid == _LeaderGuid && _Members.size() > 0)
    {
        ChangeLeader(*(_Members.begin()));
    }
    g_TeamEvent.Trigger(ITeamManager::eEvent::TeamMemberRemoved, this, _LeaderGuid, guid);
}

void Team::ChangeLeader(const UInt64 guid) {
    XASSERT(_Members.find(guid) != _Members.end(), "guid %llu not in team %llu", guid, _TeamID);
    XASSERT(_LeaderGuid != guid, "guid %llu is already the leader of team %llu", guid, _TeamID);
    IGameObject* member = g_GameWorld->QueryGameObject(guid);
    member->SetBool(Entity::IsTeamLeaderBool, true);
    UInt64 oldLeaderGuid = _LeaderGuid;
    _LeaderGuid = guid;
    g_TeamEvent.Trigger(ITeamManager::eEvent::TeamLeaderChanged, this, oldLeaderGuid, _LeaderGuid);
}

bool Team::IsTeamFighting()
{
    bool isFighting = false;
    for (auto it = _Members.begin(); it != _Members.end(); it++) {
        IGameObject* member = g_GameWorld->QueryGameObject(*it);
        if (member->GetUInt64(Entity::CopySceneGuidUInt64) != INVALID_SCENE_GUID) {
            isFighting = true;
            break;
        }
    }

    return isFighting;
}
