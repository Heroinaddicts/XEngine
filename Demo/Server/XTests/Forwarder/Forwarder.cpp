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
}

void ForwarderSenderSession::OnDisconnected() {
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
}

void ForwarderReciverSession::OnConnectFailed() {
    s_ReciverConnected = false;
}

void ForwarderReciverSession::OnDisconnected() {
    s_ReciverConnected = false;
}

bool Forwarder::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    return true;
}

bool Forwarder::Launch(Api::iEngine* const engine) {
    if (!engine->GetNetApi()->LaunchTcpServer(this, "0.0.0.0", GetForwarderPort(), 8 * SafeSystem::Network::MB)) {
        return false;
    }

    if (!engine->GetNetApi()->LaunchTcpSession(&s_ReciverSession, GetReciverHost(), GetReciverPort(), 8 * SafeSystem::Network::MB)) {
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
        TraceLog(g_Engine, "QPS : %llu\nbytes/s : %llu\nmb/s : %.2f\ntotal count : %llu\n1-512 count : %llu\n513-1024 count : %llu\n1025-2048 count : %llu\n2049-4096 count : %llu",
            stats.Count,
            stats.Bytes,
            (double)stats.Bytes / 1024.0 / 1024.0,
            stats.TotalCount,
            stats.Size1To512,
            stats.Size512To1024,
            stats.Size1024To2048,
            stats.Size2048To4096);
    }
}
