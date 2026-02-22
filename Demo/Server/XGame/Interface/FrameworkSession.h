#ifndef __FrameworkSession_h__
#define __FrameworkSession_h__

#include "iEngine.h"
#include "iNetApi.h"
#include "iTimerApi.h"
#include "SafeSystem.h"
#include "XPool.h"
#include "google/protobuf/message.h"

#include <unordered_map>
#include <functional>

UsingXEngine;

enum class eFrameworkSessionType {
    Gate = 0,
    Game = 1,
    Cache = 2
};

#pragma pack(push, 1)
struct MessageHead {
    Int32 _Len;
    UInt16 _ID;
};
#pragma pack(pop)

class IFrameworkSession;
typedef std::function<void(IFrameworkSession* const session, const UInt64 sessionId, const UInt16 id, const void* const data, const int size)> fServerMessage;

#define ALL_MESSAGE_ID 0

class IFrameworkSession : public Api::iTcpSession {
public:
    virtual ~IFrameworkSession() {}
    const eFrameworkSessionType _Type;

    IFrameworkSession(const eFrameworkSessionType type) : _Type(type) {}

    static void SetEngine(Api::iEngine* const engine) {
        s_Engine = engine;
    }

    template<typename PB>
    static void RegistServerProtobuf(const UInt16 messageId, void(*callback)(IFrameworkSession* const session, const UInt64 sessionId, const PB& body)) {
        RegistServerMessage(messageId,
            [=](IFrameworkSession* const session, const UInt64 sessionId, const UInt16 id, const void* const data, const int size) {
                PB temp;
                if (temp.ParseFromArray(data, size)) {
                    callback(session, sessionId, temp);
                }
                else {
                    ErrorLog(s_Engine, "RegistServerProtobuf ParseFromArray id %d", id);
                }
            }
        );
    }

    static void RegistServerMessage(const UInt16 id, const fServerMessage callback) {
        XASSERT(s_ServerMessageCallbacks.find(id) == s_ServerMessageCallbacks.end(), "double RegistServerMessage %d", id);
        s_ServerMessageCallbacks[id] = callback;
    }

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
protected:
    inline static std::unordered_map<UInt16, fServerMessage> s_ServerMessageCallbacks;
    inline static Api::iEngine* s_Engine;
};

class FrameworkSession : public IFrameworkSession, Api::iTimer {
public:
    virtual ~FrameworkSession() {}

    const UInt64 _SessionID;

    __forceinline static FrameworkSession* Create(const eFrameworkSessionType type) {
        return XPOOL_CREATE(s_Pool, type);
    }

    __forceinline static void Release(FrameworkSession* const session) {
        XPOOL_RELEASE(s_Pool, session);
    }

    __forceinline static FrameworkSession* Query(const UInt64 sessionId) {
        auto itor = s_FrameworkSessionMap.find(sessionId);
        if (s_FrameworkSessionMap.end() == itor) {
            return nullptr;
        }

        return itor->second;
    }

private:
    int OnReceive(const void* const content, const int size) override {
        if (size < sizeof(MessageHead)) {
            return 0;
        }

        const MessageHead* const head = (const MessageHead*)content;
        if (head->_Len > size) {
            return 0;
        }

        auto itor = s_ServerMessageCallbacks.find(head->_ID);
        if (s_ServerMessageCallbacks.end() != itor) {
            itor->second(this, _SessionID, head->_ID, ((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead));
        }
        else {
            itor = s_ServerMessageCallbacks.find(ALL_MESSAGE_ID);
            if (s_ServerMessageCallbacks.end() != itor) {
                itor->second(this, _SessionID, head->_ID, ((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead));
            }
        }

        return head->_Len;
    }

private:
    friend XPool<FrameworkSession>;
    FrameworkSession(const eFrameworkSessionType type) : IFrameworkSession(type), _SessionID(s_SessionIDAlloca++), _LastMessageTick(SafeSystem::Time::GetMilliSecond()) {}

    __forceinline void OnConnected() override {
        s_FrameworkSessionMap.insert({ _SessionID, this });
    }

    __forceinline void OnDisconnected() override {
        s_FrameworkSessionMap.erase(_SessionID);
    }

private:
    UInt64 _LastMessageTick;
    inline static XPool<FrameworkSession> s_Pool;
    inline static UInt64 s_SessionIDAlloca = 0;
    inline static std::map<UInt64, FrameworkSession*> s_FrameworkSessionMap;
};

#endif //__FrameworkSession_h__
