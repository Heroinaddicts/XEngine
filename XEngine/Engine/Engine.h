#ifndef __Engine_h__
#define __Engine_h__

#include "iEngine.h"
#include "iModule.h"
#include <string>

namespace XEngine {
    class Engine : public Api::iEngine {
    public:
        static Engine* GetInstance();

        virtual ~Engine() {}

        const char* GetName() const override;
        const char* GetLaunchParameter(const std::string& name) const override;

        Api::iNetApi* GetNetApi() const override;
        Api::iHttpApi* GetHttpApi() const override;
        Api::iTimerApi* GetTimerApi() const override;
        Api::iZipApi* GetZipApi() const override;
        Api::iTaskApi* GetTaskApi() const override;
        Api::iMysqlApi* GetMysqlApi() const override;
        Api::iRedisApi* GetRedisApi() const override;
        Api::iWebSocketsApi* GetWebSocketsApi() const override;
        Api::iMemoryApi* GetMemoryApi() const override;

        void LogAsync(const char* header, const char* content, const bool console, const char* file, const int line) override;
        void LogSync(const char* header, const char* content, const bool console, const char* file, const int line) override;

        __forceinline void Shutdown() { _Shutdown = true; }
        Api::ProcessHandle LaunchXEngineProcess(const std::map<std::string, std::string>& launchParameters) const override;

        __forceinline bool isShutdown() const { return _Shutdown; }

    protected:
        Api::iModule* FindModule(const std::string& name) override;

    private:
        Engine() : _Shutdown(false) {}

    private:
        bool _Shutdown;
    };

    extern Engine* const g_Engine;
}

#endif //__Engine_h__
