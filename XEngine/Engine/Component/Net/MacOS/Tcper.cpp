#include "Tcper.h"

namespace XEngine {
    XPool<Tcper> g_TcperPool;

    Tcper::Tcper(int socket, const std::string& ip, const UInt16 port, const bool active, const int size)
        : _Sock(socket), _IP(ip), _Port(port), _IsActiveConnect(active), _Associat(eCompletion::doIO, this), _IsClosed(false), _Session(nullptr), _SendBuffer(size, size), _RecvBuffer(size, size) {
    }

    Tcper* Tcper::Create(int socket, const std::string& host, const UInt16 port, bool active, Api::iTcpSession* const session, const int size) {
        Tcper* tcper = nullptr;
        if (active) {
            const int hostlen = host.size() + 1;
            char* temp = (char*)alloca(hostlen);
            SafeMemory::Memcpy(temp, hostlen, host.c_str(), hostlen);
            std::string ip;
            if (!GetIpByHost(host.c_str(), ip)) {
                session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connectfaild);
                return nullptr;
            }

            struct sockaddr_in addr;
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            int sock = INVALID_FD;
            if (INVALID_FD == (sock = ::socket(AF_INET, SOCK_STREAM, 0))
                || ERROR_SOCKET == SetNonBlocking(sock)
                || inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
                CLOSE_SOCKET(sock);
                session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connectfaild);
                return nullptr;
            }

            int ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
            if (SUCCESS == ret) {
                tcper = XPOOL_CREATE(g_TcperPool, sock, host, port, active, size);
                if (ERROR_SOCKET == AddKqueueRead(sock, (void*)&tcper->_Associat)) {
                    session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connectfaild);
                    CLOSE_SOCKET(sock);
                    XPOOL_RELEASE(g_TcperPool, tcper);
                    return nullptr;
                }

                tcper->CorrelateTcpSession(session);
                session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connected);
            }
            else if (ret < 0 && errno != EINPROGRESS) {
                CLOSE_SOCKET(sock);
                session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connectfaild);
                XPOOL_RELEASE(g_TcperPool, tcper);
                return nullptr;
            }
            else {
                tcper = XPOOL_CREATE(g_TcperPool, sock, host, port, active, size);
                Associat* at = XPOOL_CREATE(g_AssociatPool, eCompletion::doConnect, tcper);
                tcper->OnlySetTcpSession(session);
                if (ERROR_SOCKET == AddKqueueWrite(sock, (void*)at)) {
                    CLOSE_SOCKET(sock);
                    session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connectfaild);
                    XPOOL_RELEASE(g_AssociatPool, at);
                    XPOOL_RELEASE(g_TcperPool, tcper);
                    return nullptr;
                }
            }
        }
        else {
            tcper = XPOOL_CREATE(g_TcperPool, socket, host, port, active, size);
            if (ERROR_SOCKET == AddKqueueReadWrite(socket, (void*)&tcper->_Associat)) {
                XPOOL_RELEASE(g_TcperPool, tcper);
                XERROR(g_Engine, "AddKqueueReadWrite(g_KqueueFd, sock) error, %s", strerror(errno));
                return nullptr;
            }
        }

        return tcper;
    }

    void Tcper::Release(Tcper* tcper) {
        DeleteKqueueReadWrite(tcper->_Sock);
        CLOSE_SOCKET(tcper->_Sock);
        XPOOL_RELEASE(g_TcperPool, tcper);
    }

    void Tcper::ProcessData() {
        if (!_IsClosed) {
            XASSERT(_Session != nullptr, "Procee tcp data, but _Session is nullptr");
            Int64 tick = SafeSystem::Time::GetMicroSecond();
            if (nullptr != _Session) {
                while (!_IsClosed && _RecvBuffer.Read([&](const void* const data, const UInt32 size) {
                    return _Session->OnReceive(data, size);
                    })) {

                    if (SafeSystem::Time::GetMicroSecond() - tick > 1000) {
                        if (_RecvBuffer.GetDataSize() > 0) {
                            AUTO_LOCK(_Lock);
                            NetEvent* ev = txnew NetEvent(eNetEventType::Recv);
                            ev->_Tcper = this;
                            g_NetEventQueue.Push(ev);
                        }
                        break;
                    }
                }
            }
        }
    }

    void Tcper::OnCompleter(Associat* at, const eCompletion type, const struct kevent& kev)
    {
        if (_IsClosed)
            return;

        switch (type) {
        case eCompletion::doConnect: {
            XPOOL_RELEASE(g_AssociatPool, at);
            socklen_t len = sizeof(int);
            DeleteKqueueEvent(_Sock, EVFILT_WRITE);
            if ((kev.flags & (EV_ERROR | EV_EOF)) || ERROR_SOCKET == SetTcpNoDelay(_Sock)) {
                _IsClosed = true;
                NetEvent* ev = txnew NetEvent(eNetEventType::Connect);
                getsockopt(_Sock, SOL_SOCKET, SO_ERROR, &ev->_Code, &len);
                ev->_Code == 0 ? ev->_Code = -1 : ev->_Code = ev->_Code;
                ev->_Tcper = this;
                g_NetEventQueue.Push(ev);
                CLOSE_SOCKET(_Sock);
                return;
            }

            if (kev.filter == EVFILT_WRITE) {
                NetEvent* ev = txnew NetEvent(eNetEventType::Connect);
                ev->_Tcper = this;

                if (ERROR_SOCKET == AddKqueueReadWrite(_Sock, (void*)&_Associat)) {
                    _IsClosed = true;
                    getsockopt(_Sock, SOL_SOCKET, SO_ERROR, &ev->_Code, &len);
                    ev->_Code == 0 ? ev->_Code = -1 : ev->_Code = ev->_Code;
                    CLOSE_SOCKET(_Sock);
                }

                g_NetEventQueue.Push(ev);
            }
            else {
                _IsClosed = true;
                NetEvent* ev = txnew NetEvent(eNetEventType::Connect);
                getsockopt(_Sock, SOL_SOCKET, SO_ERROR, &ev->_Code, &len);
                ev->_Code == 0 ? ev->_Code = -1 : ev->_Code = ev->_Code;
                ev->_Tcper = this;
                g_NetEventQueue.Push(ev);
                CLOSE_SOCKET(_Sock);
            }
            break;
        }
        case eCompletion::doIO: {
            if (kev.flags & (EV_ERROR | EV_EOF)) {
                {
                    AUTO_LOCK(_Lock);
                    if (!_IsClosed) {
                        DeleteKqueueReadWrite(_Sock);
                        CLOSE_SOCKET(_Sock);
                        _IsClosed = true;
                        NetEvent* ev = txnew NetEvent(eNetEventType::Disconnect);
                        ev->_Tcper = this;
                        g_NetEventQueue.Push(ev);
                        DISCONNECTED_COUNT;
                    }
                }
            }
            else {
                if (kev.filter == EVFILT_READ) {
                    bool isRecv = false, isDisconnect = false;
                    int len = 0;
                    do {
                        len = recv(_Sock, _RecvTemp, sizeof(_RecvTemp), 0);
                        if (len < 0 && (EAGAIN == errno || EWOULDBLOCK == errno)) {
                            break;
                        }

                        if (len > 0) {
                            _RecvBuffer.Write(_RecvTemp, len, __FILE__, __LINE__);
                            isRecv = true;
                        }

                        if (len == 0) {
                            isDisconnect = true;
                        }

                    } while (len > 0);

                    if (isRecv && !isDisconnect) {
                        AUTO_LOCK(_Lock);
                        NetEvent* ev = txnew NetEvent(eNetEventType::Recv);
                        ev->_Tcper = this;
                        g_NetEventQueue.Push(ev);
                    }

                    if (isDisconnect) {
                        AUTO_LOCK(_Lock);
                        if (!_IsClosed) {
                            _IsClosed = true;
                            DeleteKqueueReadWrite(_Sock);
                            NetEvent* ev = txnew NetEvent(eNetEventType::Disconnect);
                            DISCONNECTED_COUNT;
                            ev->_Tcper = this;
                            g_NetEventQueue.Push(ev);
                        }
                    }
                }

                if (!_IsClosed && kev.filter == EVFILT_WRITE) {
                    if (_SendBuffer.GetDataSize() > 0) {
                        _SendBuffer.Read([&](const void* const data, const UInt32 size) {
                            int len = ::send(_Sock, data, size, 0);
                            return len;
                            });
                    }
                }
            }
            break;
        }
        }
    }

    void Tcper::Pause() {

    }

    void Tcper::Resume() {

    }

    void Tcper::Close() {
        AUTO_LOCK(_Lock);
        if (!_IsClosed) {
            _Session->OnTcpSessionEvent(Api::eTcpSessionEvent::Disconnected);
            CorrelateTcpSession(nullptr);
            DeleteKqueueReadWrite(_Sock);
            CLOSE_SOCKET(_Sock);
            _IsClosed = true;
            NetRequest* req = txnew NetRequest(eNetRequestType::CloseTcper);
            req->_Tcper = this;

            DISCONNECTED_COUNT;
            g_NetRequestQueue.Push(req);
        }
    }

    void Tcper::Send(const void* data, const int size, bool immediately) {
        if (!_IsClosed)
            _SendBuffer.Write(data, size, __FILE__, __LINE__);
    }

    std::string Tcper::RemoteIp() const {
        return _IP;
    }

    int Tcper::RemotePort() const {
        return _Port;
    }

    bool Tcper::IsActiveConnect() const {
        return _IsActiveConnect;
    }
}
