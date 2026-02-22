#include "TeamManager.h"
#include "Team.h"

Api::iEngine* g_Engine = nullptr;
IGameWorld* g_GameWorld = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
ICharacterManager* g_CharacterManager = nullptr;
IConfig* g_Config = nullptr;
IMatchManager* g_MatchManager;

static TeamManager* s_Self = nullptr;

XEventPool<ITeamManager::eEvent, ITeam* const, const UInt64, const UInt64> g_TeamEvent;

bool TeamManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool TeamManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_CharacterManager, CharacterManager);
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_MatchManager, MatchManager);

    g_PlayerManager->RegistPlayerProtobuf(eClientID::TeamInviteReq, OnTeamInviteReq);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::TeamInviteReply, OnTeamInviteReply);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::TeamApplyReq, OnTeamApplyReq);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::TeamApplyReply, OnTeamApplyReply);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::TeamQuitReq, OnTeamQuitReq);

    g_GameWorld->RegistGameObjectAttributeChanged(eGameObjectType::Entity, OnTeamMemberAttributeChanged);

    g_CharacterManager->RegistCharacterDockerEvent(ICharacterManager::eCharacterDockerEvent::CharacterDockerCreated, TeamManager::OnCharacterDockerLoaded);
    g_CharacterManager->RegistCharacterDockerEvent(ICharacterManager::eCharacterDockerEvent::CharacterDockerLoaded, TeamManager::OnCharacterDockerLoaded);
    return true;
}

bool TeamManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void TeamManager::OnCharacterDockerLoaded(ICharacterDocker* const docker, IGameObject* const gameObject) {
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterCreated, TeamManager::OnCharacterCreated);
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterLoaded, TeamManager::OnCharacterLoaded);
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterRelease, TeamManager::OnCharacterRelease);
}

void TeamManager::OnCharacterCreated(ICharacterDocker* const, IGameObject* const character) {

}

void TeamManager::OnCharacterLoaded(ICharacterDocker* const, IGameObject* const character) {
    ITeam* team = Team::QueryTeamByEntityGuid(character->Guid());
    if (team != nullptr)
    {
        oTeamInfoPush push;
        oTeam* oteam = push.mutable_teaminfo();
        oteam->set_teamguid(team->_TeamID);
        oteam->set_leaderguid(team->GetLeaderGuid());
        for (auto it = team->GetMembers().begin(); it != team->GetMembers().end(); it++) {
            IGameObject* member = g_GameWorld->QueryGameObject(*it);
            if (member != nullptr)
            {
                oTeamMemberInfo* teamMemberInfo = oteam->add_memberinfos();
                teamMemberInfo->set_guid(*it);
                teamMemberInfo->set_hp(member->GetFloat(Entity::HpFloat));
                teamMemberInfo->set_maxhp(member->GetFloat(Entity::MaxHpFloat));
                teamMemberInfo->set_mp(member->GetFloat(Entity::MpFloat));
                teamMemberInfo->set_maxmp(member->GetFloat(Entity::MaxMpFloat));
                teamMemberInfo->set_nickname(member->GetString(Character::NicknameString));
                teamMemberInfo->set_configid(member->ConfigID());
            }
        }

        team->SendProtobuf(eServerID::TeamInfoPush, push);
    }       


}

void TeamManager::OnCharacterRelease(ICharacterDocker* const docker, IGameObject* const character) {
    ITeam* team = Team::QueryTeamByEntityGuid(character->Guid());
    if (team != nullptr)
    {
        team->RemoveMember(character->Guid());
        if (team->GetMembers().size() <= 1)
        {
            s_Self->ReleaseTeam(team->_TeamID);
        }
        else
        {
            oTeamInfoPush push;
            oTeam* oteam = push.mutable_teaminfo();
            oteam->set_teamguid(team->_TeamID);
            oteam->set_leaderguid(team->GetLeaderGuid());
            for (auto it = team->GetMembers().begin(); it != team->GetMembers().end(); it++) {
                IGameObject* member = g_GameWorld->QueryGameObject(*it);
                if (member != nullptr)
                {
                    oTeamMemberInfo* teamMemberInfo = oteam->add_memberinfos();
                    teamMemberInfo->set_guid(*it);
                    teamMemberInfo->set_hp(member->GetFloat(Entity::HpFloat));
                    teamMemberInfo->set_maxhp(member->GetFloat(Entity::MaxHpFloat));
                    teamMemberInfo->set_mp(member->GetFloat(Entity::MpFloat));
                    teamMemberInfo->set_maxmp(member->GetFloat(Entity::MaxMpFloat));
                    teamMemberInfo->set_nickname(member->GetString(Character::NicknameString));
                    teamMemberInfo->set_configid(member->ConfigID());
                }
            }

            team->SendProtobuf(eServerID::TeamInfoPush, push);
        }
    }
}

void TeamManager::OnTeamQuitReq(IPlayer* const, const UInt64 account, const oTeamQuitReq& pb) {
    UInt64 guid = g_CharacterManager->QueryCharacterDocker(account)->GetDeployCharacter()->Guid();
    ITeam* team = Team::QueryTeamByEntityGuid(guid);
    if (team == nullptr)
    {
        TraceLog(g_Engine, "Player %llu is not in a team", guid);
        return;
    }

    team->RemoveMember(guid);
    if (team->GetMembers().size() <= 1)
    {
        s_Self->ReleaseTeam(team->_TeamID);
    }
    else
    {
        oTeamInfoPush push;
        oTeam* oteam = push.mutable_teaminfo();
        oteam->set_teamguid(team->_TeamID);
        oteam->set_leaderguid(team->GetLeaderGuid());
        for (auto it = team->GetMembers().begin(); it != team->GetMembers().end(); it++) {
            IGameObject* member = g_GameWorld->QueryGameObject(*it);
            if (member != nullptr)
            {
                oTeamMemberInfo* teamMemberInfo = oteam->add_memberinfos();
                teamMemberInfo->set_guid(*it);
                teamMemberInfo->set_hp(member->GetFloat(Entity::HpFloat));
                teamMemberInfo->set_maxhp(member->GetFloat(Entity::MaxHpFloat));
                teamMemberInfo->set_mp(member->GetFloat(Entity::MpFloat));
                teamMemberInfo->set_maxmp(member->GetFloat(Entity::MaxMpFloat));
                teamMemberInfo->set_nickname(member->GetString(Character::NicknameString));
                teamMemberInfo->set_configid(member->ConfigID());
            }
        }

        team->SendProtobuf(eServerID::TeamInfoPush, push);
    }
}

void TeamManager::OnTeamApplyReply(IPlayer* const, const UInt64 account, const oTeamApplyReply& pb) {
    IGameObject* applyer = g_GameWorld->QueryGameObject(pb.applyguid());
    if (applyer == nullptr || applyer->Type() != eGameObjectType::Character) {
        ErrorLog(g_Engine, "Cannot find applyer %llu, it's not a character", pb.applyguid());
        return;
    }
    IGameObject* target = g_CharacterManager->QueryCharacterDocker(account)->GetDeployCharacter();
    if (applyer->Guid() == target->Guid())
    {
        ErrorLog(g_Engine, "Applyer %llu cannot invite self", applyer->Guid());
        return;
    }

    if (Team::QueryTeamByEntityGuid(applyer->Guid()))
    {
        TraceLog(g_Engine, "Applyer is already in a team");
        return;
    }

    if (g_MatchManager->IsMatching(applyer->Guid()) || g_MatchManager->IsMatching(target->Guid()))
    {
        TraceLog(g_Engine, "Someone Is Matching");
        return;
    }

    if(pb.agree())
    {
        ITeam* team = Team::QueryTeamByEntityGuid(target->Guid());

        if (team != nullptr && team->GetMembers().size() >= AutoConfig::Const::Partner::CountLimit)
        {
            oTeamAddFailPush failPush;
            failPush.set_error(eError::MembersFull);
            failPush.set_operate(eTeamOperate::Apply);
            failPush.set_operateguid(target->Guid());
            failPush.set_nickname(target->GetString(Character::AccountUInt64));
            g_PlayerManager->SendProtobufToPlayer(applyer->GetUInt64(Character::AccountUInt64), eServerID::TeamAddFailPush, failPush);
            return;
        }

        if (team == nullptr) {
            team = s_Self->CreateTeam(target->Guid());
        }
        team->AddMember(applyer->Guid());

        oTeamInfoPush push;
        oTeam* oteam = push.mutable_teaminfo();
        oteam->set_teamguid(team->_TeamID);
        oteam->set_leaderguid(team->GetLeaderGuid());
        for (auto it = team->GetMembers().begin(); it != team->GetMembers().end(); it++) {
            IGameObject* member = g_GameWorld->QueryGameObject(*it);
            if (member != nullptr)
            {
                oTeamMemberInfo* teamMemberInfo = oteam->add_memberinfos();
                teamMemberInfo->set_guid(*it);
                teamMemberInfo->set_hp(member->GetFloat(Entity::HpFloat));
                teamMemberInfo->set_maxhp(member->GetFloat(Entity::MaxHpFloat));
                teamMemberInfo->set_mp(member->GetFloat(Entity::MpFloat));
                teamMemberInfo->set_maxmp(member->GetFloat(Entity::MaxMpFloat));
                teamMemberInfo->set_nickname(member->GetString(Character::NicknameString));
                teamMemberInfo->set_configid(member->ConfigID());
            }
        }

        team->SendProtobuf(eServerID::TeamInfoPush, push);
    }
    else
    {
        oTeamAddFailPush failPush;
        failPush.set_error(eError::Refuse);
        failPush.set_operate(eTeamOperate::Apply);
        failPush.set_operateguid(target->Guid());
        failPush.set_nickname(target->GetString(Character::NicknameString));
        g_PlayerManager->SendProtobufToPlayer(applyer->GetUInt64(Character::AccountUInt64), eServerID::TeamAddFailPush, failPush);
    }
}

void TeamManager::OnTeamApplyReq(IPlayer* const, const UInt64 account, const oTeamApplyReq& pb) {
    IGameObject* target = g_GameWorld->QueryGameObject(pb.targetguid());
    if (target == nullptr || target->Type() != eGameObjectType::Character) {
        ErrorLog(g_Engine, "Cannot find target %llu, it's not a character", pb.targetguid());
    }

    if (g_CharacterManager->QueryCharacterDocker(target->GetUInt64(Character::AccountUInt64))->GetDeployCharacter()->Guid() != pb.targetguid()) {
        ErrorLog(g_Engine, "Target %llu is not deployed character", pb.targetguid());
        return;
    }

    if (target->GetUInt64(Entity::TeamGuidUInt64) == INVALID_GAMEOBJECT_ID)
    {
        TraceLog(g_Engine, "Target %llu is not in a team", pb.targetguid());
        return;
    }

    IGameObject* applyer = g_CharacterManager->QueryCharacterDocker(account)->GetDeployCharacter();
    if (applyer->Guid() == pb.targetguid())
    {
        ErrorLog(g_Engine, "Applyer %llu cannot apply self", applyer->Guid());
        return;
    }

    if (g_MatchManager->IsMatching(applyer->Guid()) || g_MatchManager->IsMatching(target->Guid()))
    {
        TraceLog(g_Engine, "Someone Is Matching");
        return;
    }

    /*if (Team::QueryTeamByEntityGuid(target->Guid())->GetMembers().size() >= AutoConfig::Const::Partner::CountLimit)
    {
        TraceLog(g_Engine, "Team is full");
        return;
    }*/

    oTeamApplyReqPush reqPush;
    reqPush.set_applyguid(applyer->Guid());
    reqPush.set_nickname(applyer->GetString(Character::NicknameString));
    g_PlayerManager->SendProtobufToPlayer(target->GetUInt64(Character::AccountUInt64), eServerID::TeamApplyReqPush, reqPush);
}

void TeamManager::OnTeamInviteReply(IPlayer* const, const UInt64 account, const oTeamInviteReply& pb) {
    IGameObject* inviter = g_GameWorld->QueryGameObject(pb.inviteguid());
    if (inviter == nullptr || inviter->Type() != eGameObjectType::Character) {
        ErrorLog(g_Engine, "Cannot find inviter %llu, it's not a character", pb.inviteguid());
        return;
    }
    IGameObject* target = g_CharacterManager->QueryCharacterDocker(account)->GetDeployCharacter();
    if (inviter->Guid() == target->Guid())
    {
        ErrorLog(g_Engine, "Inviter %llu cannot invite self", inviter->Guid());
        return;
    }

    if (Team::QueryTeamByEntityGuid(target->Guid()) != nullptr)
    {
        TraceLog(g_Engine, "Target is already in a team");
        return;
    }

    if (g_MatchManager->IsMatching(inviter->Guid()) || g_MatchManager->IsMatching(target->Guid()))
    {
        TraceLog(g_Engine, "Someone Is Matching");
        return;
    }

    if (pb.agree())
    {
        ITeam* team = Team::QueryTeamByEntityGuid(inviter->Guid());
        if (team != nullptr && team->GetMembers().size() >= AutoConfig::Const::Partner::CountLimit)
        {
            oTeamAddFailPush failPush;
            failPush.set_error(eError::MembersFull);
            failPush.set_operate(eTeamOperate::Invite);
            failPush.set_operateguid(target->Guid());
            failPush.set_nickname(target->GetString(Character::NicknameString));
            g_PlayerManager->SendProtobufToPlayer(account, eServerID::TeamAddFailPush, failPush);
            return;
        }

        if (team == nullptr) {
            team = s_Self->CreateTeam(inviter->Guid());
        }
        team->AddMember(target->Guid());

        oTeamInfoPush push;
        oTeam* oteam = push.mutable_teaminfo();
        oteam->set_teamguid(team->_TeamID);
        oteam->set_leaderguid(team->GetLeaderGuid());
        for (auto it = team->GetMembers().begin(); it != team->GetMembers().end(); it++) {
            IGameObject* member = g_GameWorld->QueryGameObject(*it);
            if (member != nullptr)
            {
                oTeamMemberInfo* teamMemberInfo = oteam->add_memberinfos();
                teamMemberInfo->set_guid(*it);
                teamMemberInfo->set_hp(member->GetFloat(Entity::HpFloat));
                teamMemberInfo->set_maxhp(member->GetFloat(Entity::MaxHpFloat));
                teamMemberInfo->set_mp(member->GetFloat(Entity::MpFloat));
                teamMemberInfo->set_maxmp(member->GetFloat(Entity::MaxMpFloat));
                teamMemberInfo->set_nickname(member->GetString(Character::NicknameString));
                teamMemberInfo->set_configid(member->ConfigID());
            }
        }

        team->SendProtobuf(eServerID::TeamInfoPush, push);
    }
    else
    {
        oTeamAddFailPush failPush;
        failPush.set_error(eError::Refuse);
        failPush.set_operate(eTeamOperate::Invite);
        failPush.set_operateguid(target->Guid());

        g_PlayerManager->SendProtobufToPlayer(inviter->GetUInt64(Character::AccountUInt64), eServerID::TeamAddFailPush, failPush);
    }
}

void TeamManager::OnTeamInviteReq(IPlayer* const, const UInt64 account, const oTeamInviteReq& pb) {
    IGameObject* target = g_GameWorld->QueryGameObject(pb.targetguid());
    if (target == nullptr || target->Type() != eGameObjectType::Character) {
        TraceLog(g_Engine, "Cannot find invitee %llu, it's not a character", pb.targetguid());
        return;
    }

    if (g_CharacterManager->QueryCharacterDocker(target->GetUInt64(Character::AccountUInt64))->GetDeployCharacter()->Guid() != pb.targetguid()) {
        ErrorLog(g_Engine, "Invitee %llu is not deployed character", pb.targetguid());
        return;
    }

    IGameObject* inviter = g_CharacterManager->QueryCharacterDocker(account)->GetDeployCharacter();
    if (inviter->Guid() == pb.targetguid()) {
        TraceLog(g_Engine, "Inviter %llu cannot invite self", inviter->Guid());
        return;
    }

    if (Team::QueryTeamByEntityGuid(pb.targetguid())) {
        TraceLog(g_Engine, "Invitee %llu is already in a team", pb.targetguid());
        return;
    }

    if (g_MatchManager->IsMatching(inviter->Guid()) || g_MatchManager->IsMatching(target->Guid()))
    {
        TraceLog(g_Engine, "Someone Is Matching");
        return;
    }

    /*if (Team::QueryTeamByEntityGuid(inviter->Guid())->GetMembers().size() >= AutoConfig::Const::Partner::CountLimit)
    {
        TraceLog(g_Engine, "Team is full");
        oTeamAddFailPush push;
        push.set_error(eError::MembersFull);
        push.set_operate(eTeamOperate::Invite);
        push.set_operateguid(inviter->Guid());
        g_PlayerManager->SendProtobufToPlayer(account, eServerID::TeamAddFailPush, push);
        return;
    }*/

    oTeamInviteReqPush reqPush;
    reqPush.set_inviteguid(inviter->Guid());
    reqPush.set_nickname(inviter->GetString(Character::NicknameString));
    g_PlayerManager->SendProtobufToPlayer(target->GetUInt64(Character::AccountUInt64), eServerID::TeamInviteReqPush, reqPush);
}

void TeamManager::RegistEvent(const eEvent ev, const fTeamEvent callback, const char* file, const int line) {
    g_TeamEvent.Register(ev, callback, file, line);
}

ITeam* TeamManager::CreateTeam(const UInt64 leaderGuid) {
    Team* team = Team::QueryTeamByEntityGuid(leaderGuid);
    if (team != nullptr) {
        ErrorLog(g_Engine, "Entity %llu is already in a team %llu", leaderGuid, team->_TeamID);
        return nullptr;
    }
    team = txnew Team(g_GameWorld->GenerateGuid(), leaderGuid);
    return team;
}

void TeamManager::ReleaseTeam(const UInt64 teamID) {
    Team* team = Team::QueryTeam(teamID);
    if (team == nullptr) {
        ErrorLog(g_Engine, "Cannot find team %llu to release", teamID);
        return;
    }

    std::vector<UInt64> removeGuids;
    for (auto it = team->GetMembers().begin(); it != team->GetMembers().end(); it++) {
        removeGuids.push_back(*it);
    }

    for (auto it = removeGuids.begin(); it != removeGuids.end(); it++) {
        team->RemoveMember(*it);
    }

    txdel team;
}

ITeam* TeamManager::QueryTeam(const UInt64 guid) const {
    return Team::QueryTeam(guid);
}

ITeam* TeamManager::QueryTeamByEntityGuid(const UInt64 guid) const {
    return Team::QueryTeamByEntityGuid(guid);
}

void TeamManager::OnTeamMemberAttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue)
{
    if (index == Entity::HpFloat || index == Entity::MaxHpFloat || index == Entity::MpFloat || index == Entity::MaxMpFloat)
    {
        ITeam* team = Team::QueryTeamByEntityGuid(go->Guid());
        if (team != nullptr)
        {
            oTeamInfoPush push;
            oTeam* oteam = push.mutable_teaminfo();
            oteam->set_teamguid(team->_TeamID);
            oteam->set_leaderguid(team->GetLeaderGuid());
            for (auto it = team->GetMembers().begin(); it != team->GetMembers().end(); it++) {
                IGameObject* member = g_GameWorld->QueryGameObject(*it);
                if (member != nullptr)
                {
                    oTeamMemberInfo* teamMemberInfo = oteam->add_memberinfos();
                    teamMemberInfo->set_guid(*it);
                    teamMemberInfo->set_hp(member->GetFloat(Entity::HpFloat));
                    teamMemberInfo->set_maxhp(member->GetFloat(Entity::MaxHpFloat));
                    teamMemberInfo->set_mp(member->GetFloat(Entity::MpFloat));
                    teamMemberInfo->set_maxmp(member->GetFloat(Entity::MaxMpFloat));
                    teamMemberInfo->set_nickname(member->GetString(Character::NicknameString));
                    teamMemberInfo->set_configid(member->ConfigID());
                }
            }

            team->SendProtobuf(eServerID::TeamInfoPush, push);
        }
    }
}
