#include "PlayerManager.h"
#include "Player.h"

Api::iEngine* g_Engine = nullptr;
ICacheClient* g_CacheClient = nullptr;
ISceneManager* g_SceneManager = nullptr;
IConfig* g_Config = nullptr;

IGame* g_Game = nullptr;
IGameWorld* g_GameWorld = nullptr;
ICharacterManager* g_CharacterManager = nullptr;
PlayerManager* g_PlayerManager = nullptr;
XEventPool<IPlayerManager::ePlayerEvent, IPlayer*, const UInt64> PlayerManager::s_EventPool;

enum ePlayerManagerTimeID {
    DelayReleasePlayer = 0,
};

static const int s_ReleasePlayerDelay = SafeSystem::Time::Minute * 0.5;

bool PlayerManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    g_PlayerManager = this;
    return true;
}

bool PlayerManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Game, Game);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_CacheClient, CacheClient);
    GetModule(g_Engine, g_SceneManager, SceneManager);
    GetModule(g_Engine, g_CharacterManager, CharacterManager);

    g_Game->RegistGameEvent(IGame::eGameEvent::AccountConnect, PlayerManager::OnAccountConnect);
    g_Game->RegistGameEvent(IGame::eGameEvent::AccountDisconnect, PlayerManager::OnAccountDisconnect);

    RegistPlayerProtobuf(eClientID::DelayTestReq, PlayerManager::OnDelayTestReq);

    RegistPlayerProtobuf(eClientID::CreatePlayerReq, PlayerManager::OnCreatePlayerReq);
    g_CharacterManager->RegistCharacterDockerEvent(ICharacterManager::eCharacterDockerEvent::CharacterDockerCreated, PlayerManager::OnCharacterDockerCreated);
    g_CharacterManager->RegistCharacterDockerEvent(ICharacterManager::eCharacterDockerEvent::CharacterDockerLoaded, PlayerManager::OnCharacterDockerLoaded);
    return true;
}

void PlayerManager::OnCharacterDockerCreated(ICharacterDocker* docker, IGameObject* character) {
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::CharacterDeployed, PlayerManager::OnDeployCharacter);
    docker->RegistCharacterEvent(ICharacterDocker::eCharacterEvent::DefaultCharacterCreated, PlayerManager::OnDefaultCharacterCreated);
}

void PlayerManager::OnCharacterDockerLoaded(ICharacterDocker* docker, IGameObject* character) {
    if (g_PlayerManager->QueryPlayer(docker->_Account)->GetDeployCharacterID() != INVALID_GAMEOBJECT_ID) {
		docker->DeployCharacter(g_PlayerManager->QueryPlayer(docker->_Account)->GetDeployCharacterID());
    }
}

void PlayerManager::OnDefaultCharacterCreated(ICharacterDocker* const docker, IGameObject* const character) {
    Player* player = Player::QueryPlayer(docker->_Account);
    XASSERT(player, "wtf");
    if (player) {
        player->SetDeployCharacterID(character->Guid());
        docker->DeployCharacter(character->Guid());
    }
}

void PlayerManager::OnDeployCharacter(ICharacterDocker* const docker, IGameObject* const character) {
    Player* player = Player::QueryPlayer(docker->_Account);
    XASSERT(player, "wtf");
    if (player) {
        player->SetDeployCharacterID(character->Guid());
    }
}

bool PlayerManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void PlayerManager::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case ePlayerManagerTimeID::DelayReleasePlayer: {
        Player* player = Player::QueryPlayer(context);

        XASSERT(player, "wtf");
        if (nullptr == player) {
            ErrorLog(g_Engine, "s_PlayerDataMap logic error, account %llu", context);
            return;
        }

        if (player->Status() == ePlayerStatus::Loaded) {
            s_EventPool.Trigger(ePlayerEvent::PlayerRelease, player, context);
            player->Save();
        }

        TraceLog(g_Engine, "OnAccountLogout account %llu", context);
        g_CacheClient->ReleaseCache(context);
        Player::ReleasePlayer(player);
        break;
    }
    }
}

void PlayerManager::OnCreatePlayerReq(IPlayer* const nothing, const UInt64 account, const oCreatePlayerReq& req) {
    Player* player = Player::QueryPlayer(account);
    if (nullptr == player || player->Status() != ePlayerStatus::NeedCreate)
        return;

    player->SetStatus(ePlayerStatus::Creating);
    g_Game->PickName(req.nickname(), [=](const bool succeed) {
        if (!succeed) {
            oCreatePlayerAsw asw;
            asw.set_code(eError::NicknameExists);
            g_Game->SendProtobufToClient(account, eServerID::CreatePlayerAsw, asw);
            player->SetStatus(ePlayerStatus::NeedCreate);
            return;
        }

        //创建player
        g_CacheClient->Create(account, eTable::t_player,
            [&](ISQLCacheRow* const row) {
                //插入玩家数据
                row->GetColumn(t_player::Account__UInt64__PrimaryUnique)->SetUInt64(account);
                row->GetColumn(t_player::Name__String__Unique)->SetString(req.nickname());
                row->GetColumn(t_player::Diamond__UInt32__)->SetUInt32(0);
                row->GetColumn(t_player::Gold__UInt32__)->SetUInt32(0);
                row->GetColumn(t_player::DeployCharacterID__UInt64__)->SetUInt64(INVALID_GAMEOBJECT_ID);
            },
            [=](const bool succeed, const ISQLCacheTable* const tab) {
                if (!succeed) {
                    ErrorLog(g_Engine, "Create Player Insert Info PlayerID %llu Name %s Error", account, req.nickname().c_str());
                    oCreatePlayerAsw asw;
                    asw.set_code(eError::Unkown);
                    g_Game->SendProtobufToClient(account, eServerID::CreatePlayerAsw, asw);
                    player->SetStatus(ePlayerStatus::NeedCreate);
                    return;
                }

                Player* player = Player::QueryPlayer(account);
                if (player) {
                    player->SetNickname(req.nickname());
                    player->SetGold(0);
                    player->SetDiamond(0);
                    player->SetDeployCharacterID(INVALID_GAMEOBJECT_ID);

                    oCreatePlayerAsw asw;
                    asw.set_code(eError::Succeed);
                    g_Game->SendProtobufToClient(account, eServerID::CreatePlayerAsw, asw);

                    oPlayerInfoPush push;
                    push.set_account(player->AccountID());
                    push.set_nickname(player->Nickname());
                    push.set_diamond(player->Diamond());
                    push.set_gold(player->Gold());
                    player->SendProtobuf(eServerID::PlayerInfoPush, push);

                    player->SetStatus(ePlayerStatus::Loaded);
                    s_EventPool.Trigger(ePlayerEvent::NewPlayerCreated, player, account);
                }
            });
        }
    );
}

void PlayerManager::OnDelayTestReq(IPlayer* const nothing, const UInt64 account, const oDelayTestReq& req) {
    oDelayTestAsw asw;
    asw.mutable_req()->set_tick(req.tick());
    g_PlayerManager->SendProtobufToPlayer(account, eServerID::DelayTestAsw, asw);
}

void PlayerManager::OnAccountConnect(const UInt64 account) {
    STOP_TIMER(g_Engine, g_PlayerManager, ePlayerManagerTimeID::DelayReleasePlayer, account);

    Player* player = Player::QueryPlayer(account);
    if (nullptr == player) {
        player = Player::CreatePlayer(account);
        g_CacheClient->Query(account, eTable::t_player, t_player::Account__UInt64__PrimaryUnique, account,
            [=](const bool succeed, const ISQLCacheTable* const tab) {
                if (!succeed) {
                    ErrorLog(g_Engine, "Query t_player %llu error", account);
                    return;
                }

                Player* player = Player::QueryPlayer(account);
                if (nullptr == player) {
                    return;
                }

                if (nullptr == tab || 0 == tab->RowCount()) {
                    TraceLog(g_Engine, "New Player %llu, Notice player Create", account);
                    player->SetStatus(ePlayerStatus::NeedCreate);
                    //这里通知玩家新建角色
                    g_Game->SendProtobufToClient(account, eServerID::NewPlayer, oNewPlayer());
                }
                else {
                    tab->Query(t_player::Account__UInt64__PrimaryUnique, account,
                        [&](ISQLCacheRow* const row) {
                            player->SetNickname(row->GetColumn(t_player::Name__String__Unique)->GetString());
                            player->SetGold(row->GetColumn(t_player::Gold__UInt32__)->GetUInt32());
                            player->SetDiamond(row->GetColumn(t_player::Diamond__UInt32__)->GetUInt32());
                            player->SetDeployCharacterID(row->GetColumn(t_player::DeployCharacterID__UInt64__)->GetUInt64());
                            TraceLog(g_Engine, "Load player base info nickname %s, gold %u, diamond %u deploy character id %llu", player->Nickname().c_str(), player->Gold(), player->Diamond(), player->GetDeployCharacterID());
                            player->SetStatus(ePlayerStatus::Loaded);
                        }
                    );

                    oPlayerInfoPush push;
                    push.set_account(player->AccountID());
                    push.set_nickname(player->Nickname());
                    push.set_diamond(player->Diamond());
                    push.set_gold(player->Gold());

                    player->SendProtobuf(eServerID::PlayerInfoPush, push);
                    player->SetStatus(ePlayerStatus::Loaded);
					s_EventPool.Trigger(ePlayerEvent::PlayerLoading, player, account);
					s_EventPool.Trigger(ePlayerEvent::PlayerLoaded, player, account);
                }
            }
        );
    } else if (player->Status() == ePlayerStatus::Loaded) {
        oPlayerInfoPush push;
        push.set_account(player->AccountID());
        push.set_nickname(player->Nickname());
        push.set_diamond(player->Diamond());
        push.set_gold(player->Gold());
        player->SendProtobuf(eServerID::PlayerInfoPush, push);

		s_EventPool.Trigger(ePlayerEvent::PlayerLoading, player, account);
        s_EventPool.Trigger(ePlayerEvent::PlayerLoaded, player, account);
    }
    else if (player->Status() == ePlayerStatus::NeedCreate) {
        TraceLog(g_Engine, "New Player %llu, Notice player Create", account);
        g_Game->SendProtobufToClient(account, eServerID::NewPlayer, oNewPlayer());
    }
}

void PlayerManager::OnAccountDisconnect(const UInt64 account) {
    if (Player::QueryPlayer(account))
        START_TIMER(g_Engine, g_PlayerManager, ePlayerManagerTimeID::DelayReleasePlayer, s_ReleasePlayerDelay, 1, s_ReleasePlayerDelay, account);
}

IPlayer* PlayerManager::QueryPlayer(const UInt64 account) const {
    return Player::QueryPlayer(account);
}

void PlayerManager::RegistPlayerMessage(const UInt16 messageId, const IPlayerManager::fPlayerMessageCall fun) {
    if (nullptr == g_Game)
        GetModule(g_Engine, g_Game, Game);

    g_Game->RegistClientMessage(messageId,
        [=](const UInt64 account, const void* const data, const int size) {
            fun(QueryPlayer(account), account, data, size);
        }
    );
}

void PlayerManager::SendMessageToPlayer(const UInt64 account, const UInt16 messageId, const void* data, const int size) const {
    if (Player::QueryPlayer(account))
        g_Game->SendMessageToClient(account, messageId, data, size);
}

void PlayerManager::Broadcast(std::vector<UInt64>& list, const UInt16 messageId, const void* const data, const int size) const {
    g_Game->Broadcast(list, messageId, data, size);
    list.clear();
}

void PlayerManager::RegistPlayerEvent(const ePlayerEvent ev, const fPlayerEvent fun) {
    XEventPoolRegist(s_EventPool, ev, fun);
}
