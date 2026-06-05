#include "Reciver.h"

Api::iEngine* g_Engine = nullptr;

static XPool<ReciverSession> s_SessionPool;
static PerfTest::PacketStats s_RecvStats;

enum eReciverTimer {
    PrintQps = 1
};

static UInt16 GetReciverPort() {
    const char* port = g_Engine->GetLaunchParameter("ReciverPort");
    return port ? SafeString::StringToUInt16(port) : 7020;
}

ReciverSession* ReciverSession::Create() {
    return XPOOL_CREATE(s_SessionPool);
}

void ReciverSession::Release(ReciverSession* session) {
    XPOOL_RELEASE(s_SessionPool, session);
}

void ReciverSession::OnConnected() {
}

void ReciverSession::OnDisconnected() {
}

int ReciverSession::OnReceive(const void* const content, const int size) {
    return PerfTest::ConsumePackets(content, size,
        [&](const char* packet, const int packetSize, const char* body, const UInt16 bodySize) {
            s_RecvStats.Add(bodySize, packetSize);
        }
    );
}

bool Reciver::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    return true;
}

bool Reciver::Launch(Api::iEngine* const engine) {
    if (!engine->GetNetApi()->LaunchTcpServer(this, "0.0.0.0", GetReciverPort(), 8 * SafeSystem::Network::MB)) {
        return false;
    }

    START_TIMER(engine, this, eReciverTimer::PrintQps, 1000, Api::Unlimited, 1000, 0);
    return true;
}

bool Reciver::Destroy(Api::iEngine* const engine) {
    STOP_TIMER(engine, this, eReciverTimer::PrintQps, 0);
    Release();
    return true;
}

Api::iTcpSession* Reciver::OnMallocTcpSession(const std::string& remoteIp, const UInt16 remotePort) {
    return ReciverSession::Create();
}

void Reciver::OnError(Api::iTcpSession* session) {
}

void Reciver::OnRelease() {
}

void Reciver::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    if (id == eReciverTimer::PrintQps) {
        const PerfTest::PacketStatsSnapshot stats = s_RecvStats.TakeSnapshotAndReset();
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
