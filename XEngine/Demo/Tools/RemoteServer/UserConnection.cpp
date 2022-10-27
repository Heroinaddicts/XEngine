#include "UserConnection.h"
#include "AgentConnection.h"

#include "XBuffer.h"

int UserConnection::OnReceive(const char* content, const int size) {
	TRACE(g_engine, "UserConnection::OnReceive %d", size);
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
	TRACE(g_engine, " UserConnection::OnConnected");
	if (g_userConnection) {
		g_userConnection->Close();
	}
	g_userConnection = this;

	if (nullptr == g_agentConnection) {
		Close();
		return;
	}

	ProtoHeader header;
	header.len = sizeof(header);
	header.id = ServerToAgentProto::RequestNewConnectioon;
	g_agentConnection->Send(&header, sizeof(header));

}

void UserConnection::OnDisconnect() {
	TRACE(g_engine, " UserConnection::OnDisconnect");
	if (g_userConnection == this) {
		g_userConnection = nullptr;
	}
	xdel this;
}

void UserConnection::OnConnectFailed() {

}
