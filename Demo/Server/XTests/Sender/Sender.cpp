#include "Sender.h"

#include <cstdlib>
#include <ctime>
#include <vector>

Api::iEngine* g_Engine = nullptr;

static SenderSession s_Session;
static std::vector<char> s_Packet;
static bool s_Connected = false;
static UInt64 s_SendCount = 0;
static Int64 s_LastSendTick = 0;
static Int64 s_SendBudget = 0;

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

static int GetSenderQps() {
    const char* qps = g_Engine->GetLaunchParameter("SenderQps");
    return qps ? SafeString::StringToInt32(qps) : 0;
}

static bool IsSenderPrintQps() {
    const char* print = g_Engine->GetLaunchParameter("SenderPrintQps");
    return print && SafeString::StringToInt32(print) != 0;
}

static void SendOnePacket() {
    const UInt16 bodySize = (UInt16)(rand() % (PerfTest::MaxBodySize + 1));
    PerfTest::BuildPacket(s_Packet, bodySize);
    s_Session.Send(s_Packet.data(), (int)s_Packet.size());
    ++s_SendCount;
}

void SenderSession::OnConnected() {
    s_Connected = true;
}

void SenderSession::OnConnectFailed() {
    s_Connected = false;
}

void SenderSession::OnDisconnected() {
    s_Connected = false;
}

bool Sender::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    srand((unsigned int)time(nullptr));
    s_Packet.reserve(PerfTest::MaxPacketSize);
    return true;
}

bool Sender::Launch(Api::iEngine* const engine) {
    if (!engine->GetNetApi()->LaunchTcpSession(&s_Session, GetForwarderHost(), GetForwarderPort(), 8 * SafeSystem::Network::MB)) {
        return false;
    }

    const int senderQps = GetSenderQps();
    const int senderInterval = senderQps > 0 ? 1 : GetSenderInterval();
    START_TIMER(engine, this, eSenderTimer::SendPacket, senderInterval, Api::Unlimited, senderInterval, 0);
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
            s_LastSendTick = timestamp;
            s_SendBudget = 0;
            return;
        }

        const int senderQps = GetSenderQps();
        if (senderQps <= 0) {
            SendOnePacket();
            break;
        }

        if (s_LastSendTick == 0) {
            s_LastSendTick = timestamp;
            return;
        }

        const Int64 elapsed = timestamp - s_LastSendTick;
        s_LastSendTick = timestamp;
        if (elapsed <= 0) {
            return;
        }

        s_SendBudget += elapsed * senderQps;
        Int64 sendCount = s_SendBudget / 1000;
        s_SendBudget = s_SendBudget % 1000;
        if (sendCount > 64) {
            sendCount = 64;
            s_SendBudget = 0;
        }

        for (Int64 index = 0; index < sendCount; ++index) {
            SendOnePacket();
        }
        break;
    }
    case eSenderTimer::PrintQps: {
        s_SendCount = 0;
        break;
    }
    }
}
