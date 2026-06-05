#include "Sender.h"

#include <cstdlib>
#include <ctime>
#include <vector>

Api::iEngine* g_Engine = nullptr;

static SenderSession s_Session;
static std::vector<char> s_Packet;
static bool s_Connected = false;
static UInt64 s_SendCount = 0;

enum eSenderTimer {
    SendPacket = 1,
    PrintQps = 2
};

static UInt16 GetForwarderPort() {
    const char* port = g_Engine->GetLaunchParameter("ForwarderPort");
    return port ? SafeString::StringToUInt16(port) : 7010;
}

static std::string GetForwarderHost() {
    const char* host = g_Engine->GetLaunchParameter("ForwarderHost");
    return host ? host : "127.0.0.1";
}

static int GetSenderInterval() {
    const char* interval = g_Engine->GetLaunchParameter("SenderInterval");
    return interval ? SafeString::StringToInt32(interval) : 2;
}

static bool IsSenderPrintQps() {
    const char* print = g_Engine->GetLaunchParameter("SenderPrintQps");
    return print && SafeString::StringToInt32(print) != 0;
}

void SenderSession::OnConnected() {
    s_Connected = true;
    TraceLog(g_Engine, "Sender connected to Forwarder %s:%d", RemoteIp().c_str(), RemotePort());
}

void SenderSession::OnConnectFailed() {
    s_Connected = false;
    TraceLog(g_Engine, "Sender connect Forwarder failed");
}

void SenderSession::OnDisconnected() {
    s_Connected = false;
    TraceLog(g_Engine, "Sender disconnected from Forwarder");
}

bool Sender::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    srand((unsigned int)time(nullptr));
    s_Packet.reserve(PerfTest::MaxPacketSize);
    return true;
}

bool Sender::Launch(Api::iEngine* const engine) {
    if (!engine->GetNetApi()->LaunchTcpSession(&s_Session, GetForwarderHost(), GetForwarderPort(), 8 * SafeSystem::Network::MB)) {
        TraceLog(g_Engine, "Sender LaunchTcpSession failed");
        return false;
    }

    START_TIMER(engine, this, eSenderTimer::SendPacket, GetSenderInterval(), Api::Unlimited, GetSenderInterval(), 0);
    if (IsSenderPrintQps()) {
        START_TIMER(engine, this, eSenderTimer::PrintQps, 1000, Api::Unlimited, 1000, 0);
    }
    return true;
}

bool Sender::Destroy(Api::iEngine* const engine) {
    STOP_TIMER(engine, this, eSenderTimer::SendPacket, 0);
    STOP_TIMER(engine, this, eSenderTimer::PrintQps, 0);
    s_Session.Close();
    return true;
}

void Sender::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eSenderTimer::SendPacket: {
        if (!s_Connected) {
            return;
        }

        const UInt16 bodySize = (UInt16)(rand() % (PerfTest::MaxBodySize + 1));
        PerfTest::BuildPacket(s_Packet, bodySize);
        s_Session.Send(s_Packet.data(), (int)s_Packet.size());
        ++s_SendCount;
        break;
    }
    case eSenderTimer::PrintQps: {
        TraceLog(g_Engine, "Sender send qps %llu", s_SendCount);
        s_SendCount = 0;
        break;
    }
    }
}
