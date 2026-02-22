#ifndef __ICacheClient_h__
#define __ICacheClient_h__

#include "iModule.h"
#include "ISQLCache.h"
#include <functional>
UsingXEngine;
UsingSQLCacheTemplate;

#define CacheClientCreateRow(client, tab) client->CreateCacheRow(tab, __FILE__, __LINE__)
#define CacheClientReleaseRow(client, row) client->ReleaseCacheRow(row, file, line)

class ICacheClient : public Api::iModule {
public:
    virtual ~ICacheClient() {}
    typedef std::function<void(const bool succeed, const ISQLCacheTable* const tab)> fResult;


    typedef std::function<void(ISQLCacheRow* const row)> fInitializeCache;

    virtual void Create(const UInt64 account, const eTable tab, const fInitializeCache& init, const fResult& ret = nullptr) = 0;

    void  Query(const UInt64 account, const eTable tab, const int conditionColumn, const Int32 conditionValue, const fResult& ret) {
        this->Query(account, tab, conditionColumn, SafeString::Int32ToString(conditionValue), ret);
    }

    void  Query(const UInt64 account, const eTable tab, const int conditionColumn, const UInt32 conditionValue, const fResult& ret) {
        this->Query(account, tab, conditionColumn, SafeString::UInt32ToString(conditionValue), ret);
    }

    void  Query(const UInt64 account, const eTable tab, const int conditionColumn, const Int64 conditionValue, const fResult& ret) {
        this->Query(account, tab, conditionColumn, SafeString::Int64ToString(conditionValue), ret);
    }

    void  Query(const UInt64 account, const eTable tab, const int conditionColumn, const UInt64 conditionValue, const fResult& ret) {
        this->Query(account, tab, conditionColumn, SafeString::UInt64ToString(conditionValue), ret);
    }

    virtual void Query(const UInt64 account, const eTable tab, const int conditionColumn, const std::string& conditionValue, const fResult& ret) = 0;

    void Update(const UInt64 account, const eTable tab, const int conditionColumn, const UInt64 conditionValue, const fInitializeCache& init, const fResult& ret) {
        this->Update(account, tab, conditionColumn, SafeString::UInt64ToString(conditionValue), init, ret);
    }

    virtual void Update(const UInt64 account, const eTable tab, const int conditionColumn, const std::string& conditionValue, const fInitializeCache& init, const fResult& ret) = 0;

    void Delete(const UInt64 account, const eTable tab, const int conditionColumn, const UInt64 conditionconditionValue, const fResult& ret) {
        this->Delete(account, tab, conditionColumn, SafeString::UInt64ToString(conditionconditionValue), ret);
    }
    virtual void Delete(const UInt64 account, const eTable tab, const int conditionColumn, const std::string& conditionconditionValue, const fResult& ret) = 0;


    virtual void ReleaseCache(const UInt64 account) = 0;
};

#endif //__ICacheClient_h__
