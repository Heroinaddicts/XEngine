#include "TestNet.h"
#include "TestClient.h"

iEngine* g_Engine = nullptr;
TestNet* g_TestNet = nullptr;
int g_ClientCount = 0;

static const char* s_RemoteHost = nullptr;
static int s_RemotePort = 0;

#define PIPE_LEN (4*1024*1024)

bool TestNet::Initialize(iEngine* const engine) {
    g_TestNet = this;
    g_Engine = engine;

    s_RemoteHost = engine->GetLaunchParameter("RemoteHost");
    s_RemotePort = SafeString::StringToInt(engine->GetLaunchParameter("RemotePort"));

    const int listenPort = SafeString::StringToInt(engine->GetLaunchParameter("ListenPort"));

    engine->GetNetApi()->LaunchTcpServer(this, "0.0.0.0", listenPort, PIPE_LEN, PIPE_LEN);

    START_TIMER(engine, this, TimeConfig::eTimeID::NewClient, 100, Api::Unlimited, 100, this);
    return true;
}

bool TestNet::Launch(iEngine* const engine) {
    return true;
}

bool TestNet::Destroy(iEngine* const engine) {
    return true;
}

iTcpSession* TestNet::OnMallocConnection(const char* remote_ip, const int remote_port) {
    return xnew TestClient;
}

void TestNet::OnError(iTcpSession* session) {
    if (session) {
        xdel session;
    }
}

void TestNet::OnRelease() {

}

void TestNet::OnStart(const int id, void* const context, const int64 timestamp) {
}

void TestNet::OnTimer(const int id, void* const context, const int64 timestamp) {
    switch (id) {
    case TimeConfig::eTimeID::NewClient: {
        if (g_ClientCount > 100) {
            return;
        }
        g_Engine->GetNetApi()->LaunchTcpSession(xnew TestClient, s_RemoteHost, s_RemotePort, PIPE_LEN, PIPE_LEN);
        break;
    }
    }
}

void TestNet::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {
}

void TestNet::OnPause(const int id, void* const context, const int64 timestamp) {
}

void TestNet::OnResume(const int id, void* const context, const int64 timestamp) {
}
