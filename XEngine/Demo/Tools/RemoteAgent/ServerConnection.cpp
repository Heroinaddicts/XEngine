#include "ServerConnection.h"
#include "MstscConnection.h"

int ServerConnection::OnReceive(const char* content, const int size) {
	TRACE(g_engine, "ServerConnection::OnReceive %d", size);
	if (size < sizeof(ProtoHeader)) {
		return 0;
	}

	const ProtoHeader* header = (const ProtoHeader*)content;
	if (header->len > size) {
		return 0;
	}

	switch (header->id) {
	case ServerToAgentProto::RequestNewConnectioon: {
		g_engine->GetNetApi()->LaunchTcpSession(xnew MstscConnection, "127.0.0.1", g_mstsc_port, CONNECTION_BUFF_LEN, CONNECTION_BUFF_LEN);
		break;
	}
	case ServerToAgentProto::ServerForwardToAgent: {
		if (nullptr == g_mstscConnection) {
			g_buffer.In(content + sizeof(header), header->len - sizeof(header));
		}
		else {
			if (g_buffer.Length() > 0) {
				g_mstscConnection->Send(g_buffer.GetData(), g_buffer.Length());
			}

			g_mstscConnection->Send(content + sizeof(header), header->len - sizeof(header));
		}
		break;
	}
	}

	return header->len;
}

void ServerConnection::OnConnected() {
	TRACE(g_engine, "ServerConnection::OnConnected");
	if (g_serverConnection) {
		g_serverConnection->Close();
	}
	g_serverConnection = this;
}

void ServerConnection::OnDisconnect() {
	TRACE(g_engine, "ServerConnection::OnDisconnect");
	g_serverConnection = nullptr;
	if (g_mstscConnection) {
		g_mstscConnection->Close();
	}

	START_TIMER(g_engine, this, 0, 100, 1, 100, 0);
}

void ServerConnection::OnConnectFailed() {
	START_TIMER(g_engine, this, 0, 100, 1, 100, 0);
}

void ServerConnection::OnStart(const int id, void* const context, const int64 timestamp) {
}

void ServerConnection::OnTimer(const int id, void* const context, const int64 timestamp) {
	g_engine->GetNetApi()->LaunchTcpSession(this, g_server_host, g_server_port, CONNECTION_BUFF_LEN, CONNECTION_BUFF_LEN);
}

void ServerConnection::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {
}

void ServerConnection::OnPause(const int id, void* const context, const int64 timestamp) {
}

void ServerConnection::OnResume(const int id, void* const context, const int64 timestamp) {
}
