#ifndef __Redis_h__
#define __Redis_h__ 

#include "Header.h"

#define QUEUE_SIZE 1024

class Redis : public iRedis, public XEngine::SafeThread {
public:
    virtual ~Redis() {}

    Redis() : _RequestQueue(QUEUE_SIZE), _ResponseQueue(QUEUE_SIZE) {}

    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);

    virtual void Update(iEngine* const engine);

    virtual void Write(const std::string& key, const void* data, const int len, const unsigned_int64 context, const fRedisResult fun);
    virtual void Read(const std::string& key, const unsigned_int64 context, const fRedisResult fun);
protected:
    // Í¨¹ý SafeThread ¼Ì³Ð
    virtual void Run(void* constext) override;

private:
    XEngine::SafeQueue::SpscQueue<RedisOpt> _RequestQueue, _ResponseQueue;
};

#endif //__Redis_h__
