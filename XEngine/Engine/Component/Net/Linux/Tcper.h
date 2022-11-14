#ifndef __Tcper_h__
#define __Tcper_h__

#include "Header.h"

namespace XEngine {
    class Tcper : public iCompleter, public Api::iTcpPipe {
    public:
        virtual ~Tcper();
        
        static Tcper * Create(Api::iTcpSession * session, const int sock, const std::string & ip, const int port, int ssize, int rsize);
        static Tcper * Create(Api::iTcpSession * session, const std::string & ip, const int port, int ssize, int rsize);
        
        virtual void OnCompleter(Associat * at, const eCompletion type, const struct epoll_event & ev);
        virtual void Send(const void * data, const int size, bool immediately);
        
        __forceinline void Pause() {}
        __forceinline void Resume() {}
        virtual void Close();
        
        void DoSend();

    private:
        friend Accepter;
        friend XPool<Tcper>;
        Tcper(Api::iTcpSession * session, const std::string & ip, const int port, const int ssize, const int rsize);
    private:
        bool _Connected;
        bool _Caching;
        int _Socket;
        const Associat _Associat;
        Api::iTcpSession * const _Session;

        XBuffer _SendBuff;
        XBuffer _RecvBuff;

        int64 _LastSendTick;
    };
}

#endif //__Tcper_h__
