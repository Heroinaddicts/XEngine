#ifndef __iRedisApi_h__
#define __iRedisApi_h__

#include "MultiSys.h"

namespace XEngine {
    namespace Api {
        using StringList = std::vector<std::string>;

        using fBoolResult = std::function<void(bool)>;
        using fStringResult = std::function<void(bool, const char*)>;
        using fIntResult = std::function<void(bool, int64_t)>;
        using fVectorResult = std::function<void(bool, const StringList&)>;

        class iRedisSession;
        using fConnectRedisCallback = std::function<void(iRedisSession* const)>;

#define REDIS_CODE_SUCCESS 0

        class iRedisSession {
        public:
            virtual ~iRedisSession() {}

            virtual void AsyncGet(const std::string& key, const fStringResult& callback) = 0;
            virtual void AsyncSet(const std::string& key, const std::string& value, const fBoolResult& callback) = 0;

            virtual void AsyncSetWhenNotExists(const std::string& key, const std::string& value, const fBoolResult& callback) = 0;
            virtual void AsyncCAS(const std::string& key, const std::string& compare, const std::string& value, const fBoolResult& callback) = 0;

            virtual void Close() = 0;
        };

        class iRedisApi {
        public:
            virtual ~iRedisApi() {}
            virtual void ConnectRedis(const std::string& host, const int port, const std::string& password, fConnectRedisCallback callback) = 0;
        };
    }
}

#endif //__iRedisApi_h__
