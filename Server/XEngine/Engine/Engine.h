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
        virtual const char* GetLaunchParameter(const std::string& name);

        virtual Api::iNetApi* GetNetApi() const;
        virtual Api::iTimerApi* GetTimerApi() const;
        virtual Api::iNavigationApi* GetNavigationApi() const;
        virtual Api::iPhysicsApi* GetPhysicsApi() const;
        virtual Api::iGameObjectApi* GetGameObjectApi() const;

        virtual float GetFixedTimeStep();

        virtual void LogAsync(const std::string& log);
        virtual void LogSync(const std::string& log);

        virtual void Shutdown();

    public:
        bool isShutdown();

    protected:
        virtual Api::iModule* FindModule(const std::string& name) override;

    private:
        Engine() {}
    };

    extern Engine* const g_Engine;
}

#endif //__Engine_h__
