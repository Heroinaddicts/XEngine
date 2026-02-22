#ifndef __IGame_h__
#define __IGame_h__

#include "iModule.h"
#include <functional>
#include "google/protobuf/message.h"
UsingXEngine;

class IGame : public Api::iModule {
public:
    virtual ~IGame() {}

    enum class eGameEvent {
        AccountConnect = 0,
        AccountDisconnect = 1,

        __Count__
    };

    typedef void(*fGameEvent)(const UInt64 account);

    virtual void RegistGameEvent(const eGameEvent ev, const fGameEvent callback) = 0;

    void SendProtobufToClient(const UInt64 account, const UInt16 messageId, const ::google::protobuf::Message& pb) const {
        const int size = pb.ByteSize();
        void* temp = alloca(size);
        if (pb.SerializePartialToArray(temp, size)) {
            SendMessageToClient(account, messageId, temp, size);
        }
        else {
            XASSERT(false, "wtf");
        }
    }


    void Broadcast(const std::vector<UInt64>& list, const UInt16 messageId, const ::google::protobuf::Message& pb) const {
        const int size = pb.ByteSize();
        void* temp = alloca(size);
        if (pb.SerializePartialToArray(temp, size)) {
            Broadcast(list, messageId, temp, size);
        }
        else {
            XASSERT(false, "wtf");
        }
    }

    virtual void SendMessageToClient(const UInt64 account, const UInt16 messageId, const void* const data, const int size) const = 0;
    virtual void Broadcast(const std::vector<UInt64>& list, const UInt16 messageId, const void* const data, const int size) const = 0;

    typedef std::function<void(const UInt64 account, const void* const data, const int size)> fClientMessageCall;
    virtual void RegistClientMessage(const UInt16 messageId, const fClientMessageCall call) = 0;

    typedef std::function<void(bool succeed)> fSetNameCallback;
    virtual void PickName(const std::string& name, const fSetNameCallback& callback) = 0;
};

#endif //__IGame_h__
