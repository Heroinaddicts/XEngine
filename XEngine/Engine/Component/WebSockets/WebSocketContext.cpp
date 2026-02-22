#include "WebSocketContext.h"
#include "WebSocketPipe.h"
#include "WebSocketsAccept.h"

namespace XEngine {
	static std::map<std::string, WebSocketContext*> s_Ca_ContextMap;
	static XPool<WebSocketContext> s_Pool;

	__forceinline std::string GetKey(const eWebSocketContextType type, const char* ca, const char * cert, const char * key, const int port) {
		std::string temp;
		if (type == eWebSocketContextType::Server) {
			temp = "Server_";
		} else {
			temp = "Client_";
		}
		if (ca) {
			temp += std::string(ca) + "_" + cert + "_" + key;
		}
		if (port > 0) {
			temp += std::string("_") + std::to_string(port);
		}
		return temp;
	}

	WebSocketContext::~WebSocketContext() {
		if (_Protocols) {
			txdel[] _Protocols;
		}
	}

	WebSocketContext* WebSocketContext::CreateClientContext(
		const eWebSocketContextType type, 
		void* const user,
		const char* ca,
		const char* cert,
		const char* key,
		const int port,
		const char* subproto,
		const char* file,
		const int line
	) {
		auto itor = s_Ca_ContextMap.find(GetKey(type, ca, cert, key, port));
		if (s_Ca_ContextMap.end() != itor) {
			return itor->second;
		}

		return XPOOL_CREATE(s_Pool, type, user, ca, cert, key, port, subproto, file, line);
	}


	void WebSocketContext::ReleaseWebSocketContext(WebSocketContext* context) {
		context->CloseThread();
		if (context->_LwsContext) {
			lws_cancel_service(context->_LwsContext);
		}
		while (context->GetThreadCount() != 0) {}
		XPOOL_RELEASE(s_Pool, context);
	}


	WebSocketContext::WebSocketContext(eWebSocketContextType type, void* const user, const char* ca, const char* cert, const char* key, const int port, const char* subproto, const char* file, const int line)
		: _Type(type),
		_User(user),
		_CaPem(SafeSystem::File::LoadFileAsString(ca)),
		_CertPem(SafeSystem::File::LoadFileAsString(cert)),
		_KeyPem(SafeSystem::File::LoadFileAsString(key)),
		_LwsContext(nullptr),
		_SubProto(subproto),
		_Protocols(nullptr),
		_Port(port),
		_File(file),
		_Line(line)
	{
		struct lws_context_creation_info info = { 0 };
		memset(&info, 0, sizeof info);
		info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

		switch (type) {
		case eWebSocketContextType::Client: {
			_Protocols = new lws_protocols[2];
			_Protocols[0] = { _SubProto.c_str(), WebSocketContext::CallbackEcho, 0, 8192};
			_Protocols[1] = { NULL, NULL, 0, 0 };

			info.port = CONTEXT_PORT_NO_LISTEN;
			info.protocols = _Protocols;
			info.user = this;
			info.fd_limit_per_thread = 0;
			info.count_threads = 10;
 			info.client_ssl_ca_mem = _CaPem.c_str();
 			info.client_ssl_ca_mem_len = _CaPem.length();
// 			
// 			info.client_ssl_cert_mem = _CertPem.c_str();
// 			info.client_ssl_cert_mem_len = _CertPem.length();
// 
// 			info.client_ssl_key_mem = _KeyPem.c_str();
// 			info.client_ssl_key_mem_len = _KeyPem.length();

			_LwsContext = lws_create_context(&info);
			if (!_LwsContext) {
				ErrorLog(g_Engine, "lws_create_context failed");
			}
			break;
		}
		case eWebSocketContextType::Server: {
			_Protocols = new lws_protocols[2];
			_Protocols[0] = { _SubProto.c_str(), WebSocketContext::CallbackEcho, 0, 8192 };
			_Protocols[1] = { NULL, NULL, 0, 0 };

			info.port = _Port;
			info.protocols = _Protocols;
			info.options |= LWS_SERVER_OPTION_DISABLE_OS_CA_CERTS; 
			info.options |= LWS_SERVER_OPTION_VALIDATE_UTF8;
			info.user = this;
			info.fd_limit_per_thread = 0;
			info.count_threads = 10;

			info.server_ssl_cert_mem = _CertPem.c_str();
			info.server_ssl_cert_mem_len = _CertPem.length();

			info.server_ssl_private_key_mem = _KeyPem.c_str();
			info.server_ssl_private_key_mem_len = _KeyPem.length();

			_LwsContext = lws_create_context(&info);
			if (!_LwsContext) {
				ErrorLog(g_Engine, "lws_create_context failed");
			}
			break;
		}
		}

		if (_LwsContext) {
			Start();
		}
	}

	void WebSocketContext::Run(void* context) {
		while (!_IsClose) {
			lws_service(_LwsContext, 0);
			if (!_IsClose) {
				WbsRequest* req = nullptr;
				while (_WbsRequestQueue.Pull(req)) {
					switch (req->_Req) {
					case eWebSocketRequest::Send:
					case eWebSocketRequest::Close: {
						if (req->_Pipe->_Wsi) {
							lws_callback_on_writable(req->_Pipe->_Wsi);
						}
						break;
					}
					case eWebSocketRequest::RelesePipe: {
						WebSocketPipe::Release(req->_Pipe);
						break;
					}
					}
					ReleaseWbsRequest(req);
				}
			}
		}
	}


	int WebSocketContext::CallbackEcho(lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
		switch (reason) {
		case LWS_CALLBACK_WSI_CREATE: {
			return 0;
		}
		case LWS_CALLBACK_ESTABLISHED: {
			WebSocketContext* context = reinterpret_cast<WebSocketContext*>(lws_context_user(lws_get_context(wsi)));
			WebSocketPipe* pipe = CREATE_WEBSOCKETPIPE(wsi, context, false);
			lws_set_wsi_user((lws*)wsi, pipe);
			WbsEvent* ev = CreateWbsEvent();
			ev->_Context = context;
			ev->_Pipe = pipe;
			ev->_Event = eWebSocketEvent::Accepted;
			lws_callback_on_writable(wsi);
			g_WbsEventQueue.Push(ev);
			return 0;
		}
		case LWS_CALLBACK_RECEIVE: {
			WebSocketPipe* pipe = (WebSocketPipe*)lws_wsi_user(wsi);
			pipe->_RecvBuffer.Write(in, len);
			WbsEvent* ev = CreateWbsEvent();
			ev->_Pipe = pipe;
			ev->_Event = eWebSocketEvent::Recv;
			g_WbsEventQueue.Push(ev);
			return 0;
		}
		case LWS_CALLBACK_OPENSSL_PERFORM_SERVER_CERT_VERIFICATION: {
			break;
		}
		case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER: {
			return 0;
		}
		case LWS_CALLBACK_VHOST_CERT_AGING: {
			return 0;
		}
		case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP: {
			return 0;
		}
		case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH: {
			return 0;
		}
		case LWS_CALLBACK_CLIENT_ESTABLISHED: {
			WebSocketContext* wsc = reinterpret_cast<WebSocketContext*>(lws_context_user(lws_get_context(wsi)));
			Api::iWebSocketSession* session = (Api::iWebSocketSession*)lws_wsi_user(wsi);
			WebSocketPipe* pipe = CREATE_WEBSOCKETPIPE(wsi, wsc, true);
			lws_set_wsi_user((lws*)wsi, pipe);
			WbsEvent* ev = CreateWbsEvent();
			ev->_Context = wsc;
			ev->_Pipe = pipe;
			ev->_Session = session;
			ev->_Event = eWebSocketEvent::Connected;
			lws_callback_on_writable(wsi);
			g_WbsEventQueue.Push(ev);
			return 0;
		}
		case LWS_CALLBACK_CLIENT_RECEIVE: {
			WebSocketPipe* pipe = (WebSocketPipe*)lws_wsi_user(wsi);
			pipe->_RecvBuffer.Write(in, len);
			WbsEvent* ev = CreateWbsEvent();
			ev->_Pipe = pipe;
			ev->_Event = eWebSocketEvent::Recv;
			g_WbsEventQueue.Push(ev);
			return 0;
		}
		case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION: {
			return 0;
		}
		case LWS_CALLBACK_FILTER_NETWORK_CONNECTION: {
			return 0;
		}
		case LWS_CALLBACK_CLIENT_RECEIVE_PONG: {
			return 0;
		}
		case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: {
			return 0;
		}
		case LWS_CALLBACK_SERVER_WRITEABLE: {
			WebSocketPipe* pipe = (WebSocketPipe*)lws_wsi_user(wsi);
			if (!pipe->_IsClosed) {
				if (pipe->_SendBuffer.GetDataSize() > 0) {
					pipe->_SendBuffer.Read(
						[&](const void* const data, const UInt32 size) -> UInt32 {
							if (size > 0) {
								unsigned char* buf = (unsigned char*)alloca(LWS_PRE + size);
								memcpy(buf + LWS_PRE, data, size);
								int n = lws_write(wsi, buf + LWS_PRE, size, LWS_WRITE_BINARY);
								return n;
							}
							return size;
						}
					);
				}
				return 0;
			}
			return -1;
		}
		case LWS_CALLBACK_CLIENT_CLOSED:
		case LWS_CALLBACK_CLOSED: {
			WebSocketPipe* pipe = (WebSocketPipe*)lws_wsi_user(wsi);
			SpinLock& lock = pipe->_CloseLock;
			AUTO_LOCK(lock);
			DEBUG(g_Engine, "CallbackEcho reason LWS_CALLBACK_CLOSED");
			if (pipe) {
				pipe->OnClose(false);
				WbsEvent* ev = CreateWbsEvent();
				ev->_Pipe = pipe;
				ev->_Event = eWebSocketEvent::Disconnected;
				g_WbsEventQueue.Push(ev);
			}
			return 0;
		}
		case LWS_CALLBACK_CLIENT_WRITEABLE: {
			WebSocketPipe* pipe = (WebSocketPipe*)lws_wsi_user(wsi);
			if (!pipe->_IsClosed) {
				if (pipe->_SendBuffer.GetDataSize() > 0) {
					pipe->_SendBuffer.Read(
						[&](const void* const data, const UInt32 size) -> UInt32 {
							if (size > 0) {
								unsigned char* buf = (unsigned char*)alloca(LWS_PRE + size);
								memcpy(buf + LWS_PRE, data, size);
								int n = lws_write(wsi, buf + LWS_PRE, size, LWS_WRITE_BINARY);
								return n;
							}
							return size;
						}
					);
				}
				return 0;
			}
			return -1;
		}
		case LWS_CALLBACK_ADD_HEADERS: {
			return 0;
		}
		case LWS_CALLBACK_CONNECTING: {
			return 0;
		}
		case LWS_CALLBACK_MQTT_NEW_CLIENT_INSTANTIATED: {
			return 0;
		}
		case LWS_CALLBACK_MQTT_IDLE: {
			return 0;
		}
		case LWS_CALLBACK_MQTT_CLIENT_ESTABLISHED: {
			return 0;
		}
		case LWS_CALLBACK_MQTT_SUBSCRIBED: {
			return 0;
		}
		case LWS_CALLBACK_MQTT_CLIENT_WRITEABLE: {
			return 0;
		}
		case LWS_CALLBACK_MQTT_CLIENT_RX: {
			return 0;
		}
		case LWS_CALLBACK_MQTT_UNSUBSCRIBED: {
			return 0;
		}
		case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS: {
			return 0;
		}
		case LWS_CALLBACK_MQTT_DROP_PROTOCOL: {
			return 0;
		}
		case LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED: {
			return 0;
		}
		case LWS_CALLBACK_GET_THREAD_ID: {
			return 0;
		}
		case LWS_CALLBACK_CLOSED_CLIENT_HTTP: {
			return 0;
		}
		case LWS_CALLBACK_PROTOCOL_INIT: {
			return 0;
		}
		case LWS_CALLBACK_EVENT_WAIT_CANCELLED: {
			return 0;
		}
		case LWS_CALLBACK_CLOSED_HTTP: {
			return 0;
		}
		case LWS_CALLBACK_WSI_DESTROY: {
			return 0;
		}
		case LWS_CALLBACK_HTTP_CONFIRM_UPGRADE: {
			return 0;
		}
		case LWS_CALLBACK_HTTP_BIND_PROTOCOL: {
			return 0;
		}
		case LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION: {
			return 1;
		}
		case LWS_CALLBACK_WS_SERVER_DROP_PROTOCOL: {
			return 0;
		}
		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: {
			return 0;
		}
		case LWS_CALLBACK_CLIENT_HTTP_BIND_PROTOCOL: {
			return 0;
		}
		case LWS_CALLBACK_CLIENT_HTTP_DROP_PROTOCOL: {
			return 0;
		}
		case LWS_CALLBACK_WS_CLIENT_DROP_PROTOCOL: {
			return 0;
		}
		default: {
			DEBUG(g_Engine, "reason %d", reason);
			return 0;
		}
		}
		return 0;
	}
}
