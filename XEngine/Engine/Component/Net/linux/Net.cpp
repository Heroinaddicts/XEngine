#include "Net.h"
#include "Tcper.h"
#include "Accepter.h"
#include "SafeSystem.h"

namespace XEngine {
    iNet* Net::GetInstance() {
        static Net static_net;
        return &static_net;
    }

    bool Net::Initialize(Api::iEngine* const engine) {
        return true;
    }

    bool Net::Launch(Api::iEngine* const engine) {
        return true;
    }

    void Net::Release(Api::iEngine* const engine) {
    }

    void Net::EarlyUpdate(Api::iEngine* const engine) {
    }

    void Net::Update(Api::iEngine* const engine) {
    }

    void Net::FixedUpdate(Api::iEngine* const engine) {

    }

    void Net::LaterUpdate(Api::iEngine* const engine) {

    }

    bool Net::LaunchTcpSession(Api::iTcpSession* session, const char* host, const int port, int max_ss, int max_rs) {
        return true;
    }

    bool Net::LaunchTcpServer(Api::iTcpServer* server, const char* ip, const int port, int max_ss, int max_rs) {
        return true;
    }
}
