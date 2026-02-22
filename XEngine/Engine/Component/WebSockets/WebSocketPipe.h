#ifndef __WebSocketPipe_h__
#define __WebSocketPipe_h__

#include "Header.h"

namespace XEngine {
    class WebSocketPipe : public Api::iWebSocketPipe {
    public:
        virtual ~WebSocketPipe();

        const bool _IsActive;

        __forceinline void SetSession(Api::iWebSocketSession* session) {
            XASSERT(_Session == nullptr, "wtf");
            if (nullptr == _Session) {
                iWebSocketPipe* pipe = this;
                SafeMemory::Memcpy((void*)&session->_WebSocketPipe, sizeof(session->_WebSocketPipe), &pipe, sizeof(pipe));
                _Session = session;
            }
        }

#define CREATE_WEBSOCKETPIPE(wsi, context, active) WebSocketPipe::Create(wsi, context, active, __FILE__, __LINE__)

        static WebSocketPipe* Create(struct lws* wsi, WebSocketContext* context, const bool isActive, const char* file, const int line);
        static void Release(WebSocketPipe* pipe);

        void OnClose(bool trigger);

        XBuffer<4096> _RecvBuffer;
        XBuffer<4096> _SendBuffer;
        bool _IsClosed;
        bool _IsPause;

        SpinLock _CloseLock;
    private:
        friend WebSocketsAccept;
        friend WebSockets;
        friend WebSocketContext;
        struct lws* _Wsi;
        WebSocketContext* const _WsContext;
        Api::iWebSocketSession* _Session;

        WebSocketPipe(struct lws* wsi, WebSocketContext* context, const bool isActive, const char* file, const int line);
        friend XPool<WebSocketPipe>;
        static XPool<WebSocketPipe> s_Pool;

        // 通过 iWebSocketPipe 继承
        void Send(const void* data, const int size, bool immediately) override;
        const char* RemoteIp() const override;
        int RemotePort() const override;
        bool IsActiveConnect() const override;

        // 通过 iWebSocketPipe 继承
        void Close() override;
        void Pause() override;
        void Resume() override;

        const std::string _CreateFile;
        const int _CreateLine;
    };
}

#endif //__WebSocketPipe_h__
