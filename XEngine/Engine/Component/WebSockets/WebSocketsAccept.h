#ifndef __WebSocketsAccept_h__
#define __WebSocketsAccept_h__

#include "Header.h"

namespace XEngine {
    class WebSocketsAccept : public Api::iWebSocketServerAccept {
    public:
        virtual ~WebSocketsAccept();

        WebSocketsAccept(Api::iWebSocketsServer* server, const char * cert, const char * key, const int port, const std::string& subproto);

    public:
        Api::iWebSocketsServer* const _Server;
        WebSocketContext* const _Context;
    private:
        const int _Port;
		const std::string _Cert;
		const std::string _PrivateKey;
        const std::string _SubProto;
        // Í¨¹ý iWebSocketServerAccept ¼Ì³Ð
        void Release() override;
    };
}

#endif //__WebSocketsAccept_h__
