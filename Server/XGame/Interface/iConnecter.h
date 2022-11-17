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
    virtual ~iNodeSession() {}

    virtual int GetId() const = 0;
    virtual const char* GetName() const = 0;
    virtual const char* GetRemoteIP() const = 0;
    virtual int GetRemotePort() const = 0;
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
};

#endif //__iConnecter_h__
