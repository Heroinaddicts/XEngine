#ifndef __iEngine_h__
#define __iEngine_h__

#include <string>

namespace XEngine {
    namespace Api {
        class iModule;

        class iNetApi;
        class iTimerApi;
        class iNavigationApi;
        class iPhysicsApi;
        class iGameObjectApi;

        class iEngine {
        public:
            virtual ~iEngine() {}

            virtual const char* GetLaunchParameter(const std::string& name) = 0;

            virtual iNetApi* GetNetApi() const = 0;
            virtual iTimerApi* GetTimerApi() const = 0;
            virtual iNavigationApi* GetNavigationApi() const = 0;
            virtual iPhysicsApi* GetPhysicsApi() const = 0;
            virtual iGameObjectApi* GetGameObjectApi() const = 0;

            virtual float GetFixedTimeStep() = 0;

            virtual void LogAsync(const std::string& log) = 0;
            virtual void LogSync(const std::string& log) = 0;

            virtual void Shutdown() = 0;

            template<typename T>
            T* GetModule(const std::string& name) {
                return dynamic_cast<T*>(FindModule(name));
            }

        protected:
            virtual iModule* FindModule(const std::string& name) = 0;
        };
    }
}

#define TRACE(engine, format, ...) {\
    char log[1024] = {0};\
    sprintf_s(log, sizeof(log), "Trace : %s:%d >>>> "#format, __FILE__, __LINE__, ##__VA_ARGS__);\
    engine->LogAsync(log);\
}

#define ERROR(engine, format, ...) {\
    char log[1024] = {0}; \
    sprintf_s(log, sizeof(log), "Error : %s:%d >>>> "#format, __FILE__, __LINE__, ##__VA_ARGS__);\
    engine->LogSync(log);\
}

#endif //__iEngine_h__
