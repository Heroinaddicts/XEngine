#ifndef __iEngine_h__
#define __iEngine_h__

#include <string>

namespace XEngine {
    namespace Api {
        class iNetApi;
        class iTimerApi;
        class iNavigationApi;
        class iPhysicsApi;
        class iComponent;

        class iEngine {
        public:
            virtual ~iEngine() {}

            virtual const char* GetLaunchParameter(const std::string& name) = 0;

            virtual iNetApi* GetNetApi() = 0;
            virtual iTimerApi* GetTimerApi() = 0;
            virtual iNavigationApi* GetNavigationApi() = 0;
            virtual iPhysicsApi* GetPhysicsApi() = 0;

            virtual float GetFixedTimeStep() = 0;

            virtual void LogAsync(const std::string& log) = 0;
            virtual void LogSync(const std::string& log) = 0;

            virtual void Shutdown() = 0;

            template<typename T>
            T* GetComponent(const std::string& name) {
                return dynamic_cast<T*>(FindComponent(name));
            }

        protected:
            virtual iComponent* FindComponent(const std::string& name) = 0;
        };
    }
}

#define TRACE(engine, format, ...) {\
    char log[512] = {0};\
    sprintf_s(log, sizeof(log), "TRACE LOG : %s:%d >>>> "#format, __FILE__, __LINE__, ##__VA_ARGS__);\
    engine->LogAsync(log);\
}


#endif //__iEngine_h__
