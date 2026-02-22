#include "RedisClient.h"
#include "RedisSession.h"

namespace XEngine {
    RedisClient* RedisClient::GetInstace() {
        static RedisClient s_Instance;
        return &s_Instance;
    }
    bool RedisClient::Initialize(Api::iEngine* const engine) {
        return false;
    }

    bool RedisClient::Launch(Api::iEngine* const engine) {
        return false;
    }

    void RedisClient::Release(Api::iEngine* const engine) {
    }

    void RedisClient::EarlyUpdate(Api::iEngine* const engine) {
    }

    void RedisClient::Update(Api::iEngine* const engine) {
    }

    void RedisClient::LaterUpdate(Api::iEngine* const engine) {
    }

    void RedisClient::ConnectRedis(const std::string& host, const int port, const std::string& password, Api::fConnectRedisCallback callback) {
        Engine::GetInstance()->GetTaskApi()->PushTask(Api::eTaskProcessorType::SeparateThread,
            [=](Api::iTaskContext& context) {
                redisContext* con = redisConnect(host.c_str(), port);
                if (nullptr == con || con->err) {
                    if (con) {
                        ErrorLog(g_Engine, "Connection error: %s", con->errstr);
                        redisFree(con);
                    }
                    else {
                        ErrorLog(g_Engine, "Connection error: can't allocate redis context");
                    }

                    return -1;
                }

                if (!password.empty()) {
                    redisReply* reply = (redisReply*)redisCommand(con, "AUTH %s", password.c_str());
                    if (nullptr == reply) {
                        ErrorLog(g_Engine, "AUTH failed:  %s", con->errstr);
                        redisFree(con);
                        return -1;
                    }
                    if (reply->type == REDIS_REPLY_ERROR) {
                        ErrorLog(g_Engine, "AUTH error: %s", reply->str);
                        freeReplyObject(reply);
                        redisFree(con);
                        return -1;
                    }
                    freeReplyObject(reply);
                }

                context.Set(con);
                return REDIS_CODE_SUCCESS;
            },
            [=](Int64 code, const Api::iTaskContext& context) {
                if (code == REDIS_CODE_SUCCESS) {
                    RedisSession* session = txnew RedisSession(context.Get<redisContext>());
                    callback(session);
                }
                else {
                    callback(nullptr);
                }
            }
        );
    }
}
