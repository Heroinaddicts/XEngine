#ifndef __WebSocketContext_h__
#define __WebSocketContext_h__

#include "Header.h"

namespace XEngine {
	class WebSocketContext : public SafeThread {
	public:
		virtual ~WebSocketContext();

#define CREATE_WEBSOCKET_CONTEXT(type, user, ca, cert, key, port, subproto) WebSocketContext::CreateClientContext(type, user, ca, cert, key, port, subproto, __FILE__, __LINE__)
		static WebSocketContext* CreateClientContext(
			eWebSocketContextType type,
			void* const user,
			const char* ca,
			const char* cert,
			const char* key,
			const int port = CONTEXT_PORT_NO_LISTEN,
			const char* subproto = "XEngine",
			const char* file = __FILE__,
			const int line = __LINE__
		);

		static void ReleaseWebSocketContext(WebSocketContext* context);

		__forceinline struct lws_context* const GetContext()const {
			return _LwsContext;
		}

		__forceinline void PushRequest(WbsRequest* req) {
			_WbsRequestQueue.Push(req);
			lws_cancel_service(_LwsContext);
		}

		template<typename T>
		__forceinline T * GetUser() const {
			return (T*)_User;
		}

	protected:
		void Run(void* context) override;

		static int CallbackEcho(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len);

	private:
		WebSocketContext(
			eWebSocketContextType type,
			void* const user,
			const char* ca,
			const char* cert,
			const char* key,
			const int port,
			const char* subproto,
			const char* file,
			const int line
		);
		friend XPool<WebSocketContext>;

		const eWebSocketContextType _Type;
		const std::string _CaPem;
		const std::string _CertPem;
		const std::string _KeyPem;
		const std::string _SubProto;
		const int _Port;
		struct lws_context* _LwsContext;
		struct lws_protocols* _Protocols;
		void* const _User;
		SafeQueue::SpscQueue<WbsRequest*> _WbsRequestQueue;
		const std::string _File;
		const int _Line;
	};
}

#endif //__WebSocketContext_h__
