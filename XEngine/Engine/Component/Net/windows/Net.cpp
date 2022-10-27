#include "Net.h"
#include "Tcper.h"
#include "Accepter.h"

#include "SafeSystem.h"

namespace XEngine {
    XPool<OVERLAPPEDEX> g_OverlappedexPool;
    HANDLE g_CompletePort;

    iNet* Net::GetInstance() {
        static Net s_Instance;
        return &s_Instance;
    }

    bool Net::Initialize(Api::iEngine* const engine) {
        WSADATA wsa;
        int err = WSAStartup(MAKEWORD(1, 1), &wsa);
        if (err != 0) {
            XERROR(engine, "WSAStartup error");
            return false;
        }

        if (LOBYTE(wsa.wVersion) != 1 || HIBYTE(wsa.wVersion) != 1) {
            XERROR(engine, "WSAStartup LOBYTE or HIBYTE error");
            WSACleanup();
            return false;
        }
        g_CompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
        if (nullptr == g_CompletePort) {
            XERROR(engine, "CreateIoCompletionPort error : %s", strerror(::GetLastError()));
            return false;
        }

        return true;
    }

    bool Net::Launch(Api::iEngine* const engine) {
        return true;
    }

    void Net::Release(Api::iEngine* const engine) {
    }

    void Net::EarlyUpdate(Api::iEngine* const engine) {
        int64 tick = SafeSystem::Time::GetMilliSecond();
        do {
            DWORD bytes = 0;
            SOCKET sock = INVALID_SOCKET;
            OVERLAPPEDEX* plus = nullptr;
            SetLastError(0);

            BOOL res = GetQueuedCompletionStatus(g_CompletePort, &bytes, (PULONG_PTR)&sock, (LPOVERLAPPED*)&plus, 0);
            int err = GetLastError();
            if (!res) {
                if (WAIT_TIMEOUT == err) {
                    return;
                }
            }

            plus->_Completer->OnCompleted(plus, plus->_Type, err, bytes);
        } while (SafeSystem::Time::GetMilliSecond() - tick < 2);
    }

    void Net::Update(Api::iEngine* const engine) {
    }

    void Net::FixedUpdate(Api::iEngine* const engine) {

    }

    void Net::LaterUpdate(Api::iEngine* const engine) {
        for (auto i = _TcpNeedSendPool.begin(); i != _TcpNeedSendPool.end(); i++) {
            (*i)->AsyncSend();
        }
        _TcpNeedSendPool.clear();
    }

    bool Net::LaunchTcpSession(Api::iTcpSession* session, const char* host, const int port, int max_ss, int max_rs) {
        SetLastError(0);
        DWORD ul = 1;
        int len = 0;
        LINGER linger = { 1,0 };

        bool initiative = true;
        SafeMemory::Memcpy((void*)&session->_Initiative, sizeof(bool), &initiative, sizeof(initiative));

        if (!GetIpByHost(host, (std::string&)session->_Ip)) {
            session->OnConnectFailed();
            return false;
        }

        ((int)session->_Port) = port;

        int socket = INVALID_SOCKET;
        sockaddr_in addr;
        SafeMemory::Memset(&addr, sizeof(addr), 0, sizeof(addr));
        addr.sin_family = AF_INET;
        if (INVALID_SOCKET == (socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED))
            || SOCKET_ERROR == setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(int))
            || SOCKET_ERROR == setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char*)&len, sizeof(int))
            || setsockopt(socket, SOL_SOCKET, SO_LINGER,
                (char*)&linger, sizeof(linger))
            || SOCKET_ERROR == ioctlsocket(socket, FIONBIO, &ul)
            || (SOCKET_ERROR == ::bind(socket, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))
            || CreateIoCompletionPort((HANDLE)socket, (HANDLE)g_CompletePort, (u_long)socket, 0) != g_CompletePort) {
            SAFE_CLOSE_SOCKET(socket);
            session->OnConnectFailed();
            return false;
        }

        Tcper* pipe = Tcper::Create(session, session->_Ip, session->_Port, max_ss, max_rs, socket, true);
        if (nullptr == pipe) {
            SAFE_CLOSE_SOCKET(socket);
            session->OnConnectFailed();
            return false;
        }

        Api::iTcpPipe* ipipe = pipe;
        SafeMemory::Memcpy((void*)&session->_Pipe, sizeof(session->_Pipe), &ipipe, sizeof(ipipe));
        return true;
    }

    bool Net::LaunchTcpServer(Api::iTcpServer* server, const char* ip, const int port, int max_ss, int max_rs) {
        Accepter* ac = Accepter::Create(server, ip, port, max_ss, max_rs);
        return ac != nullptr;
    }
}
