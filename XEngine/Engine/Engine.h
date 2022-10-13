#ifndef __Engine_h__
#define __Engine_h__

#include "iEngine.h"
#include "iComponent.h"
#include <string>

namespace XEngine {
    class Engine : public Api::iEngine {
    public:
        static Engine* GetInstance();

        virtual ~Engine() {}
        virtual const char* GetLaunchParameter(const std::string& name);

        virtual Api::iNetApi* GetNetApi();
        virtual Api::iTimerApi* GetTimerApi();
        virtual Api::iNavigationApi* GetNavigationApi();
        virtual Api::iPhysicsApi* GetPhysicsApi();

        virtual float GetFixedTimeStep();

        virtual void LogAsync(const std::string& log);
        virtual void LogSync(const std::string& log);

        virtual void Shutdown();

    public:
        bool isShutdown();

    protected:
        virtual Api::iComponent* FindComponent(const std::string& name) override;

    private:
        Engine() {}
    };
}

#endif //__Engine_h__
