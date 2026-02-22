#ifndef __Net_h__
#define __Net_h__

#include "Header.h"

namespace XEngine {
    class Net : public iNet, public SafeThread {
    public:
        virtual ~Net() {}

        static iNet* GetInstance();

        // 通过 iEngineComponent 继承
        bool Initialize(Api::iEngine* const engine) override;
        bool Launch(Api::iEngine* const engine) override;
        void Release(Api::iEngine* const engine) override;
        void EarlyUpdate(Api::iEngine* const engine) override;
        void Update(Api::iEngine* const engine) override;
        void LaterUpdate(Api::iEngine* const engine) override;

        bool LaunchTcpSession(iTcpSession* session, const std::string& host, const UInt16 port, const int size) override;
        bool LaunchTcpServer(iTcpServer* server, const std::string& listenIp, const UInt16 listenPort, const int size) override;

    protected:
        void Run(void* context) override;
    };
}

#endif //__Net_h__
