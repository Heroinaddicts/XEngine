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

    for (auto i = g_SessionAppearEventPool.begin(); i != g_SessionAppearEventPool.end(); i++) {
        (*i)(session);
    }
}

__forceinline void NodeSessionDisappear(NodeSession* session) {

}

NodeSession* NodeSession::Create(const int id, const std::string& name, const std::string& remoteIp, const int remotePort) {
    return xnew NodeSession(id, name, remoteIp, remotePort);
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

    switch (header->_MsgId) {
    case NodeProto::eID::NodeReport: {
        XASSERT(header->_Len == sizeof(NodeProto::oNodeReport) + sizeof(NodeProto::MessageHeader), "node report message format error, len %d", header->_Len);
        if (header->_Len == sizeof(NodeProto::oNodeReport) + sizeof(NodeProto::MessageHeader)) {
            const NodeProto::oNodeReport* report = (const NodeProto::oNodeReport*)(((const char*)content) + sizeof(NodeProto::MessageHeader));
            this->_Id = report->_Id;
            this->_Name = report->_Name;
            NodeSessionAppear(this);
        }
        break;
    }
    default:
        XASSERT(false, "unkown message id %d", header->_MsgId);
        break;
    }

    return header->_Len;
}

void NodeSession::OnConnected() {
    NodeProto::oNodeReport report;
    report._Id = _Id;
    report.SetName(_Name.c_str());

    SendProto(NodeProto::eID::NodeReport, report);
}

void NodeSession::OnDisconnect() {
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

void NodeSession::OnStart(const int id, void* const context, const int64 timestamp) {

}

void NodeSession::OnTimer(const int id, void* const context, const int64 timestamp) {
    switch (id) {
    case TimerConfig::eID::Reconnect: {
        g_Engine->GetNetApi()->LaunchTcpSession(this, _RemoteIp.c_str(), _RemotePort, NODE_SESSION_PIPE_SIZE, NODE_SESSION_PIPE_SIZE);
        break;
    }
    case TimerConfig::eID::NodeHeartBeat: {
        break;
    }
    }
}

void NodeSession::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {

}

void NodeSession::OnPause(const int id, void* const context, const int64 timestamp) {

}

void NodeSession::OnResume(const int id, void* const context, const int64 timestamp) {

}
