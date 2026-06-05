#include "Forwarder.h"

Api::iEngine* g_Engine = nullptr;

static XPool<ForwarderSenderSession> s_SenderSessionPool;
static ForwarderReciverSession s_ReciverSession;
static bool s_ReciverConnected = false;
static PerfTest::PacketStats s_ForwardStats;

enum eForwarderTimer {
    PrintQps = 1
};

static UInt16 GetForwarderPort() {
    const char* port = g_Engine->GetLaunchParameter("ForwarderPort");
    return port ? SafeString::StringToUInt16(port) : 7010;
}

static UInt16 GetReciverPort() {
    const char* port = g_Engine->GetLaunchParameter("ReciverPort");
    return port ? SafeString::StringToUInt16(port) : 7020;
}

static std::string GetReciverHost() {
    const char* host = g_Engine->GetLaunchParameter("ReciverHost");
    return host ? host : "127.0.0.1";
}

ForwarderSenderSession* ForwarderSenderSession::Create() {
    return XPOOL_CREATE(s_SenderSessionPool);
}

void ForwarderSenderSession::Release(ForwarderSenderSession* session) {
    XPOOL_RELEASE(s_SenderSessionPool, session);
}

void ForwarderSenderSession::OnConnected() {
    TraceLog(g_Engine, "Forwarder accepted Sender %s:%d", RemoteIp().c_str(), RemotePort());
}

void ForwarderSenderSession::OnDisconnected() {
    TraceLog(g_Engine, "Forwarder Sender disconnected %s:%d", RemoteIp().c_str(), RemotePort());
}

int ForwarderSenderSession::OnReceive(const void* const content, const int size) {
    return PerfTest::ConsumePackets(content, size,
        [&](const char* packet, const int packetSize, const char* body, const UInt16 bodySize) {
            if (s_ReciverConnected) {
                s_ReciverSession.Send(packet, packetSize);
                s_ForwardStats.Add(bodySize, packetSize);
            }
        }
    );
}

void ForwarderReciverSession::OnConnected() {
    s_ReciverConnected = true;
    TraceLog(g_Engine, "Forwarder connected to Reciver %s:%d", RemoteIp().c_str(), RemotePort());
}

void ForwarderReciverSession::OnConnectFailed() {
    s_ReciverConnected = false;
    TraceLog(g_Engine, "Forwarder connect Reciver failed");
}

void ForwarderReciverSession::OnDisconnected() {
    s_ReciverConnected = false;
    TraceLog(g_Engine, "Forwarder disconnected from Reciver");
}

bool Forwarder::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    return true;
}

bool Forwarder::Launch(Api::iEngine* const engine) {
    if (!engine->GetNetApi()->LaunchTcpServer(this, "0.0.0.0", GetForwarderPort(), 8 * SafeSystem::Network::MB)) {
        TraceLog(g_Engine, "Forwarder LaunchTcpServer failed");
        return false;
    }

    if (!engine->GetNetApi()->LaunchTcpSession(&s_ReciverSession, GetReciverHost(), GetReciverPort(), 8 * SafeSystem::Network::MB)) {
        TraceLog(g_Engine, "Forwarder LaunchTcpSession to Reciver failed");
        return false;
    }

    START_TIMER(engine, this, eForwarderTimer::PrintQps, 1000, Api::Unlimited, 1000, 0);
    return true;
}

bool Forwarder::Destroy(Api::iEngine* const engine) {
    STOP_TIMER(engine, this, eForwarderTimer::PrintQps, 0);
    s_ReciverSession.Close();
    Release();
    return true;
}

Api::iTcpSession* Forwarder::OnMallocTcpSession(const std::string& remoteIp, const UInt16 remotePort) {
    return ForwarderSenderSession::Create();
}

void Forwarder::OnError(Api::iTcpSession* session) {
}

void Forwarder::OnRelease() {
}

void Forwarder::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    if (id == eForwarderTimer::PrintQps) {
        const PerfTest::PacketStatsSnapshot stats = s_ForwardStats.TakeSnapshotAndReset();
        TraceLog(g_Engine, "Forwarder forward qps %llu bytes/s %llu mb/s %.2f body[0] %llu body[1-512] %llu body[512-1024] %llu body[1024-2048] %llu body[2048-4096] %llu reciver %s",
            stats.Count,
            stats.Bytes,
            (double)stats.Bytes / 1024.0 / 1024.0,
            stats.Zero,
            stats.Size1To512,
            stats.Size512To1024,
            stats.Size1024To2048,
            stats.Size2048To4096,
            s_ReciverConnected ? "connected" : "disconnected");
    }
}
