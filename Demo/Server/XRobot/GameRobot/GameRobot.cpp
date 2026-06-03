#include "GameRobot.h"
#include "RobotSession.h"

Api::iEngine* g_Engine = nullptr;
XEventPool<IGameRobot::eEvent, IRobotSession*> g_RobotEventPool;
std::unordered_map<UInt16, IGameRobot::fServerMessage> g_MessagePool;

static GameRobot* s_Self = nullptr;

int g_RobotCount = 0;
int g_RobotSessionCount = 0;

static std::string s_Host;
static UInt16 s_Port = 18888;
static int s_MaxRobotSessionCount = 100;
static int s_Interval = 100;

bool GameRobot::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;

    s_Host = engine->GetLaunchParameter("Host");
    s_Port = engine->GetLaunchParameterUInt16("Port");

    s_MaxRobotSessionCount = engine->GetLaunchParameterInt32("RobotCount");
    s_Interval = engine->GetLaunchParameterInt32("Interval");
    return true;
}

bool GameRobot::Launch(Api::iEngine* const engine) {
    START_TIMER(g_Engine, this, 0, s_Interval, Api::Unlimited, s_Interval, 0);
    START_TIMER(g_Engine, this, 1, 1000, Api::Unlimited, 1000, 0);
    return true;
}

bool GameRobot::Destroy(Api::iEngine* const engine) {
    return true;
}

void GameRobot::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case 0: {
        if (g_RobotSessionCount < s_MaxRobotSessionCount) {
            g_Engine->GetNetApi()->LaunchTcpSession(RobotSession::Create(), s_Host, s_Port, 16 * SafeSystem::Network::KB);
        }
        break;
    }
    case 1: {
        DEBUG(g_Engine, "Tests Count %d", g_RobotCount);
        break;
    }
    }
}

void GameRobot::RegistServerMessage(const UInt16 messageId, const fServerMessage call) {
    g_MessagePool.insert({ messageId, call });
}

IRobotSession* GameRobot::QuerySession(const UInt64 account) const {
    return RobotSession::QueryByAccount(account);
}

void GameRobot::RegistEvent(const eEvent ev, const fEvent fun, const char* file, const int line) {
    g_RobotEventPool.Register(ev, fun, file, line);
}
