#ifndef __IPlayerManager_h__
#define __IPlayerManager_h__

#include "iModule.h"
#include "google/protobuf/message.h"
#include <functional>
#include "Vector3.h"
UsingXEngine;

#include "IGameWorld.h"

class IPlayer {
public:
    virtual ~IPlayer() {}

    virtual UInt64 AccountID() const = 0;
    virtual const std::string Nickname() const = 0;
    virtual UInt32 Gold() const = 0;
    virtual UInt32 Diamond() const = 0;
    virtual const Vector3& Position() const = 0;

    virtual void SetGold(const UInt32 gold) = 0;
    virtual void SetDiamond(const UInt32 diamond) = 0;
    virtual void SetPosition(const float x, const float y, const float z) = 0;

    virtual  UInt64 GetDeployCharacterID() const = 0;

    void SendProtobuf(const UInt16 messageId, const ::google::protobuf::Message& pb) const {
        const int size = pb.ByteSize();
        void* temp = alloca(size);
        if (pb.SerializePartialToArray(temp, size)) {
            SendMessage(messageId, temp, size);
        }
        else {
            XASSERT(false, "wtf");
        }
    }

    virtual void SendMessage(const UInt16 messageId, const void* data, const int size) const = 0;
};

class IPlayerManager : public Api::iModule {
public:
    virtual ~IPlayerManager() {}

    virtual IPlayer* QueryPlayer(const UInt64 account) const = 0;

    enum ePlayerEvent {
        NewPlayerCreated = 0,
        PlayerLoading = 1,
        PlayerLoaded = 2,
        PlayerRelease = 3
    };

    typedef void(*fPlayerEvent)(IPlayer* const player, const UInt64 account);
    virtual void RegistPlayerEvent(const ePlayerEvent ev, const fPlayerEvent fun) = 0;

    template<typename PB>
    void RegistPlayerProtobuf(const UInt16 messageId, void(*callback)(IPlayer* const, const UInt64 account, const PB& pb)) {
        RegistPlayerMessage(messageId,
            [=](IPlayer* const player, const UInt64 account, const void* data, const int size) {
                PB temp;
                if (temp.ParseFromArray(data, size)) {
                    callback(player, account, temp);
                }
                else {
                    XASSERT(false, "RegistPlayerProtobuf ParseFromArray id %d", messageId);
                }
            }
        );
    }
    void SendProtobufToPlayer(const UInt64 account, const UInt16 messageId, const ::google::protobuf::Message& pb) const {
        const int size = pb.ByteSize();
        void* temp = alloca(size);
        if (pb.SerializePartialToArray(temp, size)) {
            SendMessageToPlayer(account, messageId, temp, size);
        }
        else {
            XASSERT(false, "wtf");
        }
    }

    void Broadcast(std::vector<UInt64>& list, const UInt16 messageId, const ::google::protobuf::Message& pb) const {
        const int size = pb.ByteSize();
        void* temp = alloca(size);
        if (pb.SerializePartialToArray(temp, size)) {
            Broadcast(list, messageId, temp, size);
        }
        else {
            XASSERT(false, "wtf");
        }
    }

    virtual void SendMessageToPlayer(const UInt64 account, const UInt16 messageId, const void* data, const int size) const = 0;
    virtual void Broadcast(std::vector<UInt64>& list, const UInt16 messageId, const void* const data, const int size) const = 0;
protected:
    typedef std::function<void(IPlayer* const, const UInt64 account, const void* data, const int size)> fPlayerMessageCall;
    virtual void RegistPlayerMessage(const UInt16 messageId, const fPlayerMessageCall fun) = 0;

};

#endif //__IPlayerManager_h__
