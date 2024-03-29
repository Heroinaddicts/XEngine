#ifndef __Net_h__
#define __Net_h__

#include "Header.h"

namespace XEngine {
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
        virtual void FixedUpdate(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        // 通过 iNet 继承
        virtual bool LaunchTcpSession(Api::iTcpSession* session, const char* host, const int port, int max_ss, int max_rs) override;
        virtual bool LaunchTcpServer(Api::iTcpServer* server, const char* ip, const int port, int max_ss, int max_rs) override;
    };
}

#endif //__Net_h__
