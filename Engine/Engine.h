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

        virtual const char* GetName() const;
        virtual const char* GetLaunchParameter(const std::string& name) const;

        virtual Api::iNetApi* GetNetApi() const;
        virtual Api::iHttpApi* GetHttpApi() const;
        virtual Api::iTimerApi* GetTimerApi() const;
        virtual Api::iZipApi* GetZipApi() const;

        virtual void LogAsync(const char* header, const char* content, const char* file, const int line);
        virtual void LogSync(const char* header, const char* content, const char* file, const int line);

        __forceinline void Shutdown() { _Shutdown = true; }
        virtual Api::ProcessHandle LaunchXEngineProcess(const std::map<std::string, std::string>& launchParameters) const;

        __forceinline bool isShutdown() const { return _Shutdown; }

    protected:
        virtual Api::iModule* FindModule(const std::string& name) override;

    private:
        Engine() : _Shutdown(false) {}

    private:
        bool _Shutdown;
    };

    extern Engine* const g_Engine;
}

#endif //__Engine_h__
