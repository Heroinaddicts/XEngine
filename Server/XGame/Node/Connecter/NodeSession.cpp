#include "NodeSession.h"

__forceinline void NodeSessionAppear(NodeSession* session) {
    auto itor = g_NodeSessionMap.find(session->GetId());
    if (itor == g_NodeSessionMap.end()) {
        g_NodeSessionMap.insert(std::make_pair(session->GetId(), session));
    }
    else {
        XASSERT(itor->second != session, "node report logic error");
        if (itor->second == session) {
            return;
        }

        NodeSession* temp = itor->second;
        itor->second = session;
        temp->GiveUp();
    }

    TRACE(g_Engine, "Node Appear %s %d", session->GetName().c_str(), session->GetId());
    for (auto i = g_SessionAppearEventPool.begin(); i != g_SessionAppearEventPool.end(); i++) {
        (*i)(session);
    }
}

__forceinline void NodeSessionDisappear(NodeSession* session) {
    auto itor = g_NodeSessionMap.find(session->GetId());
    if (itor == g_NodeSessionMap.end() || itor->second != session) {
        return;
    }

    g_NodeSessionMap.erase(itor);
    TRACE(g_Engine, "Node Disappear %s %d", session->GetName().c_str(), session->GetId());
    for (auto i = g_SessionDisappearEventPool.begin(); i != g_SessionDisappearEventPool.end(); i++) {
        (*i)(session);
    }
}

NodeSession* NodeSession::Create(const std::string& remoteIp, const int remotePort) {
    return xnew NodeSession(remoteIp, remotePort);
}

void NodeSession::Release(NodeSession* con) {
    xdel con;
}

int NodeSession::OnReceive(const char* content, const int size) {
    if (size < sizeof(NodeProto::MessageHeader)) {
        return 0;
    }

    const NodeProto::MessageHeader* header = (const NodeProto::MessageHeader*)content;
    if (header->_Len > size) {
        return 0;
    }

    const void* body = (((const char*)content) + sizeof(NodeProto::MessageHeader));
    const unsigned_int16 bodyLen = header->_Len - sizeof(NodeProto::MessageHeader);

    switch (header->_MsgId) {
    case NodeProto::eID::NodeReport: {
        XASSERT(header->_Len == sizeof(NodeProto::oNodeReport) + sizeof(NodeProto::MessageHeader), "node report message format error, len %d", header->_Len);
        if (header->_Len == sizeof(NodeProto::oNodeReport) + sizeof(NodeProto::MessageHeader)) {
            const NodeProto::oNodeReport* report = (const NodeProto::oNodeReport*)body;
            this->_Id = report->_Id;
            this->_Name = report->_Name;
            NodeSessionAppear(this);
        }
        break;
    }
    case NodeProto::eID::HeartBeat: {
        _LastHeartBeatTick = SafeSystem::Time::GetMilliSecond();
        break;
    }
    default:
        XASSERT(false, "unkown message id %d", header->_MsgId);
        break;
    }

    auto itor = _MessageCallbacks.find(header->_MsgId);
    if (_MessageCallbacks.end() != itor) {
        for (auto i = itor->second.begin(); i != itor->second.end(); i++) {
            (*i)(this, header->_MsgId, body, bodyLen);
        }
    }

    return header->_Len;
}

void NodeSession::OnConnected() {
    _LastHeartBeatTick = SafeSystem::Time::GetMilliSecond();

    NodeProto::oNodeReport report;
    report._Id = g_NodeID;
    report.SetName(g_NodeName.c_str());

    SendProto(NodeProto::eID::NodeReport, report);

    START_TIMER(g_Engine, this, TimerConfig::eID::HeartBeat, TimerConfig::HeartBeatInterval, Api::Unlimited, TimerConfig::HeartBeatInterval, this);
    START_TIMER(g_Engine, this, TimerConfig::eID::CheckHeartBeat, TimerConfig::CheckHeartBeatInterval, Api::Unlimited, TimerConfig::CheckHeartBeatInterval, this);
}

void NodeSession::OnDisconnect() {
    g_Engine->GetTimerApi()->KillTimer(this, TimerConfig::eID::HeartBeat, this);
    g_Engine->GetTimerApi()->KillTimer(this, TimerConfig::eID::CheckHeartBeat, this);

    if (_IsGiveUP) {
        NodeSession::Release(this);
        return;
    }

    NodeSessionDisappear(this);
    if (_RemoteIp != "") {
        ERROR(g_Engine, "Node session %s:%d disconnected, reconnect", _RemoteIp.c_str(), _RemotePort);
        START_TIMER(g_Engine, this, TimerConfig::eID::Reconnect, TimerConfig::ReconnectInterval, 1, TimerConfig::ReconnectInterval, this);
    }
    else {
        GiveUp();
    }
}

void NodeSession::OnConnectFailed() {
    ERROR(g_Engine, "Node session %s:%d connect failed, reconnect", _RemoteIp.c_str(), _RemotePort);
    START_TIMER(g_Engine, this, TimerConfig::eID::Reconnect, TimerConfig::ReconnectInterval, 1, TimerConfig::ReconnectInterval, this);
}

void NodeSession::RegisterMessage(const unsigned_int16 msgid, fSessionMessage fun) {
    auto itor = _MessageCallbacks.find(msgid);
    if (_MessageCallbacks.end() == itor) {
        itor = _MessageCallbacks.insert(std::make_pair(msgid, std::set<fSessionMessage>())).first;
    }

    itor->second.insert(fun);
}

void NodeSession::SendMessage(const unsigned_int16 id, const void* body, const unsigned_int16 len) {
    NodeProto::MessageHeader header;
    header._Len = sizeof(NodeProto::MessageHeader) + len;
    header._MsgId = id;
    if (len != 0) {
        Send(&header, sizeof(header), false);
        Send(body, len);
    }
    else {
        Send(&header, sizeof(header));
    }
}

void NodeSession::OnStart(const int id, void* const context, const int64 timestamp) {

}

void NodeSession::OnTimer(const int id, void* const context, const int64 timestamp) {
    switch (id) {
    case TimerConfig::eID::Reconnect: {
        g_Engine->GetNetApi()->LaunchTcpSession(this, _RemoteIp.c_str(), _RemotePort, NODE_SESSION_PIPE_SIZE, NODE_SESSION_PIPE_SIZE);
        break;
    }
    case TimerConfig::eID::HeartBeat: {
        SendMessage(NodeProto::eID::HeartBeat);
        break;
    case TimerConfig::eID::CheckHeartBeat: {
        if (SafeSystem::Time::GetMilliSecond() - _LastHeartBeatTick > 2 * TimerConfig::HeartBeatInterval) {
            Close();
        }
        break;
    }
    }
    }
}

void NodeSession::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {

}

void NodeSession::OnPause(const int id, void* const context, const int64 timestamp) {

}

void NodeSession::OnResume(const int id, void* const context, const int64 timestamp) {

}
