#ifndef __CacheClient_h__
#define __CacheClient_h__

#include "Header.h"

class CacheClient : public ICacheClient, Api::iTimer {
public:
    virtual ~CacheClient() {}

    // 通过 ICacheClient 继承
    void Create(const UInt64 account, const eTable tab, const fInitializeCache& init, const fResult& ret) override;
    void Query(const UInt64 account, const eTable tab, const int conditionColumn, const std::string& conditionValue, const fResult& ret) override;
    void Update(const UInt64 account, const eTable tab, const int conditionColumn, const std::string& conditionValue, const fInitializeCache& init, const fResult& ret) override;
    void Delete(const UInt64 account, const eTable tab, const int conditionColumn, const std::string& conditionValue, const fResult& ret) override;

    void ReleaseCache(const UInt64 account) override;

private:
    // 通过 ICacheClient 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;
private:
    static FrameworkSession* CreateCacheServerSession();
    static void ReleaseCacheServerSession(FrameworkSession* session);

    static void OnCacheServerConnected(FrameworkSession* const session);
    static void OnCacheServerDisconnected(FrameworkSession* const session);
    static void OnCacheServerConnectFaild(FrameworkSession* const session);

    static void OnCacheAnswer(IFrameworkSession* const session, const UInt64 sessionId, const oCacheAnswer& answer);
};

#endif //__CacheClient_h__
