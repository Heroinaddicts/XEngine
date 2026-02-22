#include "WebSocketPipe.h"
#include "WebSocketContext.h"

namespace XEngine {

    XPool<WebSocketPipe> WebSocketPipe::s_Pool;
    WebSocketPipe::~WebSocketPipe() {
    }

    WebSocketPipe::WebSocketPipe(struct lws* wsi, WebSocketContext* context, const bool isActive, const char* file, const int line)
        :_Wsi(wsi), _WsContext(context), _Session(nullptr), _IsActive(isActive), _IsClosed(false), _IsPause(false), _CreateFile(file), _CreateLine(line)
    {

    }

    void WebSocketPipe::Send(const void* data, const int size, bool immediately) {
        AUTO_LOCK(_CloseLock);
        if (_IsClosed) {
            return;
        }

        _SendBuffer.Write(data, size);

        if (immediately) {
            WbsRequest* req = CreateWbsRequest();
            req->_Pipe = this;
            req->_Req = eWebSocketRequest::Send;
            _WsContext->PushRequest(req);
        }
    }

    const char* WebSocketPipe::RemoteIp() const {
        return "";
    }

    int WebSocketPipe::RemotePort() const {
        return 0;
    }

    bool WebSocketPipe::IsActiveConnect() const {
        return _IsActive;
    }

    WebSocketPipe* WebSocketPipe::Create(lws* wsi, WebSocketContext* context, const bool isActive, const char* file, const int line) {
        return XPOOL_CREATE(s_Pool, wsi, context, isActive, file, line);
    }

    void WebSocketPipe::Release(WebSocketPipe* pipe) {
        XPOOL_RELEASE(s_Pool, pipe);
    }

    void WebSocketPipe::OnClose(bool trigger) {
        AUTO_LOCK(_CloseLock);
        if (!_IsClosed) {
            _IsClosed = true;
            if (trigger) {
                WbsRequest* req = CreateWbsRequest();
                req->_Pipe = this;
                req->_Req = eWebSocketRequest::Close;
                _WsContext->PushRequest(req);
            }
        }
    }

    void WebSocketPipe::Close() {
        OnClose(true);
    }

    void WebSocketPipe::Pause() {
        _IsPause = true;
    }

    void WebSocketPipe::Resume() {
        if (!_IsClosed) {
            _IsPause = false;
            WbsEvent* ev = CreateWbsEvent();
            ev->_Context = this->_WsContext;
            ev->_Pipe = this;
            ev->_Event = eWebSocketEvent::Recv;
            g_WbsEventQueue.Push(ev);
        }
    }
}
