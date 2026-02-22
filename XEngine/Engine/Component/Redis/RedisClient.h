#ifndef __RedisClient_h__
#define __RedisClient_h__

#include "Header.h"

namespace XEngine {
    class RedisClient : public iRedis {
    public:
        virtual ~RedisClient() {}

        static RedisClient* GetInstace();

        // Í¨¹ý iRedis ¼Ì³Ð
        bool Initialize(Api::iEngine* const engine) override;
        bool Launch(Api::iEngine* const engine) override;
        void Release(Api::iEngine* const engine) override;
        void EarlyUpdate(Api::iEngine* const engine) override;
        void Update(Api::iEngine* const engine) override;
        void LaterUpdate(Api::iEngine* const engine) override;
        void ConnectRedis(const std::string& host, const int port, const std::string& password, Api::fConnectRedisCallback callback) override;
    };
}

#endif //__RedisClient_h__
