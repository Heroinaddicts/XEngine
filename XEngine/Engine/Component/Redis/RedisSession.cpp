#include "RedisSession.h"

namespace XEngine {

    Int64 RedisSession::s_IndexOffset = 0;

    void RedisSession::AsyncGet(const std::string& key, const Api::fStringResult& callback) {
        if (_IsClosed) {
            callback(false, nullptr);
            return;
        }

        g_Engine->GetTaskApi()->PushTask(Api::eTaskProcessorType::OrderedQueue,
            [=](Api::iTaskContext& context) {
                AUTO_LOCK(_Lock);
                redisReply* reply = (redisReply*)redisCommand(_Con, "GET %s", key.c_str());
                if (nullptr == reply) {
                    ErrorLog(g_Engine, "redis read %s error : %s, code %d", key.c_str(), _Con->errstr, _Con->err);
                    return _Con->err;
                }
                switch (reply->type) {
                case REDIS_REPLY_STRING:
                case REDIS_REPLY_NIL:
                    context.Set(reply);
                    return REDIS_CODE_SUCCESS;
                case REDIS_REPLY_ERROR:
                    ErrorLog(g_Engine, "redis read %s error : %s", key.c_str(), reply->str);
                    freeReplyObject(reply);
                    return -1;
                default:
                    ErrorLog(g_Engine, "GET returned unexpected type: %d", reply->type);
                    freeReplyObject(reply);
                    return -1;
                }

                return REDIS_CODE_SUCCESS;
            },
            [=](Int64 code, const Api::iTaskContext& context) {
                if (code == REDIS_CODE_SUCCESS) {
                    redisReply* reply = context.Get<redisReply>();

                    switch (reply->type) {
                    case REDIS_REPLY_STRING:
                        callback(true, reply->str);
                        break;
                    case REDIS_REPLY_NIL:
                        callback(false, "");
                        break;
                    }

                    freeReplyObject(reply);
                }
                else {
                    callback(false, "");
                }
            },
            _Index
        );
    }

    void RedisSession::AsyncSet(const std::string& key, const std::string& value, const Api::fBoolResult& callback) {
        if (_IsClosed) {
            callback(false);
            return;
        }
        g_Engine->GetTaskApi()->PushTask(Api::eTaskProcessorType::OrderedQueue,
            [=](Api::iTaskContext& context) -> Int64 {
                AUTO_LOCK(_Lock);
                redisReply* reply = (redisReply*)redisCommand(_Con, "SET %s %s", key.c_str(), value.c_str());
                if (nullptr == reply) {
                    ErrorLog(g_Engine, "redis set %s error : %s, code %d", key.c_str(), _Con->errstr, _Con->err);
                    return _Con->err;
                }

                Int64 code = -1;
                switch (reply->type) {
                case REDIS_REPLY_STATUS:
                    if (strcmp(reply->str, "OK") == 0) {
                        code = REDIS_CODE_SUCCESS;
                    }
                    break;
                case REDIS_REPLY_ERROR:
                    ErrorLog(g_Engine, "SET error: %s, code %d", reply->str, _Con->err);
                    code = -1;
                    break;

                default:
                    ErrorLog(g_Engine, "SET returned unexpected type: %d", reply->type);
                    code = -1;
                    break;
                }

                freeReplyObject(reply);
                return code;
            },
            [=](Int64 code, const Api::iTaskContext& context) {
                callback(REDIS_CODE_SUCCESS == code);
            },
            _Index
        );
    }

    void RedisSession::AsyncSetWhenNotExists(const std::string& key, const std::string& value, const Api::fBoolResult& callback) {
        if (_IsClosed) {
            callback(false);
            return;
        }

        g_Engine->GetTaskApi()->PushTask(Api::eTaskProcessorType::OrderedQueue,
            [=](Api::iTaskContext& context) {
                AUTO_LOCK(_Lock);

                const char* script =
                    "if redis.call('EXISTS', KEYS[1]) == 0 then "
                    "redis.call('SET', KEYS[1], ARGV[1]) "
                    "return 1 "
                    "else "
                    "return 0 "
                    "end";

                redisReply* reply = (redisReply*)redisCommand(
                    _Con,
                    "EVAL %s 1 %s %s",
                    script,
                    key.c_str(),
                    value.c_str()
                );

                if (nullptr == reply) {
                    ErrorLog(g_Engine, "AsyncSetWhenNotExists %s error : %s, code %d", key.c_str(), _Con->errstr, _Con->err);
                    return _Con->err;
                }
                switch (reply->type) {
                case REDIS_REPLY_INTEGER:
                    context.Set(reply);
                    return REDIS_CODE_SUCCESS;
                case REDIS_REPLY_ERROR:
                    ErrorLog(g_Engine, "AsyncSetWhenNotExists %s error : %s", key.c_str(), reply->str);
                    freeReplyObject(reply);
                    return -1;
                default:
                    ErrorLog(g_Engine, "AsyncSetWhenNotExists GET returned unexpected type: %d", reply->type);
                    freeReplyObject(reply);
                    return -1;
                }

                return REDIS_CODE_SUCCESS;
            },
            [=](Int64 code, const Api::iTaskContext& context) {
                if (code == REDIS_CODE_SUCCESS) {
                    redisReply* reply = context.Get<redisReply>();

                    switch (reply->type) {
                    case REDIS_REPLY_INTEGER:
                        callback(reply->integer == 1);
                        break;
                    case REDIS_REPLY_NIL:
                        callback(false);
                        break;
                    }

                    freeReplyObject(reply);
                }
                else {
                    callback(false);
                }
            },
            _Index
        );
    }

    void RedisSession::AsyncCAS(const std::string& key, const std::string& compare, const std::string& value, const Api::fBoolResult& callback) {
        if (_IsClosed) {
            callback(false);
            return;
        }

        g_Engine->GetTaskApi()->PushTask(Api::eTaskProcessorType::OrderedQueue,
            [=](Api::iTaskContext& context) {
                AUTO_LOCK(_Lock);
                const char* script =
                    "if redis.call('GET', KEYS[1]) == ARGV[1] then "
                    "redis.call('SET', KEYS[1], ARGV[2]) "
                    "return 1 "
                    "else "
                    "return 0 "
                    "end";

                redisReply* reply = (redisReply*)redisCommand(
                    _Con,
                    "EVAL %s 1 %s %s %s",
                    script,
                    key.c_str(),
                    compare.c_str(),
                    value.c_str()
                );

                if (nullptr == reply) {
                    ErrorLog(g_Engine, "AsyncCAS %s error : %s, code %d", key.c_str(), _Con->errstr, _Con->err);
                    return _Con->err;
                }
                switch (reply->type) {
                case REDIS_REPLY_INTEGER:
                    context.Set(reply);
                    return REDIS_CODE_SUCCESS;
                case REDIS_REPLY_ERROR:
                    ErrorLog(g_Engine, "AsyncCAS %s error : %s", key.c_str(), reply->str);
                    freeReplyObject(reply);
                    return -1;
                default:
                    ErrorLog(g_Engine, "AsyncCAS GET returned unexpected type: %d", reply->type);
                    freeReplyObject(reply);
                    return -1;
                }

                return REDIS_CODE_SUCCESS;
            },
            [=](Int64 code, const Api::iTaskContext& context) {
                if (code == REDIS_CODE_SUCCESS) {
                    redisReply* reply = context.Get<redisReply>();

                    switch (reply->type) {
                    case REDIS_REPLY_INTEGER:
                        callback(reply->integer == 1);
                        break;
                    case REDIS_REPLY_NIL:
                        callback(false);
                        break;
                    }

                    freeReplyObject(reply);
                }
                else {
                    callback(false);
                }
            },
            _Index
        );
    }

    void RedisSession::Close() {
        _IsClosed = true;
        if (_CurrentOptCount == 0) {
            redisFree(_Con);
            txdel this;
            DEBUG(g_Engine, "RedisSession %lld Released", _Index);
        }
    }
}
