#include "Net.h"
#include "Tcper.h"
#include "Accepter.h"
#include "SafeSystem.h"

namespace XEngine {
    int g_EpollerFd = INVALID_FD;
    XPool<Associat> g_AssociatPool;
    std::set<Tcper*> g_TcperNeedSend;

    iNet* Net::GetInstance() {
        static Net static_net;
        return &static_net;
    }

    bool Net::Initialize(Api::iEngine* const engine) {
        if (INVALID_FD == (g_EpollerFd = epoll_create(EPOLLER_DESC_COUNT))) {
            XERROR(g_Engine, "create epoller fd error %s", strerror(errno));
            return false;
        }
        return true;
    }

    bool Net::Launch(Api::iEngine* const engine) {
        return true;
    }

    void Net::Release(Api::iEngine* const engine) {
    }

    void Net::EarlyUpdate(Api::iEngine* const engine) {
        static struct epoll_event evs[EPOLLER_MAX_EVENT_COUNT];
        int64 tick = SafeSystem::Time::GetMilliSecond();
        do {
            errno = 0;
            int count = 0;
            if (0 == (count = epoll_wait(g_EpollerFd, evs, EPOLLER_MAX_EVENT_COUNT, 1)) || count == -1) {
                continue;
            }

            for (int i = 0; i < count; i++) {
                struct Associat* p = (struct Associat*)evs[i].data.ptr;
                p->_Completer->OnCompleter(p, p->_Ev, evs[i]);
            }
        } while (SafeSystem::Time::GetMilliSecond() - tick < 2);
    }

    void Net::Update(Api::iEngine* const engine) {
    }

    void Net::FixedUpdate(Api::iEngine* const engine) {

    }

    void Net::LaterUpdate(Api::iEngine* const engine) {
        uint64 tick = SafeSystem::Time::GetMilliSecond();
        for (auto i = g_TcperNeedSend.begin(); i != g_TcperNeedSend.end(); i++) {
            (*i)->DoSend();
        }
        uint64 use = SafeSystem::Time::GetMilliSecond() - tick;
        if (use > 5) {
            XLOG(XEngine::Engine::GetInstance(), "Net::LaterUpdate use %d, _TcpNeedSendPool size %d", use, g_TcperNeedSend.size());
        }
        g_TcperNeedSend.clear();
    }

    bool Net::LaunchTcpSession(Api::iTcpSession* session, const char* host, const int port, int max_ss, int max_rs) {
        Tcper* tcp = Tcper::Create(session, host, port, max_ss, max_rs);
        return tcp != nullptr;
    }

    bool Net::LaunchTcpServer(Api::iTcpServer* server, const char* ip, const int port, int max_ss, int max_rs) {
        Accepter* Accepter = Accepter::Create(server, ip, port, max_ss, max_rs);
        return Accepter != nullptr;
    }
}
