#ifndef __iNetApi_h__
#define __iNetApi_h__

namespace XEngine {
    namespace Api {
        class iSocket {
        public:
            virtual ~iSocket() {}
        };

        class iAccepter {
        public:
            virtual ~iAccepter() {}
            virtual void Release() = 0;
        };

        class iPipe {
        public:
            virtual ~iPipe() {}

            virtual void Pause() = 0;
            virtual void Resume() = 0;
            virtual void Close() = 0;
        };

        class iTcpPipe : public iPipe {
        public:
            virtual ~iTcpPipe() {}

            virtual void Send(const void* data, const int size, bool immediately) = 0;
        };

        class iTcpSocket : public iSocket {
        public:
            virtual ~iTcpSocket() {}
            iTcpSocket() : _pipe(nullptr) {}

            iTcpPipe* const _pipe;
        };

        class iTcpSession : public iTcpSocket {
        public:
            virtual ~iTcpSession() {}
            iTcpSession() : _initiative(false), _ip(""), _port(0) {}

            /*
            * �յ���Ϣʱ�ص�(ǰ����û��Pause)
            * content �յ������ݵ�ַ
            * size �����ܳ���
            * return int : �߼��㴦���˶������� ��return����, ����߼�����ƴ���������ڴ��������, �߼���������ƴ��
            */
            virtual int OnReceive(const char* content, const int size) = 0;
            virtual void OnConnected() = 0;
            virtual void OnDisconnect() = 0;
            virtual void OnConnectFailed() = 0;

            inline void Close() const { if (this && _pipe) { _pipe->Close(); } }
            inline void Send(const void* context, const int size, bool immediately = true) const { if (this && _pipe) { _pipe->Send(context, size, immediately); } }
            inline void Pause() { if (this && _pipe) { _pipe->Pause(); } }
            inline void Resume() { if (this && _pipe) { _pipe->Resume(); } }

            const bool _initiative; //�Ƿ�����������
            const std::string _ip;
            const int _port;
        };

        class iTcpServer {
        public:
            iTcpServer() : _ac(nullptr) {}
            virtual ~iTcpServer() {}

            virtual iTcpSession* OnMallocConnection(const char* remote_ip, const int remote_port) = 0;
            virtual void OnError(iTcpSession* session) = 0;
            virtual void OnRelease() = 0;

            virtual void Release() { _ac->Release(); }

            iAccepter* const _ac;
        };

        class iNetApi {
        public:
            virtual ~iNetApi() {}

            virtual bool LaunchTcpSession(iTcpSession* session, const char* host, const int port, int max_ss, int max_rs) = 0;
            virtual bool LaunchTcpServer(iTcpServer* server, const char* ip, const int port, int max_ss, int max_rs) = 0;
        };
    }
}

#endif //__iNetApi_h__
