#ifndef __NodeSession_h__
#define __NodeSession_h__

#include "Header.h"

class NodeSession : public iNodeSession, Api::iTimer {
public:
    virtual ~NodeSession();

    static NodeSession* Create(const std::string& remoteIp = "", const int remotePort = 0);
    static void Release(NodeSession* con);

    virtual int OnReceive(const char* content, const int size) override;
    virtual void OnConnected() override;
    virtual void OnDisconnect() override;
    virtual void OnConnectFailed() override;

    // 通过 iNodeSession 继承
    virtual int GetId() const override { return _Id; }
    virtual const std::string& GetName() const override { return _Name; }
    virtual const std::string& GetRemoteIP() const override { return _RemoteIp; }
    virtual int GetRemotePort() const override { return _RemotePort; }

    virtual void SetId(int id) { _Id = id; }
    virtual void SetName(const std::string& name) { _Name = name; }
    virtual void SetRemoteIp(const std::string& ip) { _RemoteIp = ip; }
    virtual void SetRemotePort(const int port) { _RemotePort = port; }

    virtual void SendMessage(const unsigned_int16 id, const void* body = nullptr, const unsigned_int16 len = 0);

    template<typename T>
    __forceinline void SendProto(const unsigned_int16 id, const T& body) {
        int bodyLen = body.ByteSize();
        void* temp = alloca(bodyLen);
        if (body.SerializePartialToArray(temp, bodyLen)) {
            MessageHeader header;
            header._Len = sizeof(MessageHeader) + bodyLen;
            header._MsgId = id;
            Send(&header, sizeof(header), false);
            Send(temp, bodyLen);
        }
        else {
            XASSERT(false, "body SerializePartialToArray error");
        }
    }

    __forceinline void GiveUp() {
        _IsGiveUP = true;
        Close();
    }
protected:
    virtual void Register(const unsigned_int16 msgid, const iMessageHandle* handle);

    // 通过 iTimer 继承
    virtual void OnStart(const int id, void* const context, const int64 timestamp) override;
    virtual void OnTimer(const int id, void* const context, const int64 timestamp) override;
    virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) override;
    virtual void OnPause(const int id, void* const context, const int64 timestamp) override;
    virtual void OnResume(const int id, void* const context, const int64 timestamp) override;

private:
    NodeSession(const std::string& remoteIp, const int remotePort) : _Id(INVALID_NODE_ID), _Name(""), _RemoteIp(remoteIp), _RemotePort(remotePort), _IsGiveUP(false), _LastHeartBeatTick(SafeSystem::Time::GetMilliSecond()) {}

private:
    int _Id;
    std::string _Name;
    std::string _RemoteIp;
    int _RemotePort;
    int64 _LastHeartBeatTick;
    bool _IsGiveUP;

private:
    std::map<unsigned_int16, std::vector<const iMessageHandle*>> _MessageCallbacks;
};

#endif //__NodeSession_h__
