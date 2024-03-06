#ifndef __Tcper_h__
#define __Tcper_h__

#include "Header.h"

namespace XEngine {
    class Net;
    class Tcper : public iCompleted, public Api::iTcpPipe {
    public:
        virtual ~Tcper();


        static Tcper* Create(Api::iTcpSession* session, const std::string& ip, const int port, int sock, bool isActiveConnect);

        virtual void OnCompleted(OVERLAPPEDEX* ex, const eCompletion type, const int code, const int size);
        virtual void Pause();
        virtual void Resume();
        virtual void Close();
        virtual void Send(const void* data, const int size, bool immediately);


		// Í¨¹ý iTcpPipe ¼Ì³Ð
		const char* RemoteIp() const override;
		int RemotePort() const override;
		bool IsActiveConnect() const override;

        void CloseSocket();
        void AsyncSend();
    private:
        friend XPool<Tcper>;
		Tcper(Api::iTcpSession* session, int socket, bool isActiveConnect);

    private:
        bool AsyncRecv();

    private:
        bool _IsSending, _IsRecving, _IsCached;

        char _RecvTemp[PACK_MAX_SIZE];

        XBuffer<16 * SafeSystem::Network::KB, 16 * SafeSystem::Network::KB> _SendBuff;
        XBuffer<16 * SafeSystem::Network::KB, 16 * SafeSystem::Network::KB> _RecvBuff;

        OVERLAPPEDEX _SendEx;
        OVERLAPPEDEX _RecvEx;

        Api::iTcpSession* const _Session;
        const bool _IsActiveConnect;
        int _Socket;
        std::string _RemoteIP;
        int _RemotePort;

        Net* _Net;
    };
}

#endif //__Tcper_h__
