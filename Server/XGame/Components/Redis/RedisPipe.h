#ifndef __RedisPipe_h__
#define __RedisPipe_h__

#include "Header.h"
class RedisOperator;
class RedisPipe : public iRedisPipe, public SafeThread {
public:
    enum class eErrorCode {
        SUCCESS,
        CONNECT_FAILD,
        AUTH_FAILD,
        DISCONNECTED
    };

public:
    virtual ~RedisPipe() {}

    RedisPipe(iRedisSession* session, const std::string& host, const int port, const std::string& password)
        : _Session(session), _Host(host), _Port(port), _RedisContext(nullptr), _IsConnected(false), _Password(password) {}

    virtual void Write(const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context);
    virtual void Read(const std::string& key, const int logicId, const unsigned_int64 context);
    virtual void Delete(const std::string& key, const int logicId, const unsigned_int64 context);

    virtual void Close();

    __forceinline bool IsConnected() { return _IsConnected; }
    __forceinline eErrorCode GetCode() { return _Code; }
    void Update();
    void Connect();

protected:
    // Í¨¹ý SafeThread ¼Ì³Ð
    virtual void Run(void* constext) override;

private:
    void Response(RedisOperator* opt);

public:
    const std::string _Host;
    const int _Port;
    const std::string _Password;
    iRedisSession* const _Session;
    redisContext* _RedisContext;

    eErrorCode _Code;
    bool _IsConnected;

private:
    SafeQueue::SpscQueue<RedisOperator*> _RedisOperatorRequestQueue;
    SafeQueue::SpscQueue<RedisOperator*> _RedisOperatorResponseQueue;

    SpinLock _Lock;
};

#endif //__RedisPipe_h__
