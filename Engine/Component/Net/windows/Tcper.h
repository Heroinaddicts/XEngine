#ifndef __Tcper_h__
#define __Tcper_h__

#include "Header.h"

namespace XEngine {
    class Net;
    class Tcper : public iCompleted, public Api::iTcpPipe {
    public:
        virtual ~Tcper() {}

        Tcper(Api::iTcpSession* session, const int sendsize, const int recvsize, int socket);

        static Tcper* Create(Api::iTcpSession* session, const std::string& ip, const int port, const int sendsize, const int recvsize, int sock, bool initiative);

        virtual void OnCompleted(OVERLAPPEDEX* ex, const eCompletion type, const int code, const int size);
        virtual void Pause();
        virtual void Resume();
        virtual void Close();
        virtual void Send(const void* data, const int size, bool immediately);

        bool AsyncSend();
    private:
        bool AsyncRecv();

    private:
        bool _sending, _recving, _caching;

        char _recv_temp[PACK_MAX_SIZE];

        XBuffer _send_buff;
        XBuffer _recv_buff;

        OVERLAPPEDEX _send_ex;
        OVERLAPPEDEX _recv_ex;

        Api::iTcpSession* const _session;
        int _socket;

        Net* _net;
    };
}

#endif //__Tcper_h__
