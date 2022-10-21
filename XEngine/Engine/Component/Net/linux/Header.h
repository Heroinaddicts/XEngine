#pragma once

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netdb.h>

#include "iEngineComponent.h"
#include "iNet.h"
#include "iEngine.h"

#include "SafeMemory.h"

#include "XPool.h"
#include "XBuffer.h"

#define PACK_MAX_SIZE 512
#define INVALID_SOCKET -1

#define SafeCloseSocket(sock) ::close(sock); sock=INVALID_SOCKET;

namespace XEngine {
	enum eCompletion {
		accept,
		connect,
		io
	};

	class iCompleted;

	class iCompleted {
	public:
		virtual ~iCompleted() {}

		virtual void OnCompleted(const eCompletion type, const int code, const int size) = 0;
	};

	class Tcper;
	extern XPool<Tcper> g_tcper_pool;

	class Accepter;
	extern XPool<Accepter> g_accepter_pool;

#ifdef __cpluscplus
extern "C" {
#endif //__cpluscplus

	__forceinline int SetNonBlocking(const int socket) {
		return fcntl(socket, F_SETFL, fcntl(socket, F_GETFD, 0) | O_NONBLOCK);
	}

	__forceinline int SetTcpNoDelay(const int socket) {
		int val = 1;
		return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&val, sizeof(val));
	}

#ifdef __cpluscplus
}
#endif //__cpluscplus

}
