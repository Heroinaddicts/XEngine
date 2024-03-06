#include "Accepter.h"
#include "Tcper.h"

namespace XEngine {
    XPool<Accepter> g_AccepterPool;
    
    Accepter * Accepter::Create(iTcpServer * server, const char * ip, const int port, int s_size, int r_size) {
        XASSERT(server != NULL, "wtf where is server");
        struct timeval tv;
        struct sockaddr_in addr;
        SafeMemory::Memset(&addr, sizeof (addr), 0, sizeof (addr));
        addr.sin_family = AF_INET;
        inet_pton(AF_INET, ip, &addr.sin_addr);
        addr.sin_port = htons(port);

        int sock = INVALID_FD;
        if (INVALID_FD == (sock = socket(AF_INET, SOCK_STREAM, 0))
                || ERROR_SOCKET == SetReuse(sock)
                || ERROR_SOCKET == SetNonBlocking(sock)
                || ERROR_SOCKET == bind(sock, (sockaddr *) & addr, sizeof (addr))
                || ERROR_SOCKET == listen(sock, 200)) {
            //error(g_core, "socket error %s", strerror(errno));
            CLOSE_SOCKET(sock);
            return nullptr;
        }

        Accepter * ac = XPOOL_CREATE(g_AccepterPool, server, ip, port, s_size, r_size);
        ac->_Socket = sock;
        epoll_event ev;
        ev.data.ptr = (void *) &(ac->_Associat);
        ev.events = EPOLLIN;
        if (ERROR_SOCKET == epoll_ctl(g_EpollerFd, EPOLL_CTL_ADD, sock, &ev)) {
            ac->Release();
            //error(g_core, "epoll_ctl error %s", strerror(errno));
            return nullptr;
        }

        Api::iAccepter * temp = dynamic_cast<Api::iAccepter*>(ac);
        SafeMemory::Memcpy((void *)&server->_Accepter, sizeof(void *), &temp, sizeof(temp));
        return ac;
    }

    void Accepter::OnCompleter(Associat * at, const eCompletion type, const struct epoll_event & ev) {
        XASSERT(type == eCompletion::doAccept, "Accepter on completer error");
        if (ev.events & EPOLLIN) {
            struct sockaddr_in addr;
            socklen_t len = sizeof (addr);
            int64 sock = INVALID_FD;
            int time = 0;
            while ((sock = accept(_Socket, (sockaddr *) & addr, &len)) >= 0 && time++ <= 256) {
                if (ERROR_SOCKET != SetNonBlocking(sock)
                        || ERROR_SOCKET != SetTcpNoDelay(sock)
                        || ERROR_SOCKET != SetReuse(sock)
                        || ERROR_SOCKET != SetSockSendBuff(sock, 0)
                        || ERROR_SOCKET != SetSockRecvBuff(sock, 0)) {

                    struct sockaddr_in sa;
                    int len = sizeof (sa);
                    getpeername(sock, (struct sockaddr *) &sa, (socklen_t *) & len);
                    const std::string ip = inet_ntoa(sa.sin_addr);
                    const int port = ntohs(sa.sin_port);
                    //debug(g_core, "new client %s:%d connected from prot %d", ip.c_str(), port, _address._port);

                    iTcpSession * session = _Server->OnMallocConnection(ip.c_str(), port);
                    if (!session) {
                        _Server->OnError(session);
                        CLOSE_SOCKET(sock);
                        //error(g_core, "server %x malloc a null session point", _Server);
                        continue;
                    }

                    // session->_address._ip = ip;
                    // session->_address._port = port;
                    
                    Tcper * tcp = XPOOL_CREATE(g_TcperPool, session, ip, port, _SendSize, _RecvSize);
                    struct epoll_event ev;
                    ev.data.ptr = (void *) &tcp->_Associat;
                    ev.events = EPOLLIN;
                    if (ERROR_SOCKET == epoll_ctl(g_EpollerFd, EPOLL_CTL_ADD, sock, &ev)) {
                        _Server->OnError(session);
                        //error(g_core, "epoll_ctl error %s", strerror(errno));
                        CLOSE_SOCKET(sock);
                        XPOOL_RELEASE(g_TcperPool, tcp);
                        continue;
                    }

                    tcp->_Connected = true;
                    tcp->_Socket = sock;
                    
                    iTcpPipe * pipe = dynamic_cast<iTcpPipe*>(tcp);
                    SafeMemory::Memcpy((void *)&session->_Pipe, sizeof(void *), &pipe, sizeof(void *));
                    session->OnConnected();
                    continue;
                }
                
                //error(g_core, "socket opt error %s", strerror(errno));
                CLOSE_SOCKET(sock);
            }
        } else {
            //error(g_core, "accept error %s", strerror(errno));
        }
    }

    void Accepter::Release() {
        epoll_ctl(g_EpollerFd, EPOLL_CTL_DEL, _Socket, nullptr);
        CLOSE_SOCKET(_Socket);
        _Server->OnRelease();
        XPOOL_RELEASE(g_AccepterPool, this);
    }

    Accepter::Accepter(iTcpServer * server, const char * ip, const int port, int s_size, int r_size) 
    : _Server(server), _Socket(INVALID_FD), _SendSize(s_size <= MAX_PIPE_SIZE ? s_size : MAX_PIPE_SIZE), _RecvSize(r_size <= MAX_PIPE_SIZE ? r_size : MAX_PIPE_SIZE), _Associat(eCompletion::doAccept, this) {
        
    }
}
