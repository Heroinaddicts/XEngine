#include "UserConnection.h"
#include "AgentConnection.h"

#include "XBuffer.h"

int UserConnection::OnReceive(const char* content, const int size) {
	if (!g_agentConnection) {
		Close();
		return 0;
	}

	ProtoHeader header;
	header.len = sizeof(header) + size;
	header.id = ServerToAgentProto::ServerForwardToAgent;
	g_agentConnection->Send(&header, sizeof(header), false);
	g_agentConnection->Send(content, size);
	return size;
}

void UserConnection::OnConnected() {
	if (g_userConnection) {
		g_userConnection->Close();
	}

	if (nullptr == g_agentConnection) {
		Close();
		return;
	}

	ProtoHeader header;
	header.len = sizeof(header);
	header.id = ServerToAgentProto::RequestNewConnectioon;
	g_agentConnection->Send(&header, sizeof(header));

	g_userConnection = this;
}

void UserConnection::OnDisconnect() {
	xdel this;
}

void UserConnection::OnConnectFailed() {

}
