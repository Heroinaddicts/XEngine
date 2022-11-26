#include "Connecter.h"
#include "NodeSession.h"

iEngine* g_Engine = nullptr;
Connecter* g_Connecter = nullptr;

std::map<unsigned_int64, RouteInfo> g_RouteInfos;
std::set<iConnecter::fSessionEvent> g_SessionAppearEventPool;
std::set<iConnecter::fSessionEvent> g_SessionDisappearEventPool;
std::set<iConnecter::fRouteEvent> g_RouteEventPool;

std::map<int, NodeSession*> g_NodeIdSessionMap;
std::map<std::string, std::set<NodeSession*>> g_NodeNameSessionMap;

std::string g_NodeName;
int g_NodeID = INVALID_NODE_ID;

bool Connecter::Initialize(iEngine* const engine) {
    g_Connecter = this;
    g_Engine = engine;

    const char* name = engine->GetLaunchParameter("name");
    XASSERT(engine, "where is name");
    if (nullptr == engine) {
        ERROR(engine, "can not find launch parameter name");
        return false;
    }
    g_NodeName = name;

    const char* nodeIdString = engine->GetLaunchParameter("node_id");
    XASSERT(nodeIdString, "where is node_id");
    if (nullptr == nodeIdString) {
        ERROR(engine, "can not find launch parameter node_id");
        return false;
    }
    g_NodeID = SafeString::StringToInt(nodeIdString);

    const char* remoteNodes = engine->GetLaunchParameter("remote_nodes");
    if (remoteNodes) {
        std::vector<std::string> res;
        SafeString::Split(remoteNodes, ";", res);
        for (int i = 0; i < res.size(); i++) {
            std::vector<std::string> ip_port;
            SafeString::Split(res[i], ":", ip_port);
            XASSERT(ip_port.size() == 2, "check launch parameter remote_nodes %s", remoteNodes);
            if (ip_port.size() == 2) {
                std::string ip = ip_port[0];
                int port = SafeString::StringToInt(ip_port[1]);

                NodeSession* session = NodeSession::Create(ip, port);
                engine->GetNetApi()->LaunchTcpSession(session, ip.c_str(), port, NODE_SESSION_PIPE_SIZE, NODE_SESSION_PIPE_SIZE);
            }
            else {
                ERROR(engine, "error remote info %s", res[i].c_str());
                return false;
            }
        }
    }

    const char* nodeIp = engine->GetLaunchParameter("node_ip");
    const char* nodePortString = engine->GetLaunchParameter("node_port");
    if (nodePortString && nodeIp) {
        engine->GetNetApi()->LaunchTcpServer(this, nodeIp, SafeString::StringToInt(nodePortString), NODE_SESSION_PIPE_SIZE, NODE_SESSION_PIPE_SIZE);
    }

    return true;
}

bool Connecter::Launch(iEngine* const engine) {
    return true;
}

bool Connecter::Destroy(iEngine* const engine) {
    g_SessionAppearEventPool.clear();
    g_SessionDisappearEventPool.clear();
    return true;
}

void Connecter::RouteReport(const std::string& nodeName, const eRouteEvent ev, const unsigned_int64 guid) const {
    auto itor = g_NodeNameSessionMap.find(nodeName);
    if (itor != g_NodeNameSessionMap.end() && !itor->second.empty()) {
        oRouteReport report;
        report.set_event(ev);
        report.set_guid(guid);

        int len = report.ByteSize();
        void* temp = alloca(len);
        if (report.SerializePartialToArray(temp, len)) {
            for (auto i = itor->second.begin(); i != itor->second.end(); i++) {
                (*i)->SendMessage(eArchProtoID::RouteReport, temp, len);
            }
        }
        else {
            ERROR(g_Engine, "oRouteReport SerializePartialToArray falid");
        }
    }
}

void Connecter::RouteReport(const unsigned_int32 nodeId, const eRouteEvent ev, const unsigned_int64 guid) const {
    auto itor = g_NodeIdSessionMap.find(nodeId);
    if (g_NodeIdSessionMap.end() != itor) {
        oRouteReport report;
        report.set_event(ev);
        report.set_guid(guid);

        int len = report.ByteSize();
        void* temp = alloca(len);
        if (report.SerializePartialToArray(temp, len)) {
            itor->second->SendMessage(eArchProtoID::RouteReport, temp, len);
        }
        else {
            ERROR(g_Engine, "oRouteReport SerializePartialToArray falid");
        }
    }
}

const RouteInfo* Connecter::GetRouteInfo(const unsigned_int64 guid) const {
    auto itor = g_RouteInfos.find(guid);
    if (itor != g_RouteInfos.end()) {
        return &(itor->second);
    }
    return nullptr;
}

void Connecter::RegisterRouteEvent(fRouteEvent const fun) {
    g_RouteEventPool.insert(fun);
}

void Connecter::RegisterSessionEvent(const eConnectionEvent& ev, fSessionEvent const fun) {
    switch (ev) {
    case eConnectionEvent::NodeAppear: {
        g_SessionAppearEventPool.insert(fun);
        break;
    }
    case eConnectionEvent::NodeDisappear: {
        g_SessionDisappearEventPool.insert(fun);
        break;
    }
    }
}

void Connecter::UnregisterSessionEvent(const eConnectionEvent& ev, fSessionEvent const fun) {
    switch (ev) {
    case eConnectionEvent::NodeAppear: {
        g_SessionAppearEventPool.erase(fun);
        break;
    }
    case eConnectionEvent::NodeDisappear: {
        g_SessionDisappearEventPool.erase(fun);
        break;
    }
    }
}

iNodeSession* Connecter::QueryNodeSession(const int nodeId) const {
    auto itor = g_NodeIdSessionMap.find(nodeId);
    return itor != g_NodeIdSessionMap.end() ? itor->second : nullptr;
}

iTcpSession* Connecter::OnMallocConnection(const char* remote_ip, const int remote_port) {
    return NodeSession::Create();
}

void Connecter::OnError(iTcpSession* session) {
    if (session) {
        NodeSession::Release(dynamic_cast<NodeSession*>(session));
    }
}

void Connecter::OnRelease() {
}
