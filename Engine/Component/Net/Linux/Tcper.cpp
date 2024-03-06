#include "Tcper.h"

namespace XEngine {
    XPool<Tcper> g_TcperPool;

    Tcper::~Tcper() {
        //g_TcperNeedSend.erase(this);
    }

    Tcper* Tcper::Create(Api::iTcpSession* session, const std::string& host, const int port, int ssize, int rsize) {
        // (ssize > MAX_PIPE_SIZE) ? (ssize = MAX_PIPE_SIZE) : void(0);
        // (rsize > MAX_PIPE_SIZE) ? (rsize = MAX_PIPE_SIZE) : void(0);

        const int hostlen = host.size() + 1;
        char* temp = (char*)alloca(hostlen);
        SafeMemory::Memcpy(temp, hostlen, host.c_str(), hostlen);

        static const bool b = true;
        SafeMemory::Memcpy((void*)&session->_Initiative, sizeof(bool), (void*)&b, sizeof(bool));

        std::string ip;
        if (!GetIpByHost(host.c_str(), ip)) {
            session->OnConnectFailed();
            return nullptr;
        }

        // session->_Address._ip = ip;
        // session->_Address._port = port;

        struct timeval tv;
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        int sock = INVALID_FD;
        if (INVALID_FD == (sock = socket(AF_INET, SOCK_STREAM, 0))
            || ERROR_SOCKET == SetNonBlocking(sock)
            || inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
            //error("socket error %s, sock %lld, %s:%d", strerror(errno), sock, ip.c_str(), port);
            CLOSE_SOCKET(sock);
            session->OnConnectFailed();
            return nullptr;
        }

        int ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        Tcper* p = XPOOL_CREATE(g_TcperPool, session, ip, port, ssize, rsize);
        p->_Socket = sock;
        if (SUCCESS == ret) {
            struct epoll_event ev;
            ev.data.ptr = (void*)&p->_Associat;
            ev.events = EPOLLIN;
            if (ERROR_SOCKET == epoll_ctl(g_EpollerFd, EPOLL_CTL_ADD, sock, &ev)) {
                //error("epoll_ctl error %s", strerror(errno));
                session->OnConnectFailed();
                CLOSE_SOCKET(sock);
                XPOOL_RELEASE(g_TcperPool, p);
                return nullptr;
            }

            p->_Connected = true;
            SafeMemory::Memcpy((void*)&session->_Pipe, sizeof(void*), &p, sizeof(p));
            session->OnConnected();
        }
        else if (ret < 0 && errno != EINPROGRESS) {
            CLOSE_SOCKET(sock);
            session->OnConnectFailed();
            XPOOL_RELEASE(g_TcperPool, p);
            return nullptr;
        }
        else {
            Associat* at = XPOOL_CREATE(g_AssociatPool, eCompletion::doConnect, p);
            struct epoll_event ev;
            ev.data.ptr = (void*)at;
            ev.events = EPOLLOUT;
            if (ERROR_SOCKET == epoll_ctl(g_EpollerFd, EPOLL_CTL_ADD, sock, &ev)) {
                //error("epoll_ctl error %s", strerror(errno));
                session->OnConnectFailed();
                CLOSE_SOCKET(sock);
                XPOOL_RELEASE(g_TcperPool, p);
                return nullptr;
            }
        }

        return p;
    }

    Tcper::Tcper(Api::iTcpSession* session, const std::string& ip, const int port, const int ssize, const int rsize)
        : _Session(session), _Connected(false), _Caching(false), _SendBuff(ssize), _RecvBuff(rsize), _Associat(eCompletion::doIO, this), _Socket(INVALID_FD)
    {

    }

    void Tcper::OnCompleter(Associat* at, const eCompletion type, const struct epoll_event& ev) {
        switch (type) {
        case eCompletion::doConnect: {
            //Associat * at = (Associat *)ev.data.ptr;
            XPOOL_RELEASE(g_AssociatPool, at);

            if (ERROR_SOCKET == SetTcpNoDelay(_Socket)) {
                CLOSE_SOCKET(_Socket);
                _Session->OnConnectFailed();
                XPOOL_RELEASE(g_TcperPool, this);
                return;
            }

            epoll_ctl(g_EpollerFd, EPOLL_CTL_DEL, _Socket, nullptr);
            if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                CLOSE_SOCKET(_Socket);
                _Session->OnConnectFailed();
                XPOOL_RELEASE(g_TcperPool, this);
                return;
            }

            if (ev.events & EPOLLOUT) {
                struct epoll_event event;
                event.data.ptr = (void*)&_Associat;
                event.events = EPOLLIN;
                if (ERROR_SOCKET == epoll_ctl(g_EpollerFd, EPOLL_CTL_ADD, _Socket, &event)) {
                    //error("epoll ctl add error %s", strerror(errno));
                    CLOSE_SOCKET(_Socket);
                    _Session->OnConnectFailed();
                    XPOOL_RELEASE(g_TcperPool, this);
                    return;
                }

                iPipe* p = this;
                SafeMemory::Memcpy((void*)&_Session->_Pipe, sizeof(void*), &p, sizeof(void*));
                _Connected = true;
                _Session->OnConnected();
            }
            else {
                CLOSE_SOCKET(_Socket);
                _Session->OnConnectFailed();
                XPOOL_RELEASE(g_TcperPool, this);
            }
            break;
        }
        case eCompletion::doIO: {
            if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                //error("eCompletion::doIO error %s", strerror(errno));
                CLOSE_SOCKET(_Socket);
                _Connected = false;
                SafeMemory::Memset((void*)&_Session->_Pipe, sizeof(void*), 0, sizeof(void*));
                _Session->OnDisconnect();
                g_TcperNeedSend.erase(this);
                XPOOL_RELEASE(g_TcperPool, this);
            }
            else {
                if (ev.events & EPOLLIN) {
                    static char temp[RECV_TEMP_SIZE];
                    XASSERT(_Connected, "wtf");
                    if (_Connected) {
                        const int64 tick = SafeSystem::Time::GetMicroSecond();
                        do {
                            int len = recv(_Socket, temp, sizeof(temp), 0);
                            SetTcpQuickAck(_Socket);
                            if (len < 0 && EAGAIN == errno) {
                                return;
                            }
                            else if (len > 0) {
                                //debug("pipe recv data size %d", len);
                                if (_RecvBuff.In(temp, len)) {
                                    while (_RecvBuff.Length() > 0 && !_Caching) {
                                        int use = _Session->OnReceive((const char*)_RecvBuff.GetData(), _RecvBuff.Length());
                                        if (!_Connected) {
                                            epoll_ctl(g_EpollerFd, EPOLL_CTL_DEL, _Socket, nullptr);
                                            CLOSE_SOCKET(_Socket);
                                            if (_Session) {
                                                SafeMemory::Memset((void*)&_Session->_Pipe, sizeof(void*), 0, sizeof(void*));
                                                _Session->OnDisconnect();
                                                g_TcperNeedSend.erase(this);
                                                XPOOL_RELEASE(g_TcperPool, this);
                                            }
                                            return;
                                        }

                                        if (use <= 0) {
                                            return;
                                        }
                                        _RecvBuff.Out(use);
                                    }
                                }
                                else {
                                    Close();
                                    return;
                                }
                            }
                            else {
                                Close();
                                return;
                            }
                        } while (_Connected && SafeSystem::Time::GetMicroSecond() - tick <= 1000);
                    }
                }

                if (ev.events & EPOLLOUT) {
                    g_TcperNeedSend.insert(this);
                }
            }

            break;
        }
        }
    }

    void Tcper::Close() {
        if (_Socket != INVALID_FD) {
            _Connected = false;
            epoll_ctl(g_EpollerFd, EPOLL_CTL_DEL, _Socket, nullptr);
            CLOSE_SOCKET(_Socket);
            if (_Session) {
                SafeMemory::Memset((void*)&_Session->_Pipe, sizeof(void*), 0, sizeof(void*));
                _Session->OnDisconnect();
                g_TcperNeedSend.erase(this);
                SafeMemory::Memset((void*)&_Session, sizeof(void*), 0, sizeof(void*));
            }

            XPOOL_RELEASE(g_TcperPool, this);
        }
    }

    void Tcper::Send(const void* data, const int size, bool immediately) {
        if (!_Connected) {
            return;
        }

        if (_SendBuff.In(data, size)) {
            g_TcperNeedSend.insert(this);
        }
        else {
            Close();
        }
    }

    void Tcper::DoSend() {
        XASSERT(_Connected, "wtf");
        if (_SendBuff.Length() > 0) {
            int len = ::send(_Socket, _SendBuff.GetData(), _SendBuff.Length(), 0);
            SetTcpNoDelay(_Socket);
            if (len > 0) {
                _SendBuff.Out(len);
                if (_SendBuff.Length() == 0) {
                    struct epoll_event ev;
                    ev.data.ptr = (void*)&_Associat;
                    ev.events = EPOLLIN;
                    if (ERROR_SOCKET == epoll_ctl(g_EpollerFd, EPOLL_CTL_MOD, _Socket, &ev)) {
                        epoll_ctl(g_EpollerFd, EPOLL_CTL_DEL, _Socket, nullptr);
                        CLOSE_SOCKET(_Socket);
                        SafeMemory::Memset((void*)&_Session->_Pipe, sizeof(void*), 0, sizeof(void*));
                        _Session->OnDisconnect();
                        SafeMemory::Memset((void*)&_Session, sizeof(void*), 0, sizeof(void*));
                        XPOOL_RELEASE(g_TcperPool, this);
                        return;
                    }
                }
            }
            else if (len <= 0 && EAGAIN != errno) {
                epoll_ctl(g_EpollerFd, EPOLL_CTL_DEL, _Socket, nullptr);
                CLOSE_SOCKET(_Socket);
                SafeMemory::Memset((void*)&_Session->_Pipe, sizeof(void*), 0, sizeof(void*));
                _Session->OnDisconnect();
                XPOOL_RELEASE(g_TcperPool, this);
            }
        }
    }
}
