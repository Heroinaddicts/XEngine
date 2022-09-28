#include "Accepter.h"
#include "Tcper.h"
namespace XEngine {
    static XPool<Accepter> static_accepter_pool;

    Accepter* Accepter::Create(Api::iTcpServer* server, const std::string& ip, const int port, const int ssize, const int rsize) {
        Accepter* ac = XPOOL_CREATE(static_accepter_pool, server, ip, port, ssize, rsize);

        SafeMemory::Memset(&ac->_addr, sizeof(ac->_addr), 0, sizeof(ac->_addr));
        inet_pton(AF_INET, ip.c_str(), (void*)&ac->_addr.sin_addr.s_addr);
        ac->_addr.sin_family = AF_INET;
        ac->_addr.sin_port = htons(port);

        SetLastError(0);
        int len = 0;
        if (INVALID_SOCKET == (ac->_socket = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED))
            || SOCKET_ERROR == setsockopt(ac->_socket, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(int))
            || SOCKET_ERROR == setsockopt(ac->_socket, SOL_SOCKET, SO_RCVBUF, (char*)&len, sizeof(int))
            || SOCKET_ERROR == ::bind(ac->_socket, (struct sockaddr*)&(ac->_addr), sizeof(sockaddr_in))
            || listen(ac->_socket, 2048) == SOCKET_ERROR) {
            XASSERT(false, "socket error %d", ::GetLastError());
            SAFE_CLOSE_SOCKET(ac->_socket);
            XPOOL_RELEASE(static_accepter_pool, ac);
            server->OnError(nullptr);
            return nullptr;
        }

        XASSERT(ac, "wtf");
        if (g_complete_port != CreateIoCompletionPort((HANDLE)ac->_socket, (HANDLE)g_complete_port, (u_long)ac->_socket, 0)) {
            server->OnError(nullptr);
            XPOOL_RELEASE(static_accepter_pool, ac);
            return nullptr;
        }

        if (false == ac->AsyncAccept()) {
            XPOOL_RELEASE(static_accepter_pool, ac);
            server->OnRelease();
            return nullptr;
        }


        Api::iAccepter* iac = ac;
        SafeMemory::Memcpy((void*)&server->_ac, sizeof(server->_ac), &iac, sizeof(iac));
        return ac;
    }

    void Accepter::OnCompleted(OVERLAPPEDEX* ex, const eCompletion type, const int code, const int size) {
        XASSERT(type == eCompletion::accepted, "accepter error on completer type");
        if (0 == code) {
            DWORD ul = 1;
            if (SOCKET_ERROR == setsockopt(_ex._socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&_socket, sizeof(SOCKET))
                || CreateIoCompletionPort((HANDLE)_ex._socket, (HANDLE)g_complete_port, (u_long)_ex._socket, 0) != g_complete_port
                || SOCKET_ERROR == ioctlsocket(_ex._socket, FIONBIO, &ul)
                || SOCKET_ERROR == setsockopt(_ex._socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&ul, sizeof(ul))) {
                SAFE_CLOSE_SOCKET(_ex._socket);
            }
            else {
                struct sockaddr_in sa;
                int len = sizeof(sa);
                getpeername(_ex._socket, (struct sockaddr*)&sa, &len);
                const std::string ip = inet_ntoa(sa.sin_addr);
                const int port = ntohs(sa.sin_port);

                Api::iTcpSession* session = _server->OnMallocConnection(ip.c_str(), port);
                XASSERT(session, "malloc session nullptr");
                if (session) {
                    Tcper* pipe = Tcper::Create(session, ip, port, _ssize, _rsize, _ex._socket, false);
                    if (pipe) {
                        Api::iTcpPipe* ipipe = pipe;
                        SafeMemory::Memcpy((void*)&session->_pipe, sizeof(void*), &ipipe, sizeof(ipipe));
                        ((std::string)session->_ip) = ip;
                        ((int)session->_port) = port;
                        session->OnConnected();
                    }
                    else {
                        SAFE_CLOSE_SOCKET(_ex._socket);
                        _server->OnError(session);
                    }
                }
                else {
                    SAFE_CLOSE_SOCKET(_ex._socket);
                }
            }
        }
        else {
            SAFE_CLOSE_SOCKET(_ex._socket);
        }

        if (false == AsyncAccept()) {
            _server->OnRelease();
            XPOOL_RELEASE(static_accepter_pool, this);
        }
    }

    void Accepter::Release() {

    }

    bool Accepter::AsyncAccept() {
        static LPFN_ACCEPTEX acceptex = GetAcceptExFun();
        DWORD ul = 1;
        int len = 0;
        if (INVALID_SOCKET == (_ex._socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED))
            || SOCKET_ERROR == setsockopt(_ex._socket, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(int))
            || SOCKET_ERROR == setsockopt(_ex._socket, SOL_SOCKET, SO_RCVBUF, (char*)&len, sizeof(int))
            || SOCKET_ERROR == ioctlsocket(_ex._socket, FIONBIO, &ul)) {
            return false;
        }

        int err = GetLastError();
        DWORD bytes;
        int res = acceptex(
            _socket,
            _ex._socket,
            _temp,
            0,
            sizeof(struct sockaddr_in) + 16,
            sizeof(struct sockaddr_in) + 16,
            &bytes,
            (LPOVERLAPPED)&_ex
        );

        LINGER linger = { 1,0 };
        if (res == FALSE && err != WSA_IO_PENDING
            && SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(int))
            && SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char*)&len, sizeof(int))
            && setsockopt(_socket, SOL_SOCKET, SO_LINGER,
                (char*)&linger, sizeof(linger))) {
            SAFE_CLOSE_SOCKET(_ex._socket);
            SAFE_CLOSE_SOCKET(_socket);
            return false;
        }

        return true;
    }

    Accepter::Accepter(Api::iTcpServer* server, const std::string& ip, const int port, const int s_size, const int r_size)
        : _server(server), _listen_ip(ip), _listen_port(port), _ssize(s_size), _rsize(r_size), _socket(INVALID_SOCKET), _ex(eCompletion::accepted, this) {
    }
}
