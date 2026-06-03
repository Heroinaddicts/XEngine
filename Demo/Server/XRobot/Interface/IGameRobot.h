#ifndef __IGameRobot_h__
#define __IGameRobot_h__

#include "iModule.h"
#include "iNetApi.h"
#include "FrameworkSession.h"
#include "google/protobuf/message.h"
#include "Game.pb.h"
#include "Vector2.h"
UsingXEngine;

#define INVALID_ROBOT_ID 0xFFFFFFFFFFFFFFFF

class IRobotSession : public Api::iTcpSession {
public:
    virtual ~IRobotSession() {}

    virtual UInt64 SessionID() const = 0;

    virtual UInt64 Account() const = 0;
    virtual UInt64 DeployCharacterID() const = 0;

    virtual const Vector2& Position() const = 0;
    virtual void SetPosition(const Vector2& pos) = 0;
    __forceinline void SetPosition(const float x, const float y) {
        SetPosition(Vector2(x, y));
    }

    virtual int Angle() const = 0;
    virtual void SetAngle(const int angle) = 0;

    void SendProtobuf(const UInt16 id, const ::google::protobuf::Message& body) {
        const int size = body.ByteSize();
        void* temp = alloca(size);
        if (body.SerializePartialToArray(temp, size)) {
            SendMessage(id, temp, size);
        }
    }

    void SendMessage(const UInt16 id, const void* const data, const int size) {
        MessageHead head;
        head._ID = id;
        head._Len = sizeof(head) + size;
        Send(&head, sizeof(head), false);
        Send(data, size);
    }
};

class IGameRobot : public Api::iModule {
public:
    virtual ~IGameRobot() {}

    enum class eEvent {
        RobotCreated,
        RobotRelease
    };

    virtual IRobotSession* QuerySession(const UInt64 characterId) const = 0;

    typedef void(*fEvent)(IRobotSession*);
    virtual void RegistEvent(const eEvent ev, const fEvent fun, const char* file = __FILE__, const int line = __LINE__) = 0;

    typedef std::function<void(IRobotSession* const, const UInt64 sessionId, const void* const data, const int size)> fServerMessage;
    virtual void RegistServerMessage(const UInt16 messageId, const fServerMessage call) = 0;


    template<typename PB>
    void RegistServerProtobuf(const UInt16 messageId, void(*callback)(IRobotSession* const, const UInt64 sessionId, const PB& pb)) {
        RegistServerMessage(messageId,
            [=](IRobotSession* const session, const UInt64 sessionId, const void* const data, const int size) {
                PB temp;
                if (temp.ParseFromArray(data, size)) {
                    callback(session, sessionId, temp);
                }
                else {
                    XASSERT(false, "RegistPlayerProtobuf ParseFromArray id %d", messageId);
                }
            }
        );
    }
};

#endif //__IGameRobot_h__
