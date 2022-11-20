#ifndef __iRedis_h__
#define __iRedis_h__

#include "iModule.h"
using namespace XEngine;

class iRedisPipe {
public:
    virtual ~iRedisPipe() {}

    virtual void Write(const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context) = 0;
    virtual void Read(const std::string& key, const int logicId, const unsigned_int64 context) = 0;
    virtual void Delete(const std::string& key, const int logicId, const unsigned_int64 context) = 0;

    virtual void Close() = 0;
};

class iRedisSession {
public:
    virtual ~iRedisSession() {}
    iRedisSession() : _Pipe(nullptr) {}

    virtual void OnConnect(bool success) = 0;
    virtual void OnDisconnect() = 0;

    virtual void OnWriteResponse(bool success, const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context) = 0;
    virtual void OnReadResponse(bool success, const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context) = 0;
    virtual void OnDeleteResponse(bool success, const std::string& key, const int logicId, const unsigned_int64 context) = 0;

    void Write(const std::string& key, const int logicId, const void* data, const int len, const unsigned_int64 context) {
        _Pipe ? _Pipe->Write(key, logicId, data, len, context) : void(0);
    }

    void Read(const std::string& key, const int logicId, const unsigned_int64 context) {
        _Pipe ? _Pipe->Read(key, logicId, context) : void(0);
    }

    void Delete(const std::string& key, const int logicId, const unsigned_int64 context) {
        _Pipe ? _Pipe->Delete(key, logicId, context) : void(0);
    }

    void Close() {
        _Pipe ? _Pipe->Close() : void(0);
    }

    iRedisPipe* const _Pipe;
};

class iRedis : public Api::iModule {
public:
    virtual ~iRedis() {}

    virtual void ConnectRedis(iRedisSession* session, const std::string& host, const int port, const std::string& password = "") = 0;
};

#endif //__iRedis_h__
