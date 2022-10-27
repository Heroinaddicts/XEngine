#include "Tcper.h"
#include "Net.h"

namespace XEngine {

    XPool<Tcper> static_tcper_pool;

    Tcper::Tcper(Api::iTcpSession* session, const int sendsize, const int recvsize, int socket)
        : _Session(session), _SendBuff(sendsize), _RecvBuff(recvsize), _Socket(socket),
        _SendEx(eCompletion::Sended, this), _RecvEx(eCompletion::Recved, this),
        _IsRecving(false), _IsSending(false), _IsCached(false) {

        _Net = dynamic_cast<Net*>(Net::GetInstance());

        SafeMemory::Memset(_RecvTemp, sizeof(_RecvTemp), 0, sizeof(_RecvTemp));
        XASSERT(_IsRecving == false && _IsSending == false && _IsCached == false, "wtf");
        _RecvEx._Wbuff.buf = _RecvTemp;
        _RecvEx._Wbuff.len = sizeof(_RecvTemp);
    }

    Tcper* Tcper::Create(Api::iTcpSession* session, const std::string& ip, const int port, const int sendsize, const int recvsize, int sock, bool initiative) {
        Tcper* pipe = XPOOL_CREATE(static_tcper_pool, session, sendsize, recvsize, sock);

        if (initiative) {
            static LPFN_CONNECTEX connectex = GetConnectExFun();
            OVERLAPPEDEX* ex = XPOOL_CREATE(g_OverlappedexPool, eCompletion::Connected, pipe);
            SafeMemory::Memset(&ex->_Ol, sizeof(ex->_Ol), 0, sizeof(ex->_Ol));
            SafeMemory::Memset(&ex->_Remote, sizeof(ex->_Remote), 0, sizeof(ex->_Remote));
            ex->_Remote.sin_addr.s_addr = inet_addr(ip.c_str());
            ex->_Remote.sin_port = htons(port);
            ex->_Remote.sin_family = AF_INET;
            ex->_Socket = sock;
            DWORD bytes = 0;
            int res = connectex(pipe->_Socket, (struct sockaddr*)&ex->_Remote, sizeof(struct sockaddr_in), nullptr, 0, &bytes, (LPOVERLAPPED)ex);
            int err = GetLastError();
            if (SOCKET_ERROR == res && err != WSA_IO_PENDING) {
                XPOOL_RELEASE(g_OverlappedexPool, ex);
                XPOOL_RELEASE(static_tcper_pool, pipe);
                return nullptr;
            }
        }
        else {
            if (false == pipe->AsyncRecv()) {
                SAFE_CLOSE_SOCKET(pipe->_Socket);
                XPOOL_RELEASE(static_tcper_pool, pipe);
                return nullptr;
            }
        }

        return pipe;
    }

    void Tcper::OnCompleted(OVERLAPPEDEX* ex, const eCompletion type, const int code, const int size) {
        XASSERT(type == eCompletion::Recved || type == eCompletion::Sended || type == eCompletion::Connected, "tcper on completer type error");
        switch (type) {
        case eCompletion::Connected: {
            XPOOL_RELEASE(g_OverlappedexPool, ex);
            DWORD ul = 1;
            if (ERROR_SUCCESS == code
                && SOCKET_ERROR != setsockopt(_Socket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, nullptr, 0)
                && SOCKET_ERROR != setsockopt(_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&ul, sizeof(ul))) {
                if (AsyncRecv()) {
                    Api::iTcpPipe* pipe = this;
                    SafeMemory::Memcpy((void*)&_Session->_Pipe, sizeof(_Session->_Pipe), &pipe, sizeof(pipe));
                    _Session->OnConnected();
                    return;
                }
            }

            SAFE_CLOSE_SOCKET(_Socket);
            _Session->OnConnectFailed();
            XPOOL_RELEASE(static_tcper_pool, this);
            break;
        }
        case eCompletion::Recved: {
            _IsRecving = false;
            if (0 == code && 0 != size) {
                if (_RecvBuff.In(_RecvTemp, size)) {
                    if (!AsyncRecv()) {
                        Close();
                        return;
                    }

                    int use = 0;
                    while (_RecvBuff.Length() > 0 && (use = _Session->OnReceive((const char*)_RecvBuff.GetData(), _RecvBuff.Length())) > 0) {
                        _RecvBuff.Out(use);
                        if (INVALID_SOCKET == _Socket) {
                            return;
                        }
                    }
                }
                else {
                    Close();
                }
            }
            else {
                Close();
            }
            break;
        }
        case eCompletion::Sended: {
            _IsSending = false;
            if (0 == code) {
                if (!_SendBuff.Out(size)) {
                    XASSERT(false, "send buff out size %d error", size);
                    Close();
                    return;
                }

                if (_SendBuff.Length() != 0) {
                    _Net->TcpNeedSend(this);
                }
            }
            else {
                Close();
            }
            break;
        }
        default:
            break;
        }
    }

    void Tcper::Pause() {

    }

    void Tcper::Resume() {

    }

    void Tcper::Close() {
        _Net->TcpNoNeedSend(this);
        SAFE_CLOSE_SOCKET(_Socket);
        if (false == _IsSending && false == _IsRecving) {
            SafeMemory::Memset((void*)&_Session->_Pipe, sizeof(_Session->_Pipe), 0, sizeof(_Session->_Pipe));
            if (_Session) {
                _Session->OnDisconnect();
            }
            XPOOL_RELEASE(static_tcper_pool, this);
        }
    }

    void Tcper::Send(const void* data, const int size, bool immediately) {
        if (!_SendBuff.In(data, size)) {
            Close();
            return;
        }
        if (immediately) {
            _Net->TcpNeedSend(this);
        }
    }

    bool Tcper::AsyncRecv() {
        XASSERT(false == _IsRecving, "wtf");
        SetLastError(0);
        DWORD bytes, flag = 0;
        SafeMemory::Memset(&_RecvEx._Ol, sizeof(_RecvEx._Ol), 0, sizeof(_RecvEx._Ol));
        int res = WSARecv(_Socket, &_RecvEx._Wbuff, 1, &bytes, &flag, (LPWSAOVERLAPPED)&_RecvEx, nullptr);
        int err = GetLastError();
        if (SOCKET_ERROR == res && ERROR_IO_PENDING != err) {
            return false;
        }
        _IsRecving = true;
        return true;
        return false;
    }

    bool Tcper::AsyncSend() {
        if (_IsSending) {
            return true;
        }

        if (_SendBuff.Length() == 0) {
            return _IsRecving;
        }

        SafeMemory::Memset(&_SendEx._Ol, sizeof(_SendEx._Ol), 0, sizeof(_SendEx._Ol));
        _SendEx._Wbuff.buf = (char*)_SendBuff.GetData();
        _SendEx._Wbuff.len = _SendBuff.Length();

        int res = WSASend(_Socket, &_SendEx._Wbuff, 1, nullptr, 0, (LPWSAOVERLAPPED)&_SendEx, nullptr);
        int err = GetLastError();
        if (SOCKET_ERROR == res && WSA_IO_PENDING != err) {
            return false;
        }

        _IsSending = true;
        return true;
    }
}
