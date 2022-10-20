#include "MstscConnection.h"
#include "ServerConnection.h"

int MstscConnection::OnReceive(const char* content, const int size) {
	TRACE(g_engine, "MstscConnection::OnReceive %d", size);
	if (nullptr == g_serverConnection) {
		Close();
		return 0;
	}

	g_serverConnection->Send(content, size);
	return size;
}

void MstscConnection::OnConnected() {
	TRACE(g_engine, "MstscConnection::OnConnected");
	if (g_mstscConnection) {
		g_mstscConnection->Close();
	}

	g_mstscConnection = this;
	Send(g_buffer.GetData(), g_buffer.Length());
	g_buffer.Clear();
}

void MstscConnection::OnDisconnect() {
	TRACE(g_engine, "MstscConnection::OnDisconnect");
	if (g_mstscConnection == this) {
		g_mstscConnection = nullptr;
	}
	xdel this;
}

void MstscConnection::OnConnectFailed() {
}

