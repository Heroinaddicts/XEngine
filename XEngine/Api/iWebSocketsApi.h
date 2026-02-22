#ifndef __iWebSocketsApi_h__
#define __iWebSocketsApi_h__

#include "MultiSys.h"
#include "iNetApi.h"
#include <functional>

namespace XEngine {
    namespace Api {

        class iWebSocketPipe {
        public:
            virtual ~iWebSocketPipe() {}

            virtual void Send(const void* data, const int size, bool immediately) = 0;
            virtual const char* RemoteIp() const = 0;
            virtual int RemotePort() const = 0;
            virtual bool IsActiveConnect() const = 0;
            virtual void Close() = 0;
            virtual void Pause() = 0;
            virtual void Resume() = 0;
        };

        class iWebSocketSession {
        public:

            virtual ~iWebSocketSession() {
                for (int i = 0; i < eTcpSessionEvent::__COUNT__; i++) {
                    if (_WebSocketSessionEventPool[i]) {
                        txdel _WebSocketSessionEventPool[i];
                        _WebSocketSessionEventPool[i] = nullptr;
                    }
                }
            }

            iWebSocketSession() : _WebSocketPipe(nullptr) {
                SafeMemory::Memset(_WebSocketSessionEventPool, sizeof(_WebSocketSessionEventPool), 0, sizeof(_WebSocketSessionEventPool));
            }

            iWebSocketPipe* const _WebSocketPipe;
            /*
            * 收到消息时回调(前提是没有Pause)
            * content 收到的数据地址
            * size 数据总长度
            * return int : 逻辑层处理了多少数据 就return多少, 解决逻辑层面拼包带来的内存操作开销, 逻辑层无需再拼包
            */
            virtual int OnReceive(const void* const content, const int size) = 0;

            void OnTcpSessionEvent(const eTcpSessionEvent ev) {
                if (nullptr != _WebSocketSessionEventPool[ev]) {
                    for (int i = 0; i < _WebSocketSessionEventPool[ev]->size(); i++) {
                        (*_WebSocketSessionEventPool[ev])[i](this);
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

            __forceinline void Close() const { if (this && _WebSocketPipe) { _WebSocketPipe->Close(); } }
            __forceinline void Send(const void* context, const int size, bool immediately = true) const { if (this && _WebSocketPipe) { _WebSocketPipe->Send(context, size, immediately); } }
            __forceinline void Pause() { if (this && _WebSocketPipe) { _WebSocketPipe->Pause(); } }
            __forceinline void Resume() { if (this && _WebSocketPipe) { _WebSocketPipe->Resume(); } }

            __forceinline const char* RemoteIp() const {
                if (this && _WebSocketPipe) {
                    return _WebSocketPipe->RemoteIp();
                }

                return nullptr;
            }

            __forceinline int RemotePort() const {
                if (this && _WebSocketPipe) {
                    return _WebSocketPipe->RemotePort();
                }

                return -1;
            }

            __forceinline bool IsActiveConnect() const {
                if (this && _WebSocketPipe) {
                    return _WebSocketPipe->IsActiveConnect();
                }

                return false;
            }

            template<typename T>
            void RegistTcpSessionEventCallback(const eTcpSessionEvent ev, std::function<void(T* const)> fun) {
                PrivateRegisterTcpSessionEventCallback(ev, [=](iWebSocketSession* const session) {
                    fun((T* const)session);
                    }
                );
            }

        protected:
            virtual void OnConnected() {}
            virtual void OnConnectFailed() {}
            virtual void OnDisconnected() {}

        private:
            typedef std::function<void(iWebSocketSession* const)> fTcpSessionEventCallback;
            virtual void PrivateRegisterTcpSessionEventCallback(const eTcpSessionEvent ev, const fTcpSessionEventCallback& fun) {
                if (nullptr == _WebSocketSessionEventPool[ev]) {
                    _WebSocketSessionEventPool[ev] = txnew std::vector<fTcpSessionEventCallback>();
                }
                _WebSocketSessionEventPool[ev]->push_back(fun);
            }

            std::vector<fTcpSessionEventCallback>* _WebSocketSessionEventPool[eTcpSessionEvent::__COUNT__];
        };

        class iWebSocketServerAccept {
        public:
            virtual ~iWebSocketServerAccept() {}
            virtual void Release() = 0;
        };

        class iWebSocketsServer {
        public:
            virtual ~iWebSocketsServer() {}
            iWebSocketsServer() : _Accepter(nullptr) {}

            virtual iWebSocketSession* OnMallocWebSocketSession(const char* remoteIp, const int remotePort) = 0;
            virtual void OnWebSocketError(iWebSocketSession* session) = 0;
            virtual void OnWebSocketRelease() = 0;

            virtual void WssRelease() { _Accepter->Release(); }

            iWebSocketServerAccept* const _Accepter;
        };

        class iWebSocketsApi {
        public:
            virtual ~iWebSocketsApi() {}

            virtual bool LaunchSession(iWebSocketSession* session, const char* host, const int port, const char * ca, const char* cert, const char* privateKey, const char* subproto = "") = 0;
            virtual bool LaunchServer(iWebSocketsServer* server, const char* ip, const int port, const char* cert, const char * privateKey, const char* subproto = "") = 0;
        };
    }
}

#endif //__iWebSocketsApi_h__
