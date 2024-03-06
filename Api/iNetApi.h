#ifndef __iNetApi_h__
#define __iNetApi_h__

#include "MultiSys.h"
#include "SafeMemory.h"
#include <string>
#include <vector>
#include <functional>

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

        enum eTcpSessionEvent {
            Connected = 0,
            Disconnected = 1,
            Connectfaild = 2,

            __COUNT__
        };

        class iTcpPipe : public iPipe {
        public:
            virtual ~iTcpPipe() {}

            virtual void Send(const void* data, const int size, bool immediately) = 0;
            virtual const char* RemoteIp() const = 0;
            virtual int RemotePort() const = 0;
            virtual bool IsActiveConnect() const = 0;
        };

        class iTcpSocket : public iSocket {
        public:
            virtual ~iTcpSocket() {}
            iTcpSocket() : _Pipe(nullptr) {}

            iTcpPipe* const _Pipe;
        };

        class iTcpSession : public iTcpSocket {
        public:
            virtual ~iTcpSession() {
                for (int i = 0; i < eTcpSessionEvent::__COUNT__; i++) {
                    if (_TcpSessionEventPool[i]) {
                        xdel _TcpSessionEventPool[i];
                        _TcpSessionEventPool[i] = nullptr;
                    }
                }
            }

            iTcpSession() {
                SafeMemory::Memset(_TcpSessionEventPool, sizeof(_TcpSessionEventPool), 0, sizeof(_TcpSessionEventPool));
            }
            /*
            * 收到消息时回调(前提是没有Pause)
            * content 收到的数据地址
            * size 数据总长度
            * return int : 逻辑层处理了多少数据 就return多少, 解决逻辑层面拼包带来的内存操作开销, 逻辑层无需再拼包
            */
            virtual int OnReceive(const void* const content, const int size) = 0;

            void OnTcpSessionEvent(const eTcpSessionEvent ev) {
                if (nullptr != _TcpSessionEventPool[ev]) {
                    for (int i = 0; i < _TcpSessionEventPool[ev]->size(); i++) {
                        (*_TcpSessionEventPool[ev])[i](this);
                    }
                }

                switch (ev) {
                case eTcpSessionEvent::Connectfaild: {
                    OnConnectFailed();
                    break;
                }
                case eTcpSessionEvent::Connected: {
                    OnConnected();
                    break;
                }
                case eTcpSessionEvent::Disconnected: {
                    OnDisconnected();
                    break;
                }
                }
            }

            __forceinline void Close() const { if (this && _Pipe) { _Pipe->Close(); } }
            __forceinline void Send(const void* context, const int size, bool immediately = true) const { if (this && _Pipe) { _Pipe->Send(context, size, immediately); } }
            __forceinline void Pause() { if (this && _Pipe) { _Pipe->Pause(); } }
            __forceinline void Resume() { if (this && _Pipe) { _Pipe->Resume(); } }

            __forceinline const char* RemoteIp() const {
                if (this && _Pipe) {
                    return _Pipe->RemoteIp();
                }

                return nullptr;
            }

            __forceinline int RemotePort() const {
                if (this && _Pipe) {
                    return _Pipe->RemotePort();
                }

                return -1;
            }

            __forceinline bool IsActiveConnect() const {
                if (this && _Pipe) {
                    return _Pipe->IsActiveConnect();
                }

                return false;
            }

            template<typename T>
            void RegistTcpSessionEventCallback(const eTcpSessionEvent ev, void(*fun)(T* const session)) {
                PrivateRegisterTcpSessionEventCallback(ev, [=](iTcpSession* const session) {
                    fun((T* const)session);
                    }
                );
            }

        protected:
            virtual void OnConnected() {}
            virtual void OnConnectFailed() {}
            virtual void OnDisconnected() {}

        private:
            typedef std::function<void(iTcpSession* const)> fTcpSessionEventCallback;
            virtual void PrivateRegisterTcpSessionEventCallback(const eTcpSessionEvent ev, const fTcpSessionEventCallback& fun) {
                if (nullptr == _TcpSessionEventPool[ev]) {
                    _TcpSessionEventPool[ev] = xnew std::vector<fTcpSessionEventCallback>();
                }
                _TcpSessionEventPool[ev]->push_back(fun);
            }

            std::vector<fTcpSessionEventCallback>* _TcpSessionEventPool[eTcpSessionEvent::__COUNT__];
        };

        class iTcpServer {
        public:
            iTcpServer() : _Accepter(nullptr) {}
            virtual ~iTcpServer() {}

            virtual iTcpSession* OnMallocConnection(const char* remoteIp, const int remotePort) = 0;
            virtual void OnError(iTcpSession* session) = 0;
            virtual void OnRelease() = 0;

            virtual void Release() { _Accepter->Release(); }

            iAccepter* const _Accepter;
        };

        class iNetApi {
        public:
            virtual ~iNetApi() {}

            virtual bool LaunchTcpSession(iTcpSession* session, const char* host, const int port) = 0;
            virtual bool LaunchTcpServer(iTcpServer* server, const char* ip, const int port) = 0;
        };
    }
}

#endif //__iNetApi_h__
