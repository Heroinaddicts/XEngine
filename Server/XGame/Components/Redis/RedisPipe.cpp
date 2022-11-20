#include "RedisPipe.h"
#include "RedisOperator.h"

#define SAFE_FREE_REDIS_CONTEXT(rc)\
    redisFree(rc); \
    rc = nullptr;

void RedisPipe::Write(const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context) {
    XASSERT(data != nullptr && len > 0, "wtf");
    if (data != nullptr && len > 0) {
        RedisOperator* opt = RedisOperator::Create(key, logicId, RedisOperator::eOperatorType::Write, context);
        opt->_Data = xmalloc(len);
        SafeMemory::Memcpy(opt->_Data, len, data, len);
        opt->_Len = len;

        _RedisOperatorRequestQueue.Push(opt);
    }
    else {
        ERROR(g_Engine, "RedisPipe::Write a null data");
    }
}

void RedisPipe::Read(const std::string& key, const int logicId, const unsigned_int64 context) {
    RedisOperator* opt = RedisOperator::Create(key, logicId, RedisOperator::eOperatorType::Read, context);
    _RedisOperatorRequestQueue.Push(opt);
}

void RedisPipe::Delete(const std::string& key, const int logicId, const unsigned_int64 context) {
    RedisOperator* opt = RedisOperator::Create(key, logicId, RedisOperator::eOperatorType::Delete, context);
    _RedisOperatorRequestQueue.Push(opt);
}

void RedisPipe::Close() {
    SafeMemory::Memset((void*)&(_Session->_Pipe), sizeof(_Session->_Pipe), 0, sizeof(_Session->_Pipe));
    //to do
    {
        AUTO_LOCK(_Lock);
        if (_RedisContext) {
            SAFE_FREE_REDIS_CONTEXT(_RedisContext);
        }

        RedisOperator* opt;
        while (_RedisOperatorResponseQueue.Pull(opt)) {
            Response(opt);
            RedisOperator::Release(opt);
        }

        while (_RedisOperatorRequestQueue.Pull(opt)) {
            RedisOperator::Release(opt);
        }
    }
    _Code = eErrorCode::DISCONNECTED;
    _IsConnected = false;
    _Session->OnDisconnect();
    SafeThread::Close();
}

void RedisPipe::Response(RedisOperator* opt) {
    switch (opt->_Type) {
    case RedisOperator::eOperatorType::Write: {
        if (_Session) {
            _Session->OnWriteResponse(opt->_Success, opt->_Key, opt->_LogicID, opt->_Data, opt->_Len, opt->_Context);
        }
        break;
    }
    case RedisOperator::eOperatorType::Read: {
        if (_Session) {
            _Session->OnReadResponse(opt->_Success, opt->_Key, opt->_LogicID, opt->_Data, opt->_Len, opt->_Context);
        }
        break;
    }
    case RedisOperator::eOperatorType::Delete: {
        if (_Session) {
            _Session->OnDeleteResponse(opt->_Success, opt->_Key, opt->_LogicID, opt->_Context);
        }
        break;
    }
    default:
        break;
    }
}

void RedisPipe::Update() {
    RedisOperator* opt = nullptr;
    if (_RedisOperatorResponseQueue.Pull(opt)) {
        Response(opt);

        if (!opt->_Success) {
            if (_RedisContext) {
                if (_RedisContext->err != 0) {
                    AUTO_LOCK(_Lock);
                    Close();
                }
            }
        }
        RedisOperator::Release(opt);
    }
}

void RedisPipe::Connect() {
    XASSERT(_RedisContext == nullptr, "this redis pipe already connected");

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    _RedisContext = redisConnectWithTimeout(_Host.c_str(), _Port, tv);
    if (_RedisContext) {
        if (_RedisContext->err != REDIS_OK) {
            SAFE_FREE_REDIS_CONTEXT(_RedisContext);
            _IsConnected = false;
            _Code = eErrorCode::CONNECT_FAILD;
        }
        else {
            bool b = true;
            if (_Password.length() > 0) {
                redisReply* rply = (redisReply*)redisCommand(_RedisContext, "auth %s", _Password.c_str());
                b = (rply && rply->type == REDIS_REPLY_STATUS && SafeString::Compare(rply->str, "OK"));
                if (rply) {
                    freeReplyObject(rply);
                    rply = nullptr;
                }
            }

            if (!b) {
                SAFE_FREE_REDIS_CONTEXT(_RedisContext);
                _Code = eErrorCode::AUTH_FAILD;
                _IsConnected = false;
            }
            else {
                _Code = eErrorCode::SUCCESS;
                _IsConnected = true;
            }
        }
    }
}

void RedisPipe::Run(void* constext) {
    while (!IsClose()) {
        RedisOperator* opt;
        if (_RedisOperatorRequestQueue.Pull(opt)) {
            AUTO_LOCK(_Lock);
            if (nullptr == _RedisContext) {
                SafeThread::Close();
                return;
            }

            redisReply* rply = nullptr;
            switch (opt->_Type) {
            case RedisOperator::eOperatorType::Write: {
                XASSERT(opt->_Data, "redis session write a nullptr data");
                rply = (redisReply*)redisCommand(_RedisContext, "set %s %b", opt->_Key.c_str(), opt->_Data, (size_t)opt->_Len);
                opt->_Success = (rply && rply->type == REDIS_REPLY_STATUS && SafeString::Compare(rply->str, "OK"));
                break;
            }
            case RedisOperator::eOperatorType::Read: {
                rply = (redisReply*)redisCommand(_RedisContext, "get %s", opt->_Key.c_str());
                opt->_Success = (rply && (rply->type == REDIS_REPLY_STRING || rply->type == REDIS_REPLY_NIL));
                if (opt->_Success) {
                    opt->_Data = xmalloc(rply->len);
                    SafeMemory::Memcpy(opt->_Data, rply->len, rply->str, rply->len);
                }
                break;
            }
            case RedisOperator::eOperatorType::Delete: {
                rply = (redisReply*)redisCommand(_RedisContext, "del %s", opt->_Key.c_str());
                opt->_Success = (rply && rply->type == REDIS_REPLY_INTEGER && rply->integer == 1);
                break;
            }
            default:
                XASSERT(false, "wtf");
                break;
            }

            if (rply) {
                freeReplyObject(rply);
            }
            _RedisOperatorResponseQueue.Push(opt);
        }
        else {
            SafeSystem::Time::MillisecondSleep(1);
        }
    }
}
