#include "Tcper.h"
#include "Net.h"

namespace XEngine {

    XPool<Tcper> static_tcper_pool;

    Tcper::Tcper(Api::iTcpSession* session, const int sendsize, const int recvsize, int socket)
        : _session(session), _send_buff(sendsize), _recv_buff(recvsize), _socket(socket),
        _send_ex(eCompletion::sended, this), _recv_ex(eCompletion::recved, this),
        _recving(false), _sending(false), _caching(false) {

        _net = dynamic_cast<Net*>(Net::GetInstance());

        SafeMemory::Memset(_recv_temp, sizeof(_recv_temp), 0, sizeof(_recv_temp));
        XASSERT(_recving == false && _sending == false && _caching == false, "wtf");
        _recv_ex._wbuf.buf = _recv_temp;
        _recv_ex._wbuf.len = sizeof(_recv_temp);
    }

    Tcper* Tcper::Create(Api::iTcpSession* session, const std::string& ip, const int port, const int sendsize, const int recvsize, int sock, bool initiative) {
        Tcper* pipe = XPOOL_CREATE(static_tcper_pool, session, sendsize, recvsize, sock);

        if (initiative) {
            static LPFN_CONNECTEX connectex = GetConnectExFun();
            OVERLAPPEDEX* ex = XPOOL_CREATE(g_overlappedex_pool, eCompletion::connected, pipe);
            SafeMemory::Memset(&ex->_ol, sizeof(ex->_ol), 0, sizeof(ex->_ol));
            SafeMemory::Memset(&ex->_remote, sizeof(ex->_remote), 0, sizeof(ex->_remote));
            ex->_remote.sin_addr.s_addr = inet_addr(ip.c_str());
            ex->_remote.sin_port = htons(port);
            ex->_remote.sin_family = AF_INET;
            ex->_socket = sock;
            DWORD bytes = 0;
            int res = connectex(pipe->_socket, (struct sockaddr*)&ex->_remote, sizeof(struct sockaddr_in), nullptr, 0, &bytes, (LPOVERLAPPED)ex);
            int err = GetLastError();
            if (SOCKET_ERROR == res && err != WSA_IO_PENDING) {
                XPOOL_RELEASE(g_overlappedex_pool, ex);
                XPOOL_RELEASE(static_tcper_pool, pipe);
                return nullptr;
            }
        }
        else {
            if (false == pipe->AsyncRecv()) {
                SAFE_CLOSE_SOCKET(pipe->_socket);
                XPOOL_RELEASE(static_tcper_pool, pipe);
                return nullptr;
            }
        }

        return pipe;
    }

    void Tcper::OnCompleted(OVERLAPPEDEX* ex, const eCompletion type, const int code, const int size) {
        XASSERT(type == eCompletion::recved || type == eCompletion::sended || type == eCompletion::connected, "tcper on completer type error");
        switch (type) {
        case eCompletion::connected: {
            XPOOL_RELEASE(g_overlappedex_pool, ex);
            DWORD ul = 1;
            if (ERROR_SUCCESS == code
                && SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, nullptr, 0)
                && SOCKET_ERROR != setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&ul, sizeof(ul))) {
                if (AsyncRecv()) {
                    Api::iTcpPipe* pipe = this;
                    SafeMemory::Memcpy((void*)&_session->_pipe, sizeof(_session->_pipe), &pipe, sizeof(pipe));
                    _session->OnConnected();
                    return;
                }
            }

            SAFE_CLOSE_SOCKET(_socket);
            _session->OnConnectFailed();
            XPOOL_RELEASE(static_tcper_pool, this);
            break;
        }
        case eCompletion::recved: {
            _recving = false;
            if (0 == code && 0 != size) {
                if (_recv_buff.In(_recv_temp, size)) {
                    if (!AsyncRecv()) {
                        Close();
                        return;
                    }

                    int use = 0;
                    while (_recv_buff.Length() > 0 && (use = _session->OnReceive((const char*)_recv_buff.GetData(), _recv_buff.Length())) > 0) {
                        _recv_buff.Out(use);
                        if (INVALID_SOCKET == _socket) {
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
        case eCompletion::sended: {
            _sending = false;
            if (0 == code) {
                if (!_send_buff.Out(size)) {
                    XASSERT(false, "send buff out size %d error", size);
                    Close();
                    return;
                }

                if (_send_buff.Length() != 0) {
                    _net->TcpNeedSend(this);
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
        _net->TcpNoNeedSend(this);
        SAFE_CLOSE_SOCKET(_socket);
        if (false == _sending && false == _recving) {
            SafeMemory::Memset((void*)&_session->_pipe, sizeof(_session->_pipe), 0, sizeof(_session->_pipe));
            if (_session) {
                _session->OnDisconnect();
            }
            XPOOL_RELEASE(static_tcper_pool, this);
        }
    }

    void Tcper::Send(const void* data, const int size, bool immediately) {
        if (!_send_buff.In(data, size)) {
            Close();
            return;
        }
        if (immediately) {
            _net->TcpNeedSend(this);
        }
    }

    bool Tcper::AsyncRecv() {
        XASSERT(false == _recving, "wtf");
        SetLastError(0);
        DWORD bytes, flag = 0;
        SafeMemory::Memset(&_recv_ex._ol, sizeof(_recv_ex._ol), 0, sizeof(_recv_ex._ol));
        int res = WSARecv(_socket, &_recv_ex._wbuf, 1, &bytes, &flag, (LPWSAOVERLAPPED)&_recv_ex, nullptr);
        int err = GetLastError();
        if (SOCKET_ERROR == res && ERROR_IO_PENDING != err) {
            return false;
        }
        _recving = true;
        return true;
        return false;
    }

    bool Tcper::AsyncSend() {
        if (_sending) {
            return true;
        }

        if (_send_buff.Length() == 0) {
            return _recving;
        }

        SafeMemory::Memset(&_send_ex._ol, sizeof(_send_ex._ol), 0, sizeof(_send_ex._ol));
        _send_ex._wbuf.buf = (char*)_send_buff.GetData();
        _send_ex._wbuf.len = _send_buff.Length();

        int res = WSASend(_socket, &_send_ex._wbuf, 1, nullptr, 0, (LPWSAOVERLAPPED)&_send_ex, nullptr);
        int err = GetLastError();
        if (SOCKET_ERROR == res && WSA_IO_PENDING != err) {
            return false;
        }

        _sending = true;
        return true;
    }
}
