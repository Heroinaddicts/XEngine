#ifndef __Tcper_h__
#define __Tcper_h__

#include "Header.h"

namespace XEngine {
    class Net;
    class Tcper : public iCompleter, public Api::iTcpPipe {
    public:
        virtual ~Tcper() {}

        static Tcper* Create(const int socket, const std::string& host, const UInt16 port, const bool active, Api::iTcpSession* const session, const int size);
        static void Release(Tcper* tcper);

        __forceinline void OnlySetTcpSession(Api::iTcpSession* const session) {
            if (_Session) {
                SafeMemory::Memset((void*)&_Session->_Pipe, sizeof(void*), 0, sizeof(void*));
            }

            _Session = session;
        }

        __forceinline void CorrelateTcpSession(Api::iTcpSession* const session) {
            if (_Session) {
                SafeMemory::Memset((void*)&_Session->_Pipe, sizeof(void*), 0, sizeof(void*));
            }

            _Session = session;

            if (_Session) {
                Api::iTcpPipe* p = this;
                SafeMemory::Memcpy((void*)&session->_Pipe, sizeof(void*), &p, sizeof(p));
            }
        }

        __forceinline Api::iTcpSession* GetTcpSession() const {
            return _Session;
        }

        void ProcessData();

        const std::string _IP;
        const int _Port;
        const bool _IsActiveConnect;
        const Associat _Associat;


        void OnCompleter(Associat* at, const eCompletion type, const struct epoll_event& ev) override;

        void Pause() override;
        void Resume() override;
        void Close() override;

        void Send(const void* data, const int size, bool immediately) override;
        std::string RemoteIp() const override;
        int RemotePort() const override;
        bool IsActiveConnect() const override;

    private:
        friend XPool<Tcper>;
        Tcper(const int sock, const std::string& host, const UInt16 port, const bool active, const int size);

    private:
        int _Sock;
        bool _IsClosed;
        Api::iTcpSession* _Session;

        char _RecvTemp[RECV_TEMP_SIZE];

        XBuffer _SendBuffer;
        XBuffer _RecvBuffer;

        SpinLock _Lock;
    };
}

#endif //__Tcper_h__
