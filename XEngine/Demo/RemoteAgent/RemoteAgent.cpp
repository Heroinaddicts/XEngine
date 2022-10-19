#include "RemoteAgent.h"

iEngine* g_engine = nullptr;

bool RemoteAgent::Initialize(iEngine* const engine) {
    g_engine = engine;
    return true;
}

bool RemoteAgent::Launch(iEngine* const engine) {
    return true;
}

bool RemoteAgent::Destroy(iEngine* const engine) {
    return true;
}

iTcpSession* RemoteAgent::OnMallocConnection(const char* remote_ip, const int remote_port) {
    return nullptr;
}

void RemoteAgent::OnError(iTcpSession* session) {
}

void RemoteAgent::OnRelease() {
}

int RemoteAgent::OnReceive(const char* content, const int size) {
    return 0;
}

void RemoteAgent::OnConnected() {
}

void RemoteAgent::OnDisconnect() {
}

void RemoteAgent::OnConnectFailed() {
}
