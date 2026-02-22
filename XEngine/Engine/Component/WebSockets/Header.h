#ifndef __WebSocketsHeader_h__
#define __WebSocketsHeader_h__

#include "iWebSockets.h"

#include "SafeThread.h"
#include "SafeMemory.h"
#include "SafeString.h"
#include "SafeQueue.h"
#include "XPool.h"

#include "XBuffer.h"

#include "libwebsockets.h"

#include "Engine.h"
#include <signal.h>

#include "XLock.h"

namespace XEngine {

    enum eWebSocketContextType {
        Server,
        Client
    };

    class WebSockets;
    class WebSocketPipe;
    class WebSocketsAccept;
    class WebSocketContext;

    enum eWebSocketEvent {
        Accepted,
        Connected,
        Recv,
        Disconnected,

        eWebSocketEventUnknown
    };

    struct WbsEvent {
        eWebSocketEvent _Event;
        WebSocketContext* _Context;
        WebSocketPipe* _Pipe;
        Api::iWebSocketSession* _Session;
        WbsEvent() : _Event(eWebSocketEvent::eWebSocketEventUnknown), _Context(nullptr), _Pipe(nullptr), _Session(nullptr) {}
    };

    enum eWebSocketRequest {
        Send,
        Close,
        RelesePipe,

        eWebSocketRequestUnknown
    };

    struct WbsRequest {
        eWebSocketRequest _Req;
        WebSocketPipe* _Pipe;
        WbsRequest() : _Req(eWebSocketRequest::eWebSocketRequestUnknown), _Pipe(nullptr) {}
    };

    extern XPool<WbsEvent> g_WbsEventPool;
    __forceinline WbsEvent* CreateWbsEvent() { return XPOOL_CREATE(g_WbsEventPool); }
    __forceinline void ReleaseWbsEvent(WbsEvent* ev) { XPOOL_RELEASE(g_WbsEventPool, ev); }
    extern SafeQueue::SpscQueue<WbsEvent*> g_WbsEventQueue;


    extern XPool<WbsRequest> g_WbsRequestPool;
    __forceinline WbsRequest* CreateWbsRequest() { return XPOOL_CREATE(g_WbsRequestPool); }
    __forceinline void ReleaseWbsRequest(WbsRequest* req) { XPOOL_RELEASE(g_WbsRequestPool, req); }
}

#endif //__WebSocketsHeader_h__
