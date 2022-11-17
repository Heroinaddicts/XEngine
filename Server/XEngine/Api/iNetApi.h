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
            iTcpSocket() : _Pipe(nullptr) {}

            iTcpPipe* const _Pipe;
        };

        class iTcpSession : public iTcpSocket {
        public:
            virtual ~iTcpSession() {}
            iTcpSession() : _Initiative(false), _Ip(""), _Port(0) {}

            /*
            * 收到消息时回调(前提是没有Pause)
            * content 收到的数据地址
            * size 数据总长度
            * return int : 逻辑层处理了多少数据 就return多少, 解决逻辑层面拼包带来的内存操作开销, 逻辑层无需再拼包
            */
            virtual int OnReceive(const char* content, const int size) = 0;
            virtual void OnConnected() = 0;
            virtual void OnDisconnect() = 0;
            virtual void OnConnectFailed() = 0;

            inline void Close() const { if (this && _Pipe) { _Pipe->Close(); } }
            inline void Send(const void* context, const int size, bool immediately = true) const { if (this && _Pipe) { _Pipe->Send(context, size, immediately); } }
            inline void Pause() { if (this && _Pipe) { _Pipe->Pause(); } }
            inline void Resume() { if (this && _Pipe) { _Pipe->Resume(); } }

            const bool _Initiative; //是否是主动链接
            const std::string _Ip;
            const int _Port;
        };

        class iTcpServer {
        public:
            iTcpServer() : _Accepter(nullptr) {}
            virtual ~iTcpServer() {}

            virtual iTcpSession* OnMallocConnection(const char* remote_ip, const int remote_port) = 0;
            virtual void OnError(iTcpSession* session) = 0;
            virtual void OnRelease() = 0;

            virtual void Release() { _Accepter->Release(); }

            iAccepter* const _Accepter;
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
