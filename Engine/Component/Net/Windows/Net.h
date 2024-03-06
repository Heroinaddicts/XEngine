#ifndef __Net_h__
#define __Net_h__

#include "Header.h"

#include <set>

namespace XEngine {
    class Tcper;

    class Net : public iNet {
    public:
        virtual ~Net() {}

        static iNet* GetInstance();

        // 通过 iEngineComponent 继承
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;
        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        // 通过 iNet 继承
        virtual bool LaunchTcpSession(Api::iTcpSession* session, const char* host, const int port) override;
        virtual bool LaunchTcpServer(Api::iTcpServer* server, const char* ip, const int port) override;
    private:
        Net() {}
    };
}

#endif //__Net_h__
