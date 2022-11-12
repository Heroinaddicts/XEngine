#ifndef __tcper_h__
#define __tcper_h__

#include "interface.h"

namespace tcore {
    class tcper : public iCompleter, public api::iTcpPipe {
    public:
        virtual ~tcper() {}
        
        static tcper * create(api::iTcpSession * session, const int sock, const std::string & ip, const int port, int ssize, int rsize);
        static tcper * create(api::iTcpSession * session, const std::string & ip, const int port, int ssize, int rsize);
        
        virtual void onCompleter(associat * at, const eCompletion type, const struct epoll_event & ev);
        
        virtual void close();
        virtual void cache();
        virtual void load();
        virtual void send(const void * data, const int size, bool immediately);
        
    private:
        friend accepter;
        friend tlib::tpool<tcper>;
        tcper(api::iTcpSession * session, const std::string & ip, const int port, const int ssize, const int rsize);
    private:
        bool _connected;
        bool _caching;
        int _socket;
        const associat _associat;
        const api::oAddress _remote;
        api::iTcpSession * const _session;

        tlib::cbuffer _send_buff;
        tlib::cbuffer _recv_buff;

        int64 _last_send_tick;
    };
}

#endif //__tcper_h__
