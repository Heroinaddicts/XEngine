#include "RobotSkill.h"

Api::iEngine* g_Engine = nullptr;
IGameRobot* g_GameRobot = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
IRobotSceneManager* g_RobotSceneManager = nullptr;

static RobotSkill* s_Self = nullptr;

static std::set<UInt64> s_Guids;

static UInt64 RandomTarget() {
    if (s_Guids.empty()) {
        return 0;
    }

    if (SafeTools::Rand() % 5 == 0) {
        return SafeSystem::Time::GetMilliSecond();
    }

    int index = SafeTools::Rand() % s_Guids.size();
    for (auto it = s_Guids.begin(); it != s_Guids.end(); it++) {
        if (0 == index) {
            return *it;
        }

        index--;
    }

    return 0;
}

namespace eRobotSkill {
    enum {
        RANDOM_SKILL_LINE,
        RANDOM_SKILL_TRACE,
    };
}

bool RobotSkill::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool RobotSkill::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_GameRobot, GameRobot);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_RobotSceneManager, RobotSceneManager);

    g_GameRobot->RegistEvent(IGameRobot::eEvent::RobotCreated, OnRobotCreated);
    g_GameRobot->RegistEvent(IGameRobot::eEvent::RobotRelease, OnRobotRelease);
    return true;
}

bool RobotSkill::Destroy(Api::iEngine* const engine) {
    return true;
}

void RobotSkill::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    IRobotSession* const session = (IRobotSession*)context;

    switch (id) {
    case eRobotSkill::RANDOM_SKILL_LINE: {
        oSkillCastReq req;
        req.set_skillconfigid(11003);
        float angle = SafeTools::Rand() % 360;
        req.set_angle(angle);

        DEBUG(g_Engine, "eRobotSkill::RANDOM_SKILL_LINE %f", angle);

        session->SendProtobuf(eClientID::SkillCastReq, req);
        break;
    }
    case eRobotSkill::RANDOM_SKILL_TRACE: {
        oSkillCastReq req;
        req.set_skillconfigid(11004);
        req.set_targetguid(RandomTarget());
        session->SendProtobuf(eClientID::SkillCastReq, req);
        break;
    }
    }
}

void RobotSkill::OnRobotCreated(IRobotSession* const session) {
    s_Guids.insert(session->DeployCharacterID());
    //START_TIMER(g_Engine, s_Self, eRobotSkill::RANDOM_SKILL_LINE, 5000 + SafeTools::Rand() % 2000, Api::Unlimited, 1000, session);
    //START_TIMER(g_Engine, s_Self, eRobotSkill::RANDOM_SKILL_TRACE, 5000 + SafeTools::Rand() % 2000, Api::Unlimited, 1000, session);
}

void RobotSkill::OnRobotRelease(IRobotSession* const session) {
    STOP_TIMER(g_Engine, s_Self, eRobotSkill::RANDOM_SKILL_LINE, session);
    STOP_TIMER(g_Engine, s_Self, eRobotSkill::RANDOM_SKILL_TRACE, session);
    s_Guids.erase(session->DeployCharacterID());
}
