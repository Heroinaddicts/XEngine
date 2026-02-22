#pragma once

#include "iEngineComponent.h"
#include "iNet.h"
#include "Engine.h"

#include "SafeMemory.h"
#include "SafeSystem.h"
#include "SafeThread.h"
#include "SafeQueue.h"
#include "XPool.h"
#include "XBuffer.h"
#include "XLock.h"

#include <queue>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netdb.h>
#include <set>

#define ERROR_SOCKET -1
#define INVALID_FD -1
#define SUCCESS 0
#define EPOLLER_MAX_EVENT_COUNT 512
#define EPOLLER_DESC_COUNT 10240
#define MAX_PIPE_SIZE (20 * 1024 * 1024)
#define RECV_TEMP_SIZE 512

namespace XEngine
{
    using namespace XEngine::Api;

    extern int g_EpollerFd;

    enum eCompletion
    {
        doAccept,
        doConnect,
        doIO,
    };

    class iCompleter;
    struct Associat
    {
        const eCompletion _Ev;
        iCompleter* const _Completer;

        Associat(const eCompletion ev, iCompleter* const p) : _Ev(ev), _Completer(p) {}
    };

    class iCompleter
    {
    public:
        virtual ~iCompleter() {}

        virtual void OnCompleter(Associat* at, const eCompletion type, const struct epoll_event& ev) = 0;
    };

    extern XPool<Associat> g_AssociatPool;

    class Tcper;
    extern XPool<Tcper> g_TcperPool;

    class Accepter;
    extern XPool<Accepter> g_AccepterPool;

#define CLOSE_SOCKET(sock) \
    { \
        ::shutdown(sock, SHUT_RDWR); \
        ::close(sock);     \
        sock = INVALID_FD; \
    }

    enum eNetEventType
    {
        Accept,
        Connect,
        Disconnect,
        Recv
    };

    struct NetEvent
    {
        const eNetEventType _Type;
        int _Code;
        Int64 _Sock;
        std::string _IP;
        int _Port;
        Accepter* _Accepter;
        Tcper* _Tcper;
        Api::iTcpSession* _Session;

        NetEvent(const eNetEventType type) : _Type(type), _Code(0), _Accepter(nullptr), _Tcper(nullptr), _Session(nullptr) {}
    };
    extern SafeQueue::SpscQueue<NetEvent*> g_NetEventQueue;

    enum eNetRequestType
    {
        ReleaseTcper,
        CloseTcper
    };

    struct NetRequest
    {
        const eNetRequestType _Type;
        Tcper* _Tcper;

        NetRequest(const eNetRequestType type) : _Type(type) {}
    };
    extern SafeQueue::SpscQueue<NetRequest*> g_NetRequestQueue;

    extern int g_ConnectedCount;
    extern int g_DisconnectedCount;

    extern SpinLock g_CountLock;

#define CONNECTED_COUNT    // ConnectedCount(__FILE__, __LINE__)
#define DISCONNECTED_COUNT // DisconnectedCount(__FILE__, __LINE__)

    __forceinline void ConnectedCount(const char* file, const int line)
    {
        AUTO_LOCK(g_CountLock);
        XLOG(g_Engine, "Tcper ConnectedCount %d/%d %s:%d", ++g_ConnectedCount, g_DisconnectedCount, file, line);
    }

    __forceinline void DisconnectedCount(const char* file, const int line)
    {
        AUTO_LOCK(g_CountLock);
        XLOG(g_Engine, "Tcper DisconnectedCount %d/%d %s:%d", g_ConnectedCount, ++g_DisconnectedCount, file, line);
    }
}

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus__

    __forceinline int SetNonBlocking(int sockfd)
    {
        return fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK);
    }

    __forceinline int SetTcpNoDelay(const int fd)
    {
        int val = 1;
        return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&val, sizeof(val));
    }

    __forceinline int SetTcpQuickAck(const int fd)
    {
        int val = 1;
        return setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, (const char*)&val, sizeof(val));
    }

    __forceinline int SetReuse(const int fd)
    {
        int val = 1;
        return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&val, sizeof(val));
    }

    __forceinline int SetSockSendBuff(int sock, int size)
    {
        return setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size));
    }

    __forceinline int SetSockRecvBuff(int sock, int size)
    {
        return setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size));
    }

    __forceinline bool SetMaxOpenFile(const int size)
    {
        struct rlimit rt;
        rt.rlim_max = rt.rlim_cur = size;
        return setrlimit(RLIMIT_NOFILE, &rt) != -1;
    }

    __forceinline bool SetStackSize(const int size)
    {
        struct rlimit rt;
        rt.rlim_max = rt.rlim_cur = size * 1024;
        return setrlimit(RLIMIT_STACK, &rt) != -1;
    }

    __forceinline bool GetIpByHost(const char* host, OUT std::string& ip)
    {
        struct hostent* hp;
        if ((hp = gethostbyname(host)) == NULL)
        {
            return false;
        }

        ip = inet_ntoa(*(struct in_addr*)hp->h_addr);
        return true;
    }

#ifdef __cplusplus
}
#endif //__cplusplus
