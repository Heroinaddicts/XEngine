#include "AgentConnection.h"
#include "UserConnection.h"

int AgentConnection::OnReceive(const char* content, const int size) {
	TRACE(g_engine, "AgentConnection::OnReceive %d", size);
	if (g_userConnection) {
		g_userConnection->Send(content, size);
	}

	return size;
}

void AgentConnection::OnConnected() {
	TRACE(g_engine, " AgentConnection::OnConnected");
	if (g_agentConnection) {
		g_agentConnection->Close();
	}
	g_agentConnection = this;
}

void AgentConnection::OnDisconnect() {
	TRACE(g_engine, " AgentConnection::OnDisconnect");
	if (g_agentConnection == this) {
		g_agentConnection = nullptr;
	}
	xdel this;
}

void AgentConnection::OnConnectFailed() {
}
