#ifndef __Accepter_h__
#define __Accepter_h__

#include "Header.h"

namespace XEngine {
#define accept_temp_size 512

    class Accepter : public iCompleted, public Api::iAccepter {
    public:
        virtual ~Accepter() {}

        static Accepter* Create(Api::iTcpServer* server, const std::string& ip, const int port);

        virtual void OnCompleted(OVERLAPPEDEX* ex, const eCompletion type, const int code, const int size);
        virtual void Release();
        bool AsyncAccept();

        Api::iTcpServer* const _Server;
        const std::string _ListenIP;
        const int _ListenPort;

    private:
        friend XPool<Accepter>;
        Accepter(Api::iTcpServer* server, const std::string& ip, const int port);

    private:
        char _Temp[accept_temp_size];

    private:
        int _Socket;
        sockaddr_in _Addr;
        OVERLAPPEDEX _Ex;
    };
}

#endif //__Accepter_h__
