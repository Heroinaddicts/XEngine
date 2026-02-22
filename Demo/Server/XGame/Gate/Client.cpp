#include "Client.h"

static XPool<Client> s_Pool;
static UInt64 s_ClientIDAlloca = 0;

static std::unordered_map<UInt64, Client*> s_ClientMap;
static std::unordered_map<UInt64, Client*> s_AccountClientMap;

Client* Client::Create() {
    Client* client = XPOOL_CREATE(s_Pool);
    return client;
}

Client* Client::Query(const UInt64 clientId) {
    auto itor = s_ClientMap.find(clientId);
    if (s_ClientMap.end() == itor) {
        return nullptr;
    }

    return itor->second;
}

Client* Client::QueryByAccount(const UInt64 account) {
    auto itor = s_AccountClientMap.find(account);
    if (s_AccountClientMap.end() != itor) {
        return itor->second;
    }

    return nullptr;
}

void Client::Release(Client* client) {
    s_ClientMap.erase(client->_ClientID);
    s_AccountClientMap.erase(client->Account());
    XPOOL_RELEASE(s_Pool, client);
}

void Client::ReleaseAll() {
    std::vector<Client*> temp;
    for (auto it = s_ClientMap.begin(); it != s_ClientMap.end(); it++) {
        temp.push_back(it->second);
    }

    for (int i = 0; i < temp.size(); i++) {
        temp[i]->Close();
    }
}

Client::Client() : _ClientID(s_ClientIDAlloca++), _Account(INVALID_ACCOUNT), _IsLogined(false) {}

Client::~Client() {}

void Client::OnConnected() {
    DEBUG(g_Engine, "Client::OnConnected %lld", _ClientID);
    s_ClientMap.insert({ _ClientID, this });
    g_GateEventsPool.Trigger(IGate::eGateEvents::ClientConnected, this);
    g_ClientSessionCount++;

    _LastRecvTick = SafeSystem::Time::GetMilliSecond();
    START_TIMER(g_Engine, this, 0, 1000, 1, 1000, 0);
}

void Client::OnDisconnected() {
    STOP_TIMER(g_Engine, this, 0, 0);
    s_ClientMap.erase(_ClientID);
    s_AccountClientMap.erase(_Account);
    DEBUG(g_Engine, "Client::OnDisconnected %lld, account %lld", _ClientID, _Account);
    g_GateEventsPool.Trigger(IGate::eGateEvents::ClientDsiconnected, this);
    Client::Release(this);
    g_ClientSessionCount--;
}
int Client::OnReceive(const void* const content, const int size) {
    return __OnReceive__(_ClientID, content, size);
}

void Client::RelateAccountClient(const UInt64 account, Client* const client) {
    KickOldClient(account);
    s_AccountClientMap.insert({ account, client });
    g_GateEventsPool.Trigger(IGate::eGateEvents::ClientLogind, client);
}

void Client::KickOldClient(const UInt64 account) {
    auto itor = s_AccountClientMap.find(account);
    if (s_AccountClientMap.end() != itor) {
        TraceLog(g_Engine, "Kick Old Client %lld Account %lld", itor->second->_ClientID, account);
        s_ClientMap.erase(itor->second->_ClientID);
        g_GateEventsPool.Trigger(IGate::eGateEvents::ClientDsiconnected, itor->second);
        itor->second->_Account = INVALID_ACCOUNT;
        itor->second->_IsLogined = false;
        itor->second->Close();
        s_AccountClientMap.erase(account);
    }
}

int Client::__OnReceive__(const UInt64 clientId, const void* const content, const int size) {
    _LastRecvTick = SafeSystem::Time::GetMilliSecond();
    if (size < sizeof(MessageHead)) {
        return 0;
    }

    MessageHead* head = (MessageHead*)content;
    if (head->_Len > size) {
        return 0;
    }

    switch (head->_ID) {
    case eClientID::LoginReq: {
        if (_IsLogined) {
            Close();
            DEBUG(g_Engine, "Double login");
            return 0;
        }

        oLoginReq req;
        if (!req.ParseFromArray(((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead))) {
            XASSERT(false, "wtf");
            ErrorLog(g_Engine, "oLoginReq ParseFromArray error");
            Close();
            return 0;
        }


        std::string sql = "CALL `GetAccount`(" + SafeString::Int64ToString((int)req.accounttype()) + ", '" + req.unionid() + "')";
        DEBUG(g_Engine, "Login Sql : %s", sql.c_str());
        static int s_Mask = 0;
        g_DbGroup->MysqlQuery(s_Mask++, sql,
            [=](const int code, const std::vector<std::vector<std::string>>* const result) {
                if (!Client::Query(clientId)) {
                    return;
                }

                if (this->_IsLogined) {
                    Close();
                    DEBUG(g_Engine, "Double login");
                    return;
                }

                if (code != MYSQL_CODE_SUCCESS) {
                    ErrorLog(g_Engine, "Mysql query error code %d", code);
                    Close();
                    return;
                }

                if (result == nullptr) {
                    ErrorLog(g_Engine, "Mysql query error code unknown");
                    Close();
                    return;
                }

                this->_Type = req.accounttype();
                this->_Account = SafeString::StringToUInt64((*result)[0][1]);
                if (INVALID_ACCOUNT == this->_Account) {
                    ErrorLog(g_Engine, "Login error account -1");
                    Close();
                    return;
                }

                //TraceLog(g_Engine, "Login succeed account %lld", _Account);
                this->_IsLogined = true;

                RelateAccountClient(this->_Account, this);

                std::string token = GenerateToken(_Account);
                TraceLog(g_Engine, "Try Save token client %lld account %lld", this->_ClientID, this->_Account);
                g_DbGroup->RedisAsyncSet(_Account, RedisDef::GenerateRedisKey(RedisDef::AccountToken, this->_Account), token,
                    [=](const bool ret) {
                        if (!Client::Query(clientId)) {
                            return;
                        }

                        if (ret) {
                            TraceLog(g_Engine, "Save client %lld account %lld token succeed", this->_ClientID, this->_Account);
                        }
                        else {
                            ErrorLog(g_Engine, "Save account %lld token error", _Account);
                        }
                    }
                );

                oLoginAsw asw;
                asw.set_code(eError::Succeed);
                asw.set_account(_Account);
                asw.set_token(token);

                SendProtobuf(eServerID::LoginAsw, asw);
            }
        );

        break;
    }
    case eClientID::QuickLoginReq: {
        if (_IsLogined) {
            Close();
            DEBUG(g_Engine, "Double login");
            return 0;
        }

        oQuickLoginReq req;
        if (!req.ParseFromArray(((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead))) {
            XASSERT(false, "wtf");
            ErrorLog(g_Engine, "oQuickLoginReq ParseFromArray error");
            Close();
            return 0;
        }

        static int s_Mask = 0;
        g_DbGroup->RedisAsyncGet(s_Mask++, RedisDef::GenerateRedisKey(RedisDef::AccountToken, req.account()),
            [=](const bool ret, const char* data) {
                if (!Client::Query(_ClientID)) {
                    return;
                }

                oQuickLoginAsw asw;
                if (false == ret || data == nullptr || req.token() != data) {
                    asw.set_code(eError::Unkown);
                }
                else {
                    asw.set_account(_Account);
                    asw.set_code(eError::Succeed);
                    RelateAccountClient(_Account, this);
                }

                SendProtobuf(eServerID::QuickLoginAsw, asw);
            }
        );

        break;
    }
    default: {
        if (!_IsLogined) {
            Close();
            return 0;
        }

        auto itor = g_ClientMessagePool.find(head->_ID);
        if (g_ClientMessagePool.end() != itor) {
            itor->second(this, _ClientID, ((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead));
        }
        else {
            if (g_GameServerSession) {
                oGateForwordClientMessageToGame body;
                body.set_account(_Account);
                body.set_messageid(head->_ID);
                body.set_body(((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead));

                g_GameServerSession->SendProtobuf(eFrameworkID::GateForwordClientMessageToGame, body);


                //DEBUG(g_Engine, "GateForwordClientMessageToGame account %llu messageId %u", _Account, head->_ID);
            }
        }
        break;
    }
    }

    return head->_Len;
}

void Client::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    if (SafeSystem::Time::GetMilliSecond() - _LastRecvTick >= 30000) {
        //Close();
    }
}
