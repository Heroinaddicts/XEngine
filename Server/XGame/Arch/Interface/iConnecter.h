#ifndef __iConnecter_h__
#define __iConnecter_h__

/*
* Alpha Test 之前 都用简单架构 怎么快怎么来 所以暂时摒弃Master管理那套机制 改为Connecter模块来控制 架构中各个节点之间的链接关系(需要运维手动配置)
*/

#include "iModule.h"
#include "iNetApi.h"
#include <google/protobuf/message.h>
using namespace XEngine;
enum eRouteEvent;

class iNodeSession : public Api::iTcpSession {
public:
    typedef void(*fSessionMessage)(iNodeSession* const session, const int msgid, const void* data, const int len);

protected:
    class iMessageHandle {
    public:
        virtual ~iMessageHandle() {}
        iMessageHandle(const unsigned_int16 msgid, const char* file, const int line)
            : _MsgID(msgid), _File(file), _Line(line) {}

        virtual void OnMessage(iNodeSession* const session, const int msgid, const void* data, const int len) const = 0;

        const unsigned_int16 _MsgID;
        const std::string _File;
        const int _Line;
    };

    template<typename T>
    class tPbMessager : public iMessageHandle {
    public:
        typedef void(*fPbMessage)(iNodeSession* const session, const int msgid, const T& body);
    public:
        virtual ~tPbMessager() {}
        tPbMessager(const fPbMessage fun, const unsigned_int16 msgid, const char* file, const int line) : iMessageHandle(msgid, file, line), _Function(fun) {}

        virtual void OnMessage(iNodeSession* const session, const int msgid, const void* data, const int len) const {
            T body;
            if (body.ParseFromArray(data, len)) {
                fPbMessage(session, msgid, body);
            }
            else {
                XASSERT(false, "pb parse error %s:%d", _File.c_str(), _Line);
            }
        }

        const fPbMessage _Function;
    };

public:
    virtual ~iNodeSession() {}

    virtual int GetId() const = 0;
    virtual const std::string& GetName() const = 0;
    virtual const std::string& GetRemoteIP() const = 0;
    virtual int GetRemotePort() const = 0;

    template<typename T>
    __forceinline void RegisterMessage(const unsigned_int16 msgid, const void(*fun)(iNodeSession* const session, const int msgid, const T& body), const char* file, const int line) {
        tPbMessager<T>* msger = xnew tPbMessager<T>(msgid, fun, file, line);
        Register(msgid, msger);
    }

    virtual void SendMessage(const unsigned_int16 msgid, const void* data, const unsigned_int16 len) = 0; //架构内 单挑协议 协议体长度 不可以大于 max(unsigned_int16) - 协议头长度
protected:
    virtual void Register(const unsigned_int16 msgid, const iMessageHandle* handle) = 0;
};


struct RouteInfo {
    std::map<std::string, unsigned_int32> _Route;
};

class iConnecter : public Api::iModule {
public:
    typedef void(*fSessionEvent)(iNodeSession* const session);
    typedef void(*fRouteEvent)(iNodeSession* const session, const eRouteEvent ev, const unsigned_int64 guid);

    enum class eConnectionEvent {
        NodeAppear,
        NodeDisappear
    };

public:
    virtual ~iConnecter() {}

    virtual void RouteReport(const std::string& nodeName, const eRouteEvent ev, const unsigned_int64 guid) const = 0;
    virtual void RouteReport(const unsigned_int32 nodeId, const eRouteEvent ev, const unsigned_int64 guid) const = 0;

    virtual const RouteInfo* GetRouteInfo(const unsigned_int64 guid) const = 0;

    virtual void RegisterRouteEvent(fRouteEvent const fun) = 0;

    virtual void RegisterSessionEvent(const eConnectionEvent& ev, fSessionEvent const fun) = 0;
    virtual void UnregisterSessionEvent(const eConnectionEvent& ev, fSessionEvent const fun) = 0;

    virtual iNodeSession* QueryNodeSession(const int nodeId) const = 0;
};

#endif //__iConnecter_h__
