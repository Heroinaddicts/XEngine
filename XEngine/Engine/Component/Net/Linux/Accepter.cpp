#include "Accepter.h"
#include "Tcper.h"

namespace XEngine {
    XPool<Accepter> g_AccepterPool;

    Accepter* Accepter::Create(iTcpServer* server, const std::string& listenIp, const UInt16 listenPort, const int size) {
        XASSERT(server != NULL, "wtf where is server");
        struct timeval tv;
        struct sockaddr_in addr;
        SafeMemory::Memset(&addr, sizeof(addr), 0, sizeof(addr));
        addr.sin_family = AF_INET;
        inet_pton(AF_INET, listenIp.c_str(), &addr.sin_addr);
        addr.sin_port = htons(listenPort);

        int sock = INVALID_FD;
        if (INVALID_FD == (sock = socket(AF_INET, SOCK_STREAM, 0))
            || ERROR_SOCKET == SetReuse(sock)
            || ERROR_SOCKET == SetNonBlocking(sock)
            || ERROR_SOCKET == bind(sock, (sockaddr*)&addr, sizeof(addr))
            || ERROR_SOCKET == listen(sock, 200)) {
            XERROR(g_Engine, "socket error %s", strerror(errno));
            CLOSE_SOCKET(sock);
            return nullptr;
        }

        Accepter* ac = XPOOL_CREATE(g_AccepterPool, sock, server, listenIp, listenPort, size);
        epoll_event ev;
        ev.data.ptr = (void*)&(ac->_Associat);
        ev.events = EPOLLIN;
        if (ERROR_SOCKET == epoll_ctl(g_EpollerFd, EPOLL_CTL_ADD, sock, &ev)) {
            XERROR(g_Engine, "epoll_ctl(g_EpollerFd, EPOLL_CTL_ADD, sock, &ev) error %s", strerror(errno));
            ac->Release();
            return nullptr;
        }

        Api::iAccepter* temp = dynamic_cast<Api::iAccepter*>(ac);
        SafeMemory::Memcpy((void*)&server->_Accepter, sizeof(void*), &temp, sizeof(temp));

        XLOG(g_Engine, "Accepter %s:%d is Created", listenIp.c_str(), listenPort);
        return ac;
    }

    void Accepter::OnCompleter(Associat* at, const eCompletion type, const struct epoll_event& ev) {
        XASSERT(type == eCompletion::doAccept, "Accepter on completer error");
        if (ev.events & EPOLLIN) {
            struct sockaddr_in addr;
            socklen_t len = sizeof(addr);
            Int64 sock = INVALID_FD;
            int time = 0;
            while ((sock = accept(_Socket, (sockaddr*)&addr, &len)) >= 0 && time++ <= 256) {
                if (ERROR_SOCKET != SetNonBlocking(sock)
                    || ERROR_SOCKET != SetTcpNoDelay(sock)
                    || ERROR_SOCKET != SetReuse(sock)
                    || ERROR_SOCKET != SetSockSendBuff(sock, 0)
                    || ERROR_SOCKET != SetSockRecvBuff(sock, 0)) {

                    struct sockaddr_in sa;
                    int len = sizeof(sa);
                    getpeername(sock, (struct sockaddr*)&sa, (socklen_t*)&len);
                    const std::string ip = inet_ntoa(sa.sin_addr);
                    const int port = ntohs(sa.sin_port);

                    Tcper* tcper = Tcper::Create(sock, ip, port, false, nullptr, _TcpBufferSize);
                    if (nullptr == tcper) {
                        CLOSE_SOCKET(sock);
                        continue;
                    }

                    NetEvent* netev = txnew NetEvent(eNetEventType::Accept);
                    netev->_Accepter = this;
                    netev->_Tcper = tcper;
                    CONNECTED_COUNT;
                    g_NetEventQueue.Push(netev);
                    continue;
                }
                else {
                    XERROR(g_Engine, "socket opt error %s", strerror(errno));
                    CLOSE_SOCKET(sock);
                }
            }
        }
        else {
            XERROR(g_Engine, "accept error %s", strerror(errno));
        }
    }

    void Accepter::Release() {
    }

    Accepter::Accepter(const int sock, iTcpServer* server, const std::string& listenIp, const UInt16 listenPort, const int size)
        : _Server(server), _Socket(sock), _ListenIP(listenIp), _ListenPort(listenPort), _Associat(eCompletion::doAccept, this), _TcpBufferSize(size) {

    }
}
