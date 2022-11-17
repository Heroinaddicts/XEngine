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
        bool _IsSending, _IsRecving, _IsCached;

        char _RecvTemp[PACK_MAX_SIZE];

        XBuffer _SendBuff;
        XBuffer _RecvBuff;

        OVERLAPPEDEX _SendEx;
        OVERLAPPEDEX _RecvEx;

        Api::iTcpSession* const _Session;
        int _Socket;

        Net* _Net;
    };
}

#endif //__Tcper_h__
