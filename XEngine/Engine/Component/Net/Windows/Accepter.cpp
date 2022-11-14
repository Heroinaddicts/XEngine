#include "Accepter.h"
#include "Tcper.h"
namespace XEngine {
    static XPool<Accepter> s_AccepterPool;

    Accepter* Accepter::Create(Api::iTcpServer* server, const std::string& ip, const int port, const int ssize, const int rsize) {
        Accepter* ac = XPOOL_CREATE(s_AccepterPool, server, ip, port, ssize, rsize);

        SafeMemory::Memset(&ac->_Addr, sizeof(ac->_Addr), 0, sizeof(ac->_Addr));
        inet_pton(AF_INET, ip.c_str(), (void*)&ac->_Addr.sin_addr.s_addr);
        ac->_Addr.sin_family = AF_INET;
        ac->_Addr.sin_port = htons(port);

        SetLastError(0);
        int len = 0;
        if (INVALID_SOCKET == (ac->_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED))
            || SOCKET_ERROR == setsockopt(ac->_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(int))
            || SOCKET_ERROR == setsockopt(ac->_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&len, sizeof(int))
            || SOCKET_ERROR == ::bind(ac->_Socket, (struct sockaddr*)&(ac->_Addr), sizeof(sockaddr_in))
            || listen(ac->_Socket, 2048) == SOCKET_ERROR) {
            XASSERT(false, "socket error %d", ::GetLastError());
            SAFE_CLOSE_SOCKET(ac->_Socket);
            XPOOL_RELEASE(s_AccepterPool, ac);
            server->OnError(nullptr);
            return nullptr;
        }

        XASSERT(ac, "wtf");
        if (g_CompletePort != CreateIoCompletionPort((HANDLE)ac->_Socket, g_CompletePort, (u_long)ac->_Socket, 0)) {
            server->OnError(nullptr);
            XPOOL_RELEASE(s_AccepterPool, ac);
            return nullptr;
        }

        if (false == ac->AsyncAccept()) {
            XPOOL_RELEASE(s_AccepterPool, ac);
            server->OnRelease();
            return nullptr;
        }


        Api::iAccepter* iac = ac;
        SafeMemory::Memcpy((void*)&server->_Accepter, sizeof(server->_Accepter), &iac, sizeof(iac));
        return ac;
    }

    void Accepter::OnCompleted(OVERLAPPEDEX* ex, const eCompletion type, const int code, const int size) {
        XASSERT(type == eCompletion::Accepted, "accepter error on completer type");
        if (0 == code) {
            DWORD ul = 1;
            if (SOCKET_ERROR == setsockopt(_Ex._Socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&_Socket, sizeof(SOCKET))
                || CreateIoCompletionPort((HANDLE)_Ex._Socket, (HANDLE)g_CompletePort, (u_long)_Ex._Socket, 0) != g_CompletePort
                || SOCKET_ERROR == ioctlsocket(_Ex._Socket, FIONBIO, &ul)
                || SOCKET_ERROR == setsockopt(_Ex._Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&ul, sizeof(ul))) {
                SAFE_CLOSE_SOCKET(_Ex._Socket);
            }
            else {
                struct sockaddr_in sa;
                int len = sizeof(sa);
                getpeername(_Ex._Socket, (struct sockaddr*)&sa, &len);
                const std::string ip = inet_ntoa(sa.sin_addr);
                const int port = ntohs(sa.sin_port);

                Api::iTcpSession* session = _Server->OnMallocConnection(ip.c_str(), port);
                if (session) {
                    Tcper* pipe = Tcper::Create(session, ip, port, _SendSize, _RecvSize, _Ex._Socket, false);
                    if (pipe) {
                        Api::iTcpPipe* ipipe = pipe;
                        SafeMemory::Memcpy((void*)&session->_Pipe, sizeof(void*), &ipipe, sizeof(ipipe));
                        ((std::string)session->_Ip) = ip;
                        ((int)session->_Port) = port;
                        session->OnConnected();
                    }
                    else {
                        SAFE_CLOSE_SOCKET(_Ex._Socket);
                        _Server->OnError(session);
                    }
                }
                else {
                    SAFE_CLOSE_SOCKET(_Ex._Socket);
                }
            }
        }
        else {
            SAFE_CLOSE_SOCKET(_Ex._Socket);
        }

        if (false == AsyncAccept()) {
            _Server->OnRelease();
            XPOOL_RELEASE(s_AccepterPool, this);
        }
    }

    void Accepter::Release() {

    }

    bool Accepter::AsyncAccept() {
        static LPFN_ACCEPTEX acceptex = GetAcceptExFun();
        DWORD ul = 1;
        int len = 0;
        if (INVALID_SOCKET == (_Ex._Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED))
            || SOCKET_ERROR == setsockopt(_Ex._Socket, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(int))
            || SOCKET_ERROR == setsockopt(_Ex._Socket, SOL_SOCKET, SO_RCVBUF, (char*)&len, sizeof(int))
            || SOCKET_ERROR == ioctlsocket(_Ex._Socket, FIONBIO, &ul)) {
            return false;
        }

        int err = GetLastError();
        DWORD bytes;
        int res = acceptex(
            _Socket,
            _Ex._Socket,
            _Temp,
            0,
            sizeof(struct sockaddr_in) + 16,
            sizeof(struct sockaddr_in) + 16,
            &bytes,
            (LPOVERLAPPED)&_Ex
        );

        LINGER linger = { 1,0 };
        if (res == FALSE && err != WSA_IO_PENDING
            && SOCKET_ERROR != setsockopt(_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(int))
            && SOCKET_ERROR != setsockopt(_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&len, sizeof(int))
            && setsockopt(_Socket, SOL_SOCKET, SO_LINGER,
                (char*)&linger, sizeof(linger))) {
            SAFE_CLOSE_SOCKET(_Ex._Socket);
            SAFE_CLOSE_SOCKET(_Socket);
            return false;
        }

        return true;
    }

    Accepter::Accepter(Api::iTcpServer* server, const std::string& ip, const int port, const int s_size, const int r_size)
        : _Server(server), _ListenIP(ip), _ListenPort(port), _SendSize(s_size), _RecvSize(r_size), _Socket(INVALID_SOCKET), _Ex(eCompletion::Accepted, this) {
    }
}
