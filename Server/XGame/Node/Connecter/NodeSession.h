#ifndef __NodeSession_h__
#define __NodeSession_h__

#include "Header.h"

class NodeSession : public iNodeSession, Api::iTimer {

public:
    virtual ~NodeSession() {}

    static NodeSession* Create(const std::string& remoteIp = "", const int remotePort = 0);
    static void Release(NodeSession* con);

    virtual int OnReceive(const char* content, const int size) override;
    virtual void OnConnected() override;
    virtual void OnDisconnect() override;
    virtual void OnConnectFailed() override;

    // 通过 iNodeSession 继承
    virtual int GetId() const override { return _Id; }
    virtual const char* GetName() const override { return _Name.c_str(); }
    virtual const char* GetRemoteIP() const override { return _RemoteIp.c_str(); }
    virtual int GetRemotePort() const override { return _RemotePort; }

    virtual void SetId(int id) { _Id = id; }
    virtual void SetName(const std::string& name) { _Name = name; }
    virtual void SetRemoteIp(const std::string& ip) { _RemoteIp = ip; }
    virtual void SetRemotePort(const int port) { _RemotePort = port; }

    template<typename T>
    __forceinline void SendProto(const NodeProto::eID& id, const T& body) {
        NodeProto::MessageHeader header;
        header._Len = sizeof(NodeProto::MessageHeader) + sizeof(body);
        header._MsgId = id;

        Send(&header, sizeof(header), false);
        Send(&body, sizeof(body));
    }

    __forceinline void SendMessage(const NodeProto::eID& id, const void* body = nullptr, const unsigned_int16 len = 0) {
        NodeProto::MessageHeader header;
        header._Len = sizeof(NodeProto::MessageHeader) + len;
        header._MsgId = id;
		if (len != 0) {
			Send(&header, sizeof(header), false);
			Send(body, len);
        }
		else {
			Send(&header, sizeof(header));
        }
    }

    __forceinline void GiveUp() {
        _IsGiveUP = true;
        Close();
    }
protected:
    // 通过 iTimer 继承
    virtual void OnStart(const int id, void* const context, const int64 timestamp) override;
    virtual void OnTimer(const int id, void* const context, const int64 timestamp) override;
    virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) override;
    virtual void OnPause(const int id, void* const context, const int64 timestamp) override;
    virtual void OnResume(const int id, void* const context, const int64 timestamp) override;

private:
    NodeSession(const std::string& remoteIp, const int remotePort) : _Id(INVALID_NODE_ID), _Name(""), _RemoteIp(remoteIp), _RemotePort(remotePort), _IsGiveUP(false) {}

private:
    int _Id;
    std::string _Name;
    std::string _RemoteIp;
    int _RemotePort;

    int64 _LastHeartBeatTick;

    bool _IsGiveUP;
};

#endif //__NodeSession_h__
