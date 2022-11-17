#ifndef __Accepter_h__
#define __Accepter_h__

#include "Header.h"

namespace XEngine {
    class Accepter : public iCompleter, public Api::iAccepter {
    public:
        virtual ~Accepter() {}
        
        static Accepter * Create(iTcpServer * server, const char * ip, const int port, int s_size, int r_size);
        
        virtual void OnCompleter(Associat * at, const eCompletion type, const struct epoll_event & ev);
        virtual void Release();
        
    private:
        friend XPool<Accepter>;
        Accepter(iTcpServer * server, const char * ip, const int port, int s_size, int r_size);
        
    private:
        iTcpServer * const _Server;
        const int _SendSize;
        const int _RecvSize;
        int _Socket;
        sockaddr_in _Addr;
        Associat _Associat;
    };
}

#endif //__Accepter_h__
