#ifndef __iConnecter_h__
#define __iConnecter_h__

/*
* Alpha Test 之前 都用简单架构 怎么快怎么来 所以暂时摒弃Master管理那套机制 改为Connecter模块来控制 架构中各个节点之间的链接关系(需要运维手动配置)
*/

#include "iModule.h"
#include "iNetApi.h"
using namespace XEngine;

class iNodeSession : public Api::iTcpSession {
public:
    typedef void(*fSessionMessage)(iNodeSession* const session, const int msgid, const void* data, const int len);

public:
    virtual ~iNodeSession() {}

    virtual int GetId() const = 0;
    virtual const std::string& GetName() const = 0;
    virtual const std::string& GetRemoteIP() const = 0;
    virtual int GetRemotePort() const = 0;

    virtual void RegisterMessage(const unsigned_int16 msgid, const fSessionMessage fun) = 0;
    virtual void SendMessage(const unsigned_int16 msgid, const void* data, const unsigned_int16 len) = 0; //架构内 单挑协议 协议体长度 不可以大于 max(unsigned_int16) - 协议头长度
};

class iConnecter : public Api::iModule {
public:
    typedef void(*fSessionEvent)(iNodeSession* const session);

    enum class eConnectionEvent {
        NodeAppear,
        NodeDisappear
    };

public:
    virtual ~iConnecter() {}

    virtual void RegisterSessionEvent(const eConnectionEvent& ev, fSessionEvent const fun) = 0;
    virtual void UnregisterSessionEvent(const eConnectionEvent& ev, fSessionEvent const fun) = 0;

    virtual iNodeSession* QueryNodeSession(const int nodeId) const = 0;
};

#endif //__iConnecter_h__
