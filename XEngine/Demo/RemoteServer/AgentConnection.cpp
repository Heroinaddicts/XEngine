#include "AgentConnection.h"
#include "UserConnection.h"

int AgentConnection::OnReceive(const char* content, const int size) {
	if (g_userConnection) {
		g_userConnection->Send(content, size);
	}

	return size;
}

void AgentConnection::OnConnected() {
	if (g_agentConnection) {
		g_agentConnection->Close();
	}

	g_agentConnection = this;
}

void AgentConnection::OnDisconnect() {
	xdel this;
}

void AgentConnection::OnConnectFailed() {
}
