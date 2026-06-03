#include "RobotSession.h"

static XPool<RobotSession> s_Pool;
static UInt64 s_RobotSessionIDAlloca = 0;

static std::unordered_map<UInt64, RobotSession*> s_RobotSessionMap;
static std::unordered_map<UInt64, RobotSession*> s_AccountRobotSessionMap;

enum eRobotSessionTimer {
    CheckHeartBeat = 0,
    RandMove = 1,
    DelayClose = 2
};

RobotSession* RobotSession::Create() {
    RobotSession* RobotSession = XPOOL_CREATE(s_Pool);
    return RobotSession;
}

RobotSession* RobotSession::Query(const UInt64 sessionId) {
    auto itor = s_RobotSessionMap.find(sessionId);
    if (s_RobotSessionMap.end() == itor) {
        return nullptr;
    }

    return itor->second;
}

RobotSession* RobotSession::QueryByAccount(const UInt64 account) {
    auto itor = s_AccountRobotSessionMap.find(account);
    if (s_AccountRobotSessionMap.end() == itor) {
        return nullptr;
    }

    return itor->second;
}

void RobotSession::Release(RobotSession* RobotSession) {
    XPOOL_RELEASE(s_Pool, RobotSession);
}

RobotSession::RobotSession()
    : _LastTick(SafeSystem::Time::GetMilliSecond()),
    _LastRecvTick(SafeSystem::Time::GetMilliSecond()),
    _Account(INVALID_ROBOT_ID),
    _DeployCharacterID(INVALID_ROBOT_ID),
    _SessionID(s_RobotSessionIDAlloca++)
{
    START_TIMER(g_Engine, this, eRobotSessionTimer::CheckHeartBeat, 1000, Api::Unlimited, 1000, 0);
    s_RobotSessionMap.insert({ _SessionID, this });
    g_RobotSessionCount++;
}

RobotSession::~RobotSession() {
    s_RobotSessionMap.erase(_SessionID);
    s_AccountRobotSessionMap.erase(_Account);
    if (_DeployCharacterID != INVALID_ROBOT_ID) {
        g_RobotCount--;
        g_RobotEventPool.Trigger(IGameRobot::eEvent::RobotRelease, this);
    }
    STOP_TIMER(g_Engine, this, eRobotSessionTimer::CheckHeartBeat, 0);
    STOP_TIMER(g_Engine, this, eRobotSessionTimer::RandMove, 0);
    STOP_TIMER(g_Engine, this, eRobotSessionTimer::DelayClose, 0);
    g_RobotSessionCount--;
}

void RobotSession::OnConnected() {
    oLoginReq req;
    req.set_accounttype(eAccountType::Trial);
    req.set_unionid(
        SafeString::Int64ToString(SafeSystem::Time::GetMilliSecond()) + SafeString::Int64ToString(SafeTools::Rand())
    );

    SendProtobuf(eClientID::LoginReq, req);
}

void RobotSession::OnConnectFailed() {
    RobotSession::Release(this);
}

void RobotSession::OnDisconnected() {
    RobotSession::Release(this);
}
int RobotSession::OnReceive(const void* const content, const int size) {
    _LastRecvTick = SafeSystem::Time::GetMilliSecond();
    if (size < sizeof(MessageHead)) {
        return 0;
    }

    MessageHead* const head = (MessageHead*)content;
    if (head->_Len > size) {
        return 0;
    }

    switch (head->_ID) {
    case eServerID::LoginAsw: {
        oLoginAsw asw;
        if (!asw.ParseFromArray(((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead))) {
            XASSERT(false, "wtf");
            ErrorLog(g_Engine, "oLoginReq ParseFromArray error");
            Close();
            return 0;
        }

        if (asw.code() != eError::Succeed) {
            Close();
            return 0;
        }

        _Account = asw.account();
        s_AccountRobotSessionMap.insert({ _Account, this });
        DEBUG(g_Engine, "Login Use tick %llu", SafeSystem::Time::GetMilliSecond() - _LastTick);
        break;
    }
    case eServerID::NewPlayer: {
        oCreatePlayerReq req;
        req.set_nickname(SafeString::Int64ToString(SafeSystem::Time::GetMilliSecond()) + SafeString::Int64ToString(SafeTools::Rand()));
        SendProtobuf(eClientID::CreatePlayerReq, req);
        break;
    }
    case eServerID::CreatePlayerAsw: {
        oCreatePlayerAsw asw;
        if (!asw.ParseFromArray(((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead))) {
            XASSERT(false, "wtf");
            ErrorLog(g_Engine, "oCreatePlayerAsw ParseFromArray error");
            Close();
            return 0;
        }

        if (asw.code() != eError::Succeed) {
            Close();
            return 0;
        }

        DEBUG(g_Engine, "Create Player use tick %llu", SafeSystem::Time::GetMilliSecond() - _LastTick);
        break;
    }
    case eServerID::PlayerInfoPush: {
        DEBUG(g_Engine, "PlayerInfoPush use tick %llu", SafeSystem::Time::GetMilliSecond() - _LastTick);
        break;
    }
    case eServerID::SwitchCharacterPush: {
        oSwitchCharacterPush push;
        if (!push.ParseFromArray(((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead))) {
            XASSERT(false, "wtf");
            ErrorLog(g_Engine, "oCreatePlayerAsw ParseFromArray error");
            Close();
            return 0;
        }

        _DeployCharacterID = push.characterid();
        _Position.x = push.x();
        _Position.y = push.y();

        g_RobotCount++;
        g_RobotEventPool.Trigger(IGameRobot::eEvent::RobotCreated, this);
        break;
    }
    default: {
        auto itor = g_MessagePool.find(head->_ID);
        if (g_MessagePool.end() != itor) {
            itor->second(this, _SessionID, ((const char* const)content) + sizeof(MessageHead), head->_Len - sizeof(MessageHead));
        }
        break;
    }
    }

    return head->_Len;
}

void RobotSession::OnTimer(const int id, const UInt64 context, const Int64 timestamp) {
    switch (id) {
    case eRobotSessionTimer::CheckHeartBeat: {
        if (SafeSystem::Time::GetMilliSecond() - _LastRecvTick >= 10000) {
            //Close();
        }
        break;
    }
    case eRobotSessionTimer::RandMove: {
        oStateChange change;
        change.set_state(1);
        change.set_angle(SafeTools::Rand(360));
        change.mutable_pos()->set_x(0);
        change.mutable_pos()->set_y(0);
        this->SendProtobuf(eClientID::StateChange, change);
        break;
    }
    case eRobotSessionTimer::DelayClose: {
        //Close();
        break;
    }
    }
}
