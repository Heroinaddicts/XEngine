#ifndef __Accepter_h__
#define __Accepter_h__

#include "Header.h"

namespace XEngine {
    class Accepter : public iCompleter, public Api::iAccepter {
    public:
        virtual ~Accepter() {}

        static Accepter* Create(iTcpServer* server, const std::string& listenIp, const UInt16 listenPort, const int size);

        virtual void OnCompleter(Associat* at, const eCompletion type, const struct epoll_event& ev);
        virtual void Release();

    private:
        friend XPool<Accepter>;
        Accepter(const int sock, iTcpServer* server, const std::string& listenIp, const UInt16 listenPort, const int size);

    public:
        iTcpServer* const _Server;
        const std::string _ListenIP;
        const UInt16 _ListenPort;
        const Associat _Associat;
        const int _TcpBufferSize;

        const int _Socket;
    };
}

#endif //__Accepter_h__
