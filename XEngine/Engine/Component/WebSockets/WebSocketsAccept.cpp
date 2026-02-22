#include "WebSocketsAccept.h"
#include "WebSocketContext.h"

namespace XEngine {
    WebSocketsAccept::~WebSocketsAccept() {}

    WebSocketsAccept::WebSocketsAccept(Api::iWebSocketsServer* server, const char* cert, const char* key, const int port, const std::string& subproto)
        : _Server(server), _Port(port), _Cert(cert), _PrivateKey(key), _SubProto(subproto),
        _Context(CREATE_WEBSOCKET_CONTEXT(eWebSocketContextType::Server, this, "", cert, key, port, subproto.c_str()))
	{
		iWebSocketServerAccept* accept = this;
		SafeMemory::Memcpy((void*)&_Server->_Accepter, sizeof(_Server->_Accepter), &accept, sizeof(accept));
    }

    void WebSocketsAccept::Release() {

    }
}
