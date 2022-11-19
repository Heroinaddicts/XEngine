#pragma once

#include "iEngineComponent.h"
#include "iNet.h"
#include "iEngine.h"

#include "SafeMemory.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma  comment(lib, "ws2_32.lib")

#include "XPool.h"
#include "XBuffer.h"

#define PACK_MAX_SIZE 512

namespace XEngine {
    enum class eCompletion {
        Accepted,
        Connected,
        Sended,
        Recved
    };

    class iCompleted;
    struct OVERLAPPEDEX {
        OVERLAPPED _Ol;
        WSABUF _Wbuff;
        int _Socket;
        sockaddr_in _Remote;

        const eCompletion _Type;
        iCompleted* const _Completer;

        OVERLAPPEDEX(const eCompletion type, iCompleted* const completer) : _Type(type), _Completer(completer) {
            SafeMemory::Memset(&_Ol, sizeof(_Ol), 0, sizeof(_Ol));
        }
    };

    class iCompleted {
    public:
        virtual ~iCompleted() {}

        virtual void OnCompleted(OVERLAPPEDEX* const ex, const eCompletion type, const int code, const int size) = 0;
    };

    static LPFN_ACCEPTEX GetAcceptExFun() {
        static LPFN_ACCEPTEX fun = nullptr;
        if (nullptr == fun) {
            GUID guid = WSAID_ACCEPTEX;
            DWORD dwBytes = 0;
            SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &fun, sizeof(fun), &dwBytes, nullptr, nullptr);

            XASSERT(fun, "Get AcceptEx fun error, error code : %d", GetLastError());
        }

        return fun;
    }

    static LPFN_CONNECTEX GetConnectExFun() {
        static LPFN_CONNECTEX fun = nullptr;
        if (nullptr == fun) {
            GUID guid = WSAID_CONNECTEX;
            DWORD bytes = 0;
            SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &fun, sizeof(fun), &bytes, nullptr, nullptr);
            XASSERT(fun, "get connectex fun error %d", GetLastError());
        }

        return fun;
    }

    static bool GetIpByHost(const char* host, OUT std::string& ip) {
        struct hostent* hp;
        if ((hp = gethostbyname(host)) == nullptr) {
            return false;
        }

        ip = inet_ntoa(*(struct in_addr*)hp->h_addr);
        return true;
    }

#   define SAFE_CLOSE_SOCKET(sock) {\
        closesocket(sock); \
        sock = INVALID_SOCKET; \
    }

    extern XPool<OVERLAPPEDEX> g_OverlappedexPool;

    extern HANDLE g_CompletePort;
}