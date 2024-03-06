#include "Tcper.h"
#include "Net.h"

namespace XEngine {

    XPool<Tcper> s_TcperPool;

    Tcper::~Tcper() {
    }

    Tcper::Tcper(Api::iTcpSession* session, int socket, bool isActiveConnect)
        : _Session(session), _Socket(socket),
        _SendEx(eCompletion::Sended, this), _RecvEx(eCompletion::Recved, this),
        _IsRecving(false), _IsSending(false), _IsCached(false), _IsActiveConnect(isActiveConnect), _RemoteIP("unknown"), _RemotePort(-1) {

        _Net = dynamic_cast<Net*>(Net::GetInstance());

        SafeMemory::Memset(_RecvTemp, sizeof(_RecvTemp), 0, sizeof(_RecvTemp));
        XASSERT(_IsRecving == false && _IsSending == false && _IsCached == false, "wtf");
        _RecvEx._Wbuff.buf = _RecvTemp;
        _RecvEx._Wbuff.len = sizeof(_RecvTemp);
    }

    Tcper* Tcper::Create(Api::iTcpSession* session, const std::string& ip, const int port, int sock, bool isActiveConnect) {
        Tcper* pipe = XPOOL_CREATE(s_TcperPool, session, sock, isActiveConnect);

        if (isActiveConnect) {
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
                XPOOL_RELEASE(s_TcperPool, pipe);
                return nullptr;
            }
        }
        else {
            GetRemoteIpAndPort(sock, pipe->_RemoteIP, pipe->_RemotePort);
            if (false == pipe->AsyncRecv()) {
                SAFE_CLOSE_SOCKET(pipe->_Socket);
                XPOOL_RELEASE(s_TcperPool, pipe);
                return nullptr;
            }
        }

        return pipe;
    }

    void Tcper::OnCompleted(OVERLAPPEDEX* ex, const eCompletion type, const int code, const int size) {
        XASSERT(type == eCompletion::Recved || type == eCompletion::Sended || type == eCompletion::Connected, "Tcper on completer type error");
        switch (type) {
        case eCompletion::Connected: {
            XPOOL_RELEASE(g_OverlappedexPool, ex);
            DWORD ul = 1;
            if (ERROR_SUCCESS == code
                && SOCKET_ERROR != setsockopt(_Socket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, nullptr, 0)
                && SOCKET_ERROR != setsockopt(_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&ul, sizeof(ul))) {
                if (AsyncRecv()) {
                    Api::iTcpPipe* pipe = this;
                    GetRemoteIpAndPort(_Socket, _RemoteIP, _RemotePort);
                    SafeMemory::Memcpy((void*)&_Session->_Pipe, sizeof(_Session->_Pipe), &pipe, sizeof(pipe));
                    _Session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connected);
                    return;
                }
            }

            SAFE_CLOSE_SOCKET(_Socket);
            _Session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connectfaild);
            XPOOL_RELEASE(s_TcperPool, this);
            break;
        }
        case eCompletion::Recved: {
            _IsRecving = false;
            if (0 == code && 0 != size) {
                _RecvBuff.Write(_RecvTemp, size);
                if (!AsyncRecv()) {
                    Close();
                    return;
                }

                int use = 0;
                while (
                    _RecvBuff.Read(
                        [&](const void* const data, const UInt32 size) -> UInt32 {
                            return _Session->OnReceive(data, size);
                        }
                    )) {
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
                _SendBuff.ReadOut(size);
                _SendBuff.Read(
                    [&](const void* const data, const UInt32 len) ->UInt32 {
                        if (len > 0) {
                            this->AsyncSend();
                        }
                        return 0;
                    }
                );
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
        CloseSocket();
    }

    void Tcper::CloseSocket() {
        SAFE_CLOSE_SOCKET(_Socket);
        if (false == _IsSending && false == _IsRecving) {
            if (_Session) {
                if (_Session->_Pipe == this) {
                    SafeMemory::Memset((void*)&_Session->_Pipe, sizeof(_Session->_Pipe), 0, sizeof(_Session->_Pipe));
                }
                _Session->OnTcpSessionEvent(Api::eTcpSessionEvent::Disconnected);
            }
            XPOOL_RELEASE(s_TcperPool, this);
        }
    }

    void Tcper::Send(const void* data, const int size, bool immediately) {
        _SendBuff.Write(data, size);
        if (immediately) {
            AsyncSend();
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
    }

    const char* Tcper::RemoteIp() const {
        return _RemoteIP.c_str();
    }

    int Tcper::RemotePort() const {
        return _RemotePort;
    }

    bool Tcper::IsActiveConnect() const {
        return _IsActiveConnect;
    }

#define MAX_SEND_LEN 16384

    void Tcper::AsyncSend() {
        if (_IsSending) {
            return;
        }

        int err = -1;
        if (!_SendBuff.Read(
            [&](const void* const data, const UInt32 len) -> UInt32 {
                if (len > 0) {
                    SafeMemory::Memset(&_SendEx._Ol, sizeof(_SendEx._Ol), 0, sizeof(_SendEx._Ol));
                    _SendEx._Wbuff.buf = (char*)data;
                    _SendEx._Wbuff.len = len <= MAX_SEND_LEN ? len : MAX_SEND_LEN;
                    if (WSASend(_Socket, &_SendEx._Wbuff, 1, nullptr, 0, (LPWSAOVERLAPPED)&_SendEx, nullptr) != 0) {
                        err = GetLastError();
                        if (WSA_IO_PENDING != err) {
                            _IsSending = false;
                        }
                    }
                    _IsSending = true;
                }
                else {
                    _IsSending = false;
                }

                return 0;
            }
        )) {
        }

        if (err != WSA_IO_PENDING) {
            CloseSocket();
        }
    }
}
