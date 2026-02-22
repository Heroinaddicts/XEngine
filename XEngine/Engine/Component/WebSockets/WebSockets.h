#ifndef __WebSockets_h__
#define __WebSockets_h__

#include "Header.h"

namespace XEngine {
    class WebSockets : public iWebSockets {
    public:
        virtual ~WebSockets() {}

        static WebSockets* GetInstance();

    private:
        WebSockets();

        // 通过 iWebSockets 继承
        bool Initialize(Api::iEngine* const engine) override;
        bool Launch(Api::iEngine* const engine) override;
        void Release(Api::iEngine* const engine) override;
        void EarlyUpdate(Api::iEngine* const engine) override;
        void Update(Api::iEngine* const engine) override;
        void LaterUpdate(Api::iEngine* const engine) override;

        // 通过 iWebSockets 继承
        bool LaunchSession(Api::iWebSocketSession* session, const char* host, const int port, const char* ca, const char* cert, const char* privateKey, const char* subproto) override;
        bool LaunchServer(Api::iWebSocketsServer* server, const char* ip, const int port, const char* cert, const char* privateKey, const char* subproto) override;
    };
}

#endif //__WebSockets_h__
