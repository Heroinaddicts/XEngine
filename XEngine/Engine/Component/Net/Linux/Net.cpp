#include "Net.h"
#include "Tcper.h"
#include "Accepter.h"
#include "SafeSystem.h"

namespace XEngine {
    int g_EpollerFd = INVALID_FD;
    XPool<Associat> g_AssociatPool;
    SafeQueue::SpscQueue<NetEvent*> g_NetEventQueue;
    SafeQueue::SpscQueue<NetRequest*> g_NetRequestQueue;


    int g_ConnectedCount = 0;
    int g_DisconnectedCount = 0;
    SpinLock g_CountLock;

    iNet* Net::GetInstance() {
        static Net* s_Instance = nullptr;
        if (nullptr == s_Instance) {
            s_Instance = txnew Net();
        }
        return s_Instance;
    }

    bool Net::Initialize(Api::iEngine* const engine) {
        if (INVALID_FD == (g_EpollerFd = epoll_create(EPOLLER_DESC_COUNT))) {
            XERROR(g_Engine, "create epoller fd error %s", strerror(errno));
            return false;
        }

        Start(1);
        return true;
    }

    bool Net::Launch(Api::iEngine* const engine) {
        return true;
    }

    void Net::Release(Api::iEngine* const engine) {
    }

    void Net::EarlyUpdate(Api::iEngine* const engine) {
        NetEvent* netev = nullptr;

        Int64 tick = SafeSystem::Time::GetMicroSecond();
        while (g_NetEventQueue.Pull(OUT netev)) {
            switch (netev->_Type) {
            case eNetEventType::Accept: {
                iTcpSession* session = netev->_Accepter->_Server->OnMallocTcpSession(netev->_IP, netev->_Port);
                XASSERT(session != nullptr, "OnMallocTcpSession return nullptr?????");
                if (nullptr != session) {
                    netev->_Tcper->CorrelateTcpSession(session);
                    session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connected);
                    //XLOG(g_Engine, "Tcper %lld set session", netev->_Tcper);
                }
                else {
                    NetRequest* req = txnew NetRequest(eNetRequestType::ReleaseTcper);
                    req->_Tcper = netev->_Tcper;
                }
                break;
            }
            case eNetEventType::Recv: {
                netev->_Tcper->ProcessData();
                break;
            }
            case eNetEventType::Connect: {
                if (netev->_Code == 0) {
                    netev->_Tcper->CorrelateTcpSession(netev->_Tcper->GetTcpSession());
                    netev->_Tcper->GetTcpSession()->OnTcpSessionEvent(Api::eTcpSessionEvent::Connected);
                }
                else {
                    XLOG(g_Engine, "Connect error %d", netev->_Code);
                    netev->_Tcper->GetTcpSession()->OnTcpSessionEvent(Api::eTcpSessionEvent::Connectfaild);
                    NetRequest* req = txnew NetRequest(eNetRequestType::ReleaseTcper);
                    req->_Tcper = netev->_Tcper;
                    g_NetRequestQueue.Push(req);
                }
                break;
            }
            case eNetEventType::Disconnect: {
                //XLOG(g_Engine, "Tcper %lld Disconnect", netev->_Tcper);
                Api::iTcpSession* session = netev->_Tcper->GetTcpSession();
                netev->_Tcper->CorrelateTcpSession(nullptr);
                if (session) {
                    session->OnTcpSessionEvent(Api::eTcpSessionEvent::Disconnected);
                }

                NetRequest* req = txnew NetRequest(eNetRequestType::ReleaseTcper);
                req->_Tcper = netev->_Tcper;
                g_NetRequestQueue.Push(req);
                break;
            }
            default:
                break;
            }
            txdel netev;

            if (SafeSystem::Time::GetMicroSecond() - tick > 5000) {
                break;
            }
        }
    }

    void Net::Update(Api::iEngine* const engine) {
    }

    void Net::LaterUpdate(Api::iEngine* const engine) {

    }

    bool Net::LaunchTcpSession(iTcpSession* session, const std::string& host, const UInt16 port, const int size) {
        return nullptr != Tcper::Create(-1, host, port, true, session, size);
    }

    bool Net::LaunchTcpServer(iTcpServer* server, const std::string& listenIp, const UInt16 listenPort, const int size) {
        return nullptr != Accepter::Create(server, listenIp, listenPort, size);
    }

    void Net::Run(void* context) {
        static struct epoll_event evs[EPOLLER_MAX_EVENT_COUNT];
        while (!_IsClose) {
            Int64 tick = SafeSystem::Time::GetMicroSecond();
            errno = 0;
            int count = 0;
            if ((count = epoll_wait(g_EpollerFd, evs, EPOLLER_MAX_EVENT_COUNT, -1)) > 0) {
                for (int i = 0; i < count; i++) {
                    struct Associat* p = (struct Associat*)evs[i].data.ptr;
                    p->_Completer->OnCompleter(p, p->_Ev, evs[i]);
                }
            }

            NetRequest* req = nullptr;
            while (g_NetRequestQueue.Pull(OUT req)) {
                switch (req->_Type) {
                case eNetRequestType::ReleaseTcper: {
                    Tcper::Release(req->_Tcper);
                    break;
                }
                case eNetRequestType::CloseTcper: {
                    NetEvent* ev = txnew NetEvent(eNetEventType::Disconnect);
                    ev->_Tcper = req->_Tcper;
                    g_NetEventQueue.Push(ev);
                    break;
                }
                default:
                    break;
                }

                txdel req;
            }
        }
    }
}
