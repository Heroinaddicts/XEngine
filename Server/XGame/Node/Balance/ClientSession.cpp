#include "ClientSession.h"

int ClientSession::OnReceive(const char* content, const int size) {
    return size;
}

void ClientSession::OnConnected() {
    if (g_GateList.size() > 0) {

        //to do ��ѡgate��Ϣ ���͸��ͻ���


        g_SelectIndexGateList++;
        g_SelectIndexGateList = g_SelectIndexGateList % g_GateList.size();
    }
    else {
        //to do ���߿ͻ��� û��gate
    }

    START_TIMER(g_Engine, this, TimerConfig::eID::DelayClose, TimerConfig::DelayCloseTimeLen, 1, TimerConfig::DelayCloseTimeLen, this);
}

void ClientSession::OnDisconnect() {
    g_Engine->GetTimerApi()->KillTimer(this, TimerConfig::eID::DelayClose, this);
}

void ClientSession::OnConnectFailed() {

}

void ClientSession::OnStart(const int id, void* const context, const int64 timestamp) {

}

void ClientSession::OnTimer(const int id, void* const context, const int64 timestamp) {
    switch (id) {
    case TimerConfig::eID::DelayClose: {
        Close();
        break;
    }
    }
}

void ClientSession::OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) {

}

void ClientSession::OnPause(const int id, void* const context, const int64 timestamp) {

}

void ClientSession::OnResume(const int id, void* const context, const int64 timestamp) {

}
