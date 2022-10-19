#include "RemoteAgent.h"
#include "MstscConnection.h"
#include "ServerConnection.h"

iEngine* g_engine = nullptr;

MstscConnection* g_mstscConnection = nullptr;
ServerConnection* g_serverConnection = nullptr;

const char* g_server_host = nullptr;
int g_server_port = 0;
int g_mstsc_port = 0;
XBuffer g_buffer(CONNECTION_BUFF_LEN);

bool RemoteAgent::Initialize(iEngine* const engine) {
    g_engine = engine;

    g_server_host = engine->GetLaunchParameter("ServerHost");
    g_server_port = SafeString::StringToInt(engine->GetLaunchParameter("ServerPort"));
    g_mstsc_port = SafeString::StringToInt(engine->GetLaunchParameter("MstscPort"));

    engine->GetNetApi()->LaunchTcpSession(xnew ServerConnection, g_server_host, g_server_port, CONNECTION_BUFF_LEN, CONNECTION_BUFF_LEN);
    return true;
}

bool RemoteAgent::Launch(iEngine* const engine) {
    return true;
}

bool RemoteAgent::Destroy(iEngine* const engine) {
    return true;
}
