#ifndef __Accepter_h__
#define __Accepter_h__

#include "Header.h"

namespace XEngine {
#define accept_temp_size 512

    class Accepter : public iCompleted, public Api::iAccepter {
    public:
        virtual ~Accepter() {}

        static Accepter* Create(Api::iTcpServer* server, const std::string& ip, const int port, const int ssize, const int rsize);

        virtual void OnCompleted(OVERLAPPEDEX* ex, const eCompletion type, const int code, const int size);
        virtual void Release();
        bool AsyncAccept();

        Api::iTcpServer* const _server;
        const int _ssize;
        const int _rsize;

        const std::string _listen_ip;
        const int _listen_port;

    private:
        friend XPool<Accepter>;
        Accepter(Api::iTcpServer* server, const std::string& ip, const int port, const int s_size, const int r_size);

    private:
        char _temp[accept_temp_size];

    private:
        int _socket;
        sockaddr_in _addr;
        OVERLAPPEDEX _ex;
    };
}

#endif //__Accepter_h__
