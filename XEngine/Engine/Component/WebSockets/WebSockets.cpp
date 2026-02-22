#include "WebSockets.h"
#include "WebSocketPipe.h"
#include "WebSocketsAccept.h"
#include "WebSocketContext.h"

namespace XEngine {
    XPool<WbsEvent> g_WbsEventPool;
    SafeQueue::SpscQueue<WbsEvent*> g_WbsEventQueue;
    XPool<WbsRequest> g_WbsRequestPool;

    WebSockets* WebSockets::GetInstance() {
        static WebSockets s_WebSockets;
        return &s_WebSockets;
    }

    WebSockets::WebSockets() {}

	bool WebSockets::Initialize(Api::iEngine* const engine) {
		lws_set_log_level(
			LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO,
			[](int level, const char* line) {
				switch (level) {
				case LLL_NOTICE:
				case LLL_INFO:
				case LLL_WARN:
					TraceLog(g_Engine, "%s", line);
					break;
				case LLL_ERR:
                    ErrorLog(g_Engine, "%s", line);
					break;
				}
			}
		);
        return true;
    }

    bool WebSockets::Launch(Api::iEngine* const engine) {
        return true;
    }

    void WebSockets::Release(Api::iEngine* const engine) {
    }

    void WebSockets::EarlyUpdate(Api::iEngine* const engine) {
    }

    void WebSockets::Update(Api::iEngine* const engine) {
    }

    void WebSockets::LaterUpdate(Api::iEngine* const engine) {
        WbsEvent* ev = nullptr;
        UInt64 tick = SafeSystem::Time::GetMicroSecond();
        while (g_WbsEventQueue.Pull(ev)) {
            switch (ev->_Event) {
            case eWebSocketEvent::Accepted: {
                Api::iWebSocketSession* session = ev->_Context->GetUser<WebSocketsAccept>()->_Server->OnMallocWebSocketSession("", 0);
                ev->_Pipe->SetSession(session);
                ev->_Pipe->_Session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connected);
                break;
            }
            case eWebSocketEvent::Connected: {
                ev->_Pipe->SetSession(ev->_Session);
                ev->_Session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connected);
                break;
            }
            case eWebSocketEvent::Recv: {
                while (!ev->_Pipe->_IsClosed && !ev->_Pipe->_IsPause &&
                    ev->_Pipe->_RecvBuffer.Read(
                        [&](const void* const data, const UInt32 size) -> UInt32 {
                            if (size > 0) {
								return ev->_Pipe->_Session->OnReceive(data, size);
                            }
                            return 0;
                        }
                    )) {
                }
                break;
            }
            case eWebSocketEvent::Disconnected: {
                Api::iWebSocketSession* session = ev->_Pipe->_Session;
                Api::iWebSocketPipe* temp = nullptr;
                SafeMemory::Memcpy((void*)&ev->_Pipe->_Session->_WebSocketPipe, sizeof(ev->_Pipe->_Session->_WebSocketPipe), &temp, sizeof(temp));
                session->OnTcpSessionEvent(Api::eTcpSessionEvent::Disconnected);

                WbsRequest* req = CreateWbsRequest();
				req->_Req = eWebSocketRequest::RelesePipe;
				req->_Pipe = ev->_Pipe;
                break;
            }
            }

            ReleaseWbsEvent(ev);

            if (SafeSystem::Time::GetMicroSecond() - tick > 1000) {
                break;
			}
        }
	}

    bool WebSockets::LaunchSession(Api::iWebSocketSession* session, const char* host, const int port, const char* ca, const char* cert, const char* privateKey, const char* subproto) {
        WebSocketContext* context = CREATE_WEBSOCKET_CONTEXT(eWebSocketContextType::Client, this, ca, cert, privateKey, CONTEXT_PORT_NO_LISTEN, subproto);
        if (nullptr == context->GetContext()) {
            WebSocketContext::ReleaseWebSocketContext(context);
			ErrorLog(g_Engine, "Create WebSocketContext failed, Client connection failed %s:%d", host, port);
            session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connectfaild);
			return false;
        }

		// 客户端连接参数
		struct lws_client_connect_info info = { 0 };
		memset(&info, 0, sizeof(info));
		info.context = context->GetContext();
		info.port = port;
		info.address = host;
		info.path = "/";
		info.host = info.address;
		info.origin = info.address;
		info.protocol = subproto;
		info.local_protocol_name = "lws-minimal-client";
		info.userdata = session;
        info.ssl_connection |= LCCSCF_USE_SSL;
		info.ssl_connection |= LCCSCF_ALLOW_SELFSIGNED;
		info.ssl_connection |= LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;

		lws* wsi = lws_client_connect_via_info(&info);
		if (!wsi) {
			WebSocketContext::ReleaseWebSocketContext(context);
			session->OnTcpSessionEvent(Api::eTcpSessionEvent::Connectfaild);
			ErrorLog(g_Engine, "Client connection failed %s:%d", host, port);
			return false;
		}
		lws_set_wsi_user((lws*)wsi, session);
        return true;
    }

    bool WebSockets::LaunchServer(Api::iWebSocketsServer* server, const char* ip, const int port, const char* cert, const char* privateKey, const char* subproto) {
        WebSocketsAccept* wsa = txnew WebSocketsAccept(server, cert, privateKey, port, subproto);
        return true;
	}
}
