#ifndef __accepter_h__
#define __accepter_h__

#include "interface.h"

namespace tcore {
    class accepter : public iCompleter, public api::iAccepter {
    public:
        virtual ~accepter() {}
        
        static accepter * create(iTcpServer * server, const char * ip, const int port, int s_size, int r_size);
        
        virtual void onCompleter(associat * at, const eCompletion type, const struct epoll_event & ev);
        virtual void release();
        
    private:
        friend tlib::tpool<accepter>;
        accepter(iTcpServer * server, const char * ip, const int port, int s_size, int r_size);
        
    private:
        iTcpServer * const _server;
        const int _ssize;
        const int _rsize;
        api::oAddress _address;
        int _socket;
        sockaddr_in _addr;
        associat _at;
    };
}

#endif //__accepter_h__
