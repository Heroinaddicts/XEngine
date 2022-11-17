#include "RemoteServer.h"
#include "AgentConnection.h"
#include "UserConnection.h"

iEngine* g_engine = nullptr;
AgentConnection* g_agentConnection = nullptr;
UserConnection* g_userConnection = nullptr;

class AgentServer : public iTcpServer {
public:
    virtual ~AgentServer() {}

    // 通过 iTcpServer 继承
    virtual iTcpSession* OnMallocConnection(const char* remote_ip, const int remote_port) override {
        return xnew AgentConnection;
    }

    virtual void OnError(iTcpSession* session) override {
    }

    virtual void OnRelease() override {
    }
};

class UserServer : public iTcpServer {
public:
	virtual ~UserServer() {}

    // 通过 iTcpServer 继承
    virtual iTcpSession* OnMallocConnection(const char* remote_ip, const int remote_port) override {
        return xnew UserConnection;
    }

    virtual void OnError(iTcpSession* session) override {

    }

    virtual void OnRelease() override {

    }
};

bool RemoteServer::Initialize(iEngine* const engine) {
    g_engine = engine;

    int agentPort = SafeString::StringToInt(engine->GetLaunchParameter("AgentPort"));
    int userPort = SafeString::StringToInt(engine->GetLaunchParameter("UserPort"));

    engine->GetNetApi()->LaunchTcpServer(xnew UserServer, "0.0.0.0", userPort, CONNECTION_BUFF_LEN, CONNECTION_BUFF_LEN);
    engine->GetNetApi()->LaunchTcpServer(xnew AgentServer, "0,0,0,0", agentPort, CONNECTION_BUFF_LEN, CONNECTION_BUFF_LEN);
    return true;
}

bool RemoteServer::Launch(iEngine* const engine) {
    return true;
}

bool RemoteServer::Destroy(iEngine* const engine) {
    return true;
}
