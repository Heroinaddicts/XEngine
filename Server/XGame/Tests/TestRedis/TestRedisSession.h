#ifndef __TestRedisSession_h__
#define __TestRedisSession_h__

#include "Header.h"

class TestRedisSession : public iRedisSession, public Api::iTimer {
public:
    virtual ~TestRedisSession() {}

    TestRedisSession() : _ID(g_RedisSessionID++), _Index(0), _GiveUP(false) {}

    // 通过 iRedisSession 继承
    virtual void OnConnect(bool success) override;
    virtual void OnDisconnect() override;
    virtual void OnWriteResponse(bool success, const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context) override;
    virtual void OnReadResponse(bool success, const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context) override;

    // 通过 iRedisSession 继承
    virtual void OnDeleteResponse(bool success, const std::string& key, const int logicId, const unsigned_int64 context) override;

    virtual void OnStart(const int id, void* const context, const int64 timestamp) override;
    virtual void OnTimer(const int id, void* const context, const int64 timestamp) override;
    virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) override;

    virtual void OnPause(const int id, void* const context, const int64 timestamp) override;
    virtual void OnResume(const int id, void* const context, const int64 timestamp) override;

    __forceinline void GiveUP() {
        _GiveUP = true;
        Close();
    }
private:
    int _ID;
    int _Index;
    bool _GiveUP;
};

#endif //__TestRedisSession_h__
