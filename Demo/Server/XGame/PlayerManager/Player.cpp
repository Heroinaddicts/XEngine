#include "Player.h"
#include "PlayerManager.h"

static XPool<Player> s_Pool;
static std::unordered_map<UInt64, Player*> s_PlayerMap;

Player* Player::CreatePlayer(const UInt64 account) {
    if (s_PlayerMap.find(account) != s_PlayerMap.end()) {
        XASSERT(false, "wtf");
        ErrorLog(g_Engine, "Double Create Player %llu", account);
        return nullptr;
    }

    Player* player = XPOOL_CREATE(s_Pool, account);
    s_PlayerMap.insert({ account, player });
    return player;
}

Player* Player::QueryPlayer(const UInt64 account) {
    auto itor = s_PlayerMap.find(account);
    if (s_PlayerMap.end() != itor)
        return itor->second;
    return nullptr;
}

void Player::ReleasePlayer(Player* player) {
    s_PlayerMap.erase(player->_Account);
    XPOOL_RELEASE(s_Pool, player);
}

Player::Player(const UInt64 account) : _Account(account), _Nickname(""), _Gold(0), _Diamond(0), _Status(ePlayerStatus::Loading), _DeployCharacterID(INVALID_GAMEOBJECT_ID) {}


void Player::Save() {
    g_CacheClient->Update(_Account, eTable::t_player, t_player::Account__UInt64__PrimaryUnique, _Account,
        [&](ISQLCacheRow* const row) {
            row->GetColumn(t_player::Diamond__UInt32__)->SetUInt32(_Diamond);
            row->GetColumn(t_player::Gold__UInt32__)->SetUInt32(_Gold);
            row->GetColumn(t_player::DeployCharacterID__UInt64__)->SetUInt64(_DeployCharacterID);
        },
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (!succeed) {
                ErrorLog(g_Engine, "Save Player data error");
            }
        }
    );
}

void Player::SendMessage(const UInt16 messageId, const void* data, const int size) const {
    g_Game->SendMessageToClient(_Account, messageId, data, size);
}
