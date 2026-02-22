#ifndef __IGate_h__
#define __IGate_h__

#include "iModule.h"
#include "iNetApi.h"
#include "FrameworkSession.h"
UsingXEngine;

#define INVALID_ACCOUNT 0xFFFFFFFFFFFFFFFF

class IClient : public Api::iTcpSession {
public:
    virtual ~IClient() {}

    virtual UInt64 ClientID() const = 0;
    virtual UInt64 Account() const = 0;
    virtual bool IsLogined() const = 0;

    void SendProtobuf(const UInt16 id, const ::google::protobuf::Message& body) {
        const int size = body.ByteSize();
        void* temp = alloca(size);
        if (body.SerializePartialToArray(temp, size)) {
            SendMessage(id, temp, size);
        }
    }

    void SendMessage(const UInt16 id, const void* const data, const int size) {
        MessageHead head;
        head._Len = sizeof(head) + size;
        head._ID = id;
        Send(&head, sizeof(head), false);
        Send(data, size);
    }
};

class IGate : public Api::iModule {
public:
    virtual ~IGate() {}

    enum class eGateEvents {
        ClientConnected = 0,
        ClientLogind = 1,
        ClientLogout = 2,
        ClientDsiconnected = 3,

        __Count__
    };

    typedef void(*fGateEvent)(IClient* const);

    virtual void RegistGateEvent(const eGateEvents ev, const fGateEvent callback) = 0;

    virtual IClient* QueryClient(const UInt64 clinetId) const = 0;
    typedef std::function<void(IClient* const client, const UInt64 clientId, const void* const data, const int size)> fClientMessager;
    virtual void RegistClientMessage(const UInt16 messageId, const fClientMessager callback) = 0;
};

#endif //__IGate_h__
