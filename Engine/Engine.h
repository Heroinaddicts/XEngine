#ifndef __Engine_h__
#define __Engine_h__

#include "iEngine.h"
#include <string>

namespace XEngine {
    class Engine : public Api::iEngine {
    public:
        static Engine* GetInstance();

        virtual ~Engine() {}
        virtual const char* GetLaunchParameter(const std::string& name);
        virtual void LogAsync(const std::string& log);
        virtual void LogSync(const std::string& log);
        virtual void Shutdown();

    public:
        bool isShutdown();

    private:
        Engine() {}
    };
}

#endif //__Engine_h__
