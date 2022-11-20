#ifndef __Redis_h__
#define __Redis_h__ 

#include "Header.h"

#define QUEUE_SIZE 64
class Redis : public iRedis, public SafeThread {
public:
    virtual ~Redis() {}

    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);
    virtual void Update(iEngine* const engine);

    virtual void ConnectRedis(iRedisSession* session, const std::string& host, const int port, const std::string& password);

protected:
    virtual void Run(void* constext);

private:
    XEngine::SafeQueue::SpscQueue<RedisPipe*, QUEUE_SIZE> _PipeConnectRequestQueue;
    XEngine::SafeQueue::SpscQueue<RedisPipe*, QUEUE_SIZE> _PipeConnectResponseQueue;

};

#endif //__Redis_h__
