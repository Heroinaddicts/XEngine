#include "Shutdown.h"

iTcpSession* Shutdown::OnMallocConnection(const char* remote_ip, const int remote_port) {
    g_Engine->Shutdown();
    return nullptr;
}

void Shutdown::OnError(iTcpSession* session) {
}

void Shutdown::OnRelease() {
}
