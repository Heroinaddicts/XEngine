#ifndef __iConnecter_h__
#define __iConnecter_h__

/*
* Alpha Test ֮ǰ ���ü򵥼ܹ� ��ô����ô�� ������ʱ����Master�������׻��� ��ΪConnecterģ�������� �ܹ��и����ڵ�֮������ӹ�ϵ(��Ҫ��ά�ֶ�����)
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
    virtual void SendMessage(const unsigned_int16 msgid, const void* data, const unsigned_int16 len) = 0; //�ܹ��� ����Э�� Э���峤�� �����Դ��� max(unsigned_int16) - Э��ͷ����
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
