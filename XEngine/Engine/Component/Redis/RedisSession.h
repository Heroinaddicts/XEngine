#ifndef __RedisSession_h__
#define __RedisSession_h__

#include "Header.h"

namespace XEngine {
    class RedisSession : public Api::iRedisSession {
    public:
        virtual ~RedisSession() {}

        RedisSession(redisContext* con) : _Con(con), _Index(s_IndexOffset++), _CurrentOptCount(0), _IsClosed(false) {}

        void AsyncGet(const std::string& key, const Api::fStringResult& callback) override;
        void AsyncSet(const std::string& key, const std::string& value, const Api::fBoolResult& callback) override;

        void AsyncSetWhenNotExists(const std::string& key, const std::string& value, const Api::fBoolResult& callback) override;
        void AsyncCAS(const std::string& key, const std::string& compare, const std::string& value, const Api::fBoolResult& callback) override;

        void Close() override;
    private:
        SpinLock _Lock;
        int _CurrentOptCount;
    private:
        redisContext* const _Con;
        const Int64 _Index;
        static Int64 s_IndexOffset;

        bool _IsClosed;
    };
}

#endif //__RedisSession_h__
