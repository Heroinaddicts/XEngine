#include "MatchManager.h"
#include "Match.h"

Api::iEngine* g_Engine = nullptr;
ISceneManager* g_SceneManager = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
IFsm* g_Fsm = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
ISkillManager* g_SkillManager = nullptr;
ITeamManager* g_TeamManager = nullptr;

static MatchManager* s_Self = nullptr;

static std::unordered_map<Int64, std::unordered_map<UInt64, UInt64>> s_MatchMapTeamsBySceneId;
static std::unordered_map<UInt64, IMatch*> s_MatchMap;

enum  eTimerID
{
    TryMatching = 0,
    MatchSuccess = 1,
};

bool MatchManager::Initialize(Api::iEngine* const engine) {

    g_Engine = engine;
    s_Self = this;
    return true;
}

bool MatchManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_Fsm, Fsm);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_SkillManager, SkillManager);
    GetModule(g_Engine, g_TeamManager, TeamManager);

    g_PlayerManager->RegistPlayerProtobuf(eClientID::MatchApplyReq, MatchManager::OnMatchApplyReq);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::MatchCancleReq, MatchManager::OnMatchCancleReq);
    START_TIMER(g_Engine, s_Self, eTimerID::TryMatching, 250, Api::Unlimited, 250, 0);
    return true;
}

bool MatchManager::IsMatching(const UInt64 guid)
{
    UInt64 matchGuid = guid;
    if (g_TeamManager->QueryTeamByEntityGuid(matchGuid) != nullptr)
    {
        matchGuid = g_TeamManager->QueryTeamByEntityGuid(matchGuid)->_TeamID;
    }

    bool isMatching = false;

    for (auto it = s_MatchMap.begin(); it != s_MatchMap.end(); it++)
    {
        if (it->second->IsMemberTeam(matchGuid))
        {
            isMatching = true;
            break;
        }
    }

    for (auto it = s_MatchMapTeamsBySceneId.begin(); it != s_MatchMapTeamsBySceneId.end(); it++)
    {
        if (it->second.find(matchGuid) != it->second.end())
        {
            isMatching = true;
            break;
        }
    }

    return isMatching;
}

Int64 MatchManager::GetMatchSceneId(const UInt64 guid)
{
    Int64 sceneId = 0;
    for (auto it = s_MatchMapTeamsBySceneId.begin(); it != s_MatchMapTeamsBySceneId.end(); it++)
    {
        if (it->second.find(guid) != it->second.end())
        {
            sceneId = it->first;
            break;
        }
    }

    return sceneId;
}

bool MatchManager::IsMatchTimeOut()
{
    bool isMatchTimeOut = false;
    Int64 sceneId = 0;
    UInt64 guid = 0;
    for (auto it = s_MatchMapTeamsBySceneId.begin(); it != s_MatchMapTeamsBySceneId.end(); it++)
    {
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++)
        {
            if (it1->second != INVALID_GAMEOBJECT_ID && SafeSystem::Time::GetMilliSecond() - it1->second >= AutoConfig::Const::MatchRules::MaxTime)
            {
                sceneId = it->first;
                guid = it1->first;
                isMatchTimeOut = true;
                it1->second = INVALID_GAMEOBJECT_ID;
                break;
            }
        }
    }

    //处理超时进入副本
    if (isMatchTimeOut)
    {
        TimeOutMatchSuccess(sceneId, guid);
    }

    return isMatchTimeOut;
}

void MatchManager::TimeOutMatchSuccess(const Int64 sceneId, const UInt64 guid)
{
    IMatch* match = CreateMatch(sceneId);

    match->AddTeam(guid);

    auto itor = s_MatchMapTeamsBySceneId.find(sceneId);
    if (itor == s_MatchMapTeamsBySceneId.end())
    {
        ErrorLog(g_Engine, "MatchMapTeams has not sceneId %llu data", sceneId);
        return;
    }

    for (auto it = itor->second.begin(); it != itor->second.end(); it++)
    {
        if (it->first != guid)
        {
            match->AddTeam(it->first);
        }
    }

    //发送匹配成功
    oMatchSuccessPush push;

    for (auto it = match->GetTeams().begin(); it != match->GetTeams().end(); it++)
    {
        if (g_TeamManager->QueryTeam(*it) != nullptr)
        {
            ITeam* team = g_TeamManager->QueryTeam(*it);
            team->SendProtobuf(eServerID::MatchSuccessPush, push);
        }
        else
        {
            UInt64 account = g_GameWorld->QueryGameObject(*it)->GetUInt64(Character::AccountUInt64);
            g_PlayerManager->SendProtobufToPlayer(account, eServerID::MatchSuccessPush, push);
        }
        auto itor = s_MatchMapTeamsBySceneId.find(sceneId);
        if (itor != s_MatchMapTeamsBySceneId.end())
        {
            itor->second.erase(*it);
        }
        else
        {
            ErrorLog(g_Engine, "MatchMapTeams has not sceneID %llu data", sceneId);
        }
    }

    START_TIMER(g_Engine, s_Self, eTimerID::MatchSuccess, AutoConfig::Const::MatchRules::Countdown, 1, 0, match->_MatchID);
}

void MatchManager::OnTimer(const int id, const UInt64 context, const Int64 timestamp)
{
    switch (id)
    {
    case eTimerID::TryMatching:
    {
        //是否对应的副本里面有匹配超时玩家
        if (IsMatchTimeOut())
        {
            return;
        }

        for (auto it = s_MatchMapTeamsBySceneId.begin(); it != s_MatchMapTeamsBySceneId.end(); it++)
        {
            TryMatch(it->first);
        }
        break;
    }
    case eTimerID::MatchSuccess:
    {
        IMatch* match = s_Self->QueryMatch(context);
        if (match == nullptr)
        {
            return;
        }

        g_SceneManager->CreateCopyScene(match->GetSceneId(),
            [=](IScene* const scene) {
                for (auto it = match->GetTeams().begin(); it != match->GetTeams().end(); it++)
                {
                    if (g_TeamManager->QueryTeam(*it) != nullptr)
                    {
                        ITeam* team = g_TeamManager->QueryTeam(*it);
                        scene->EnterScene(team);
                    }
                    else
                    {
                        scene->EnterScene(*it);
                    }
                }

                s_Self->ReleaseMatch(match->_MatchID);
            }
        );
        break;
    }
    }
}

void MatchManager::TryMatch(const Int64 sceneId)
{
    std::vector<UInt64> fourPlayers;
    std::vector<UInt64> threePlayers;
    std::vector<UInt64> twoPlayers;
    std::vector<UInt64> onePlayers;

    auto itor = s_MatchMapTeamsBySceneId.find(sceneId);
    if (itor == s_MatchMapTeamsBySceneId.end())
    {
        ErrorLog(g_Engine, "MatchMapTeams has not sceneId %llu data", sceneId);
        return;
    }

    for (auto it = itor->second.begin(); it != itor->second.end(); it++)
    {
        ITeam* team = g_TeamManager->QueryTeam(it->first);
        if (team != nullptr)
        {
            if (team->GetMembers().size() == 4)
            {
                fourPlayers.push_back(it->first);
            }
            else if (team->GetMembers().size() == 3)
            {
                threePlayers.push_back(it->first);
            }
            else if(team->GetMembers().size() == 2)
            {
                twoPlayers.push_back(it->first);
            }
        }
        else
        {
            onePlayers.push_back(it->first);
        }
    }

    std::vector<UInt64> matchedPlayersOrTeams;

    if (fourPlayers.size() < 2)
    {
        return;
    }

    matchedPlayersOrTeams.push_back(fourPlayers[0]);
    matchedPlayersOrTeams.push_back(fourPlayers[1]);

    if (threePlayers.size() >= 1 && twoPlayers.size() >= 2 && onePlayers.size() >= 1)
    {
        matchedPlayersOrTeams.push_back(threePlayers[0]);
        matchedPlayersOrTeams.push_back(twoPlayers[0]);
        matchedPlayersOrTeams.push_back(twoPlayers[1]);
        matchedPlayersOrTeams.push_back(onePlayers[0]);

        MatchSuccess(sceneId, matchedPlayersOrTeams);
        return;
    }

    if (threePlayers.size() >= 2 && twoPlayers.size() >= 1)
    {
        matchedPlayersOrTeams.push_back(threePlayers[0]);
        matchedPlayersOrTeams.push_back(threePlayers[1]);
        matchedPlayersOrTeams.push_back(twoPlayers[0]);

        MatchSuccess(sceneId, matchedPlayersOrTeams);
        return;
    }

    if (threePlayers.size() >= 2 && onePlayers.size() >= 2)
    {
        matchedPlayersOrTeams.push_back(threePlayers[0]);
        matchedPlayersOrTeams.push_back(threePlayers[1]);
        matchedPlayersOrTeams.push_back(onePlayers[0]);
        matchedPlayersOrTeams.push_back(onePlayers[1]);

        MatchSuccess(sceneId, matchedPlayersOrTeams);
        return;
    }

    if (twoPlayers.size() >= 4)
    {
        matchedPlayersOrTeams.push_back(twoPlayers[0]);
        matchedPlayersOrTeams.push_back(twoPlayers[1]);
        matchedPlayersOrTeams.push_back(twoPlayers[2]);
        matchedPlayersOrTeams.push_back(twoPlayers[3]);
        
        MatchSuccess(sceneId, matchedPlayersOrTeams);
        return;
    }

}

void MatchManager::MatchSuccess(const Int64 sceneId, std::vector<UInt64> matchedPlayersOrTeams)
{
    IMatch* match = CreateMatch(sceneId);
    for (auto it = matchedPlayersOrTeams.begin(); it != matchedPlayersOrTeams.end(); it++)
    {
        match->AddTeam(*it);
    }

    //发送匹配成功
    oMatchSuccessPush push;

    for (auto it = match->GetTeams().begin(); it != match->GetTeams().end(); it++)
    {
        if (g_TeamManager->QueryTeam(*it) != nullptr)
        {
            ITeam* team = g_TeamManager->QueryTeam(*it);
            team->SendProtobuf(eServerID::MatchSuccessPush, push);
        }
        else
        {
            UInt64 account = g_GameWorld->QueryGameObject(*it)->GetUInt64(Character::AccountUInt64);
            g_PlayerManager->SendProtobufToPlayer(account, eServerID::MatchSuccessPush, push);
        }
        auto itor = s_MatchMapTeamsBySceneId.find(sceneId);
        if (itor != s_MatchMapTeamsBySceneId.end())
        {
            itor->second.erase(*it);
        }
        else
        {
            ErrorLog(g_Engine, "MatchMapTeams has not sceneID %llu data", sceneId);
        }
    }

    START_TIMER(g_Engine, s_Self, eTimerID::MatchSuccess, AutoConfig::Const::MatchRules::Countdown, 1, 0, match->_MatchID);
}

void MatchManager::OnMatchApplyReq(IPlayer* const player, const UInt64 account, const oMatchApplyReq& pb)
{
    UInt64 matchGuid = player->GetDeployCharacterID();
    if (g_TeamManager->QueryTeamByEntityGuid(matchGuid) != nullptr)
    {
        ITeam* team = g_TeamManager->QueryTeamByEntityGuid(matchGuid);
        matchGuid = team->_TeamID;
        if (team->GetLeaderGuid() != player->GetDeployCharacterID())
        {
            TraceLog(g_Engine, "Player %llu is not leader", player->GetDeployCharacterID());
            return;
        }

        if (team->IsTeamFighting())
        {
            TraceLog(g_Engine, "team is in fighting");
            return;
        }
    }

    if (s_MatchMapTeamsBySceneId.find(pb.sceneid()) == s_MatchMapTeamsBySceneId.end())
    {
        s_MatchMapTeamsBySceneId.insert({ pb.sceneid(), {} });
    }

    if (s_Self->IsMatching(player->GetDeployCharacterID()))
    {
        TraceLog(g_Engine, "Player %llu is matching", player->GetDeployCharacterID());
        return;
    }

    s_MatchMapTeamsBySceneId.find(pb.sceneid())->second.insert({ matchGuid, SafeSystem::Time::GetMilliSecond() });

    //发送开始匹配
    oMatchApplyReqPush push;
    push.set_sceneid(pb.sceneid());

    if (g_TeamManager->QueryTeam(matchGuid) != nullptr)
    {
        ITeam* team = g_TeamManager->QueryTeam(matchGuid);
        team->SendProtobuf(eServerID::MatchApplyReqPush, push);
    }
    else
    {
        UInt64 account = g_GameWorld->QueryGameObject(matchGuid)->GetUInt64(Character::AccountUInt64);
        g_PlayerManager->SendProtobufToPlayer(account, eServerID::MatchApplyReqPush, push);
    }
}

void MatchManager::OnMatchCancleReq(IPlayer* const player, const UInt64 account, const oMatchCancleReq& pb)
{
    UInt64 matchGuid = player->GetDeployCharacterID();
    if (g_TeamManager->QueryTeamByEntityGuid(matchGuid) != nullptr)
    {
        matchGuid = g_TeamManager->QueryTeamByEntityGuid(matchGuid)->_TeamID;
    }

    /*if (!s_Self->IsMatching(player->GetDeployCharacterID()))
    {
        TraceLog(g_Engine, "Player %llu is not matching", player->GetDeployCharacterID());
        return;
    }*/

    //发送取消匹配
    oMatchCancleReqPush push;

    if (g_TeamManager->QueryTeam(matchGuid) != nullptr)
    {
        ITeam* team = g_TeamManager->QueryTeam(matchGuid);
        team->SendProtobuf(eServerID::MatchCancleReqPush, push);
    }
    else
    {
        UInt64 account = g_GameWorld->QueryGameObject(matchGuid)->GetUInt64(Character::AccountUInt64);
        g_PlayerManager->SendProtobufToPlayer(account, eServerID::MatchCancleReqPush, push);
    }

    Int64 sceneId = s_Self->GetMatchSceneId(matchGuid);

    auto itor = s_MatchMapTeamsBySceneId.find(sceneId);
    if (itor != s_MatchMapTeamsBySceneId.end())
    {
        itor->second.erase(matchGuid);
    }
    else
    {
        ErrorLog(g_Engine, "MatchMapTeams has not sceneId %llu data", sceneId);
    }
}

bool MatchManager::Destroy(Api::iEngine* const engine) {
    return true;
}

IMatch* MatchManager::CreateMatch(const Int64 sceneId)
{
    IMatch* match = txnew Match(g_GameWorld->GenerateGuid(), sceneId);
    s_MatchMap.insert({ match->_MatchID, match });

    return match;
}

IMatch* MatchManager::QueryMatch(const UInt64 guid)
{
    if (s_MatchMap.find(guid) != s_MatchMap.end())
    {
        return s_MatchMap.find(guid)->second;
    }

    return nullptr;
}

void MatchManager::ReleaseMatch(const UInt64 guid)
{
    if (s_MatchMap.find(guid) != s_MatchMap.end())
    {
        txdel s_MatchMap.find(guid)->second;
        s_MatchMap.erase(guid);
    }
}
