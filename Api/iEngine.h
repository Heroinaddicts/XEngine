#ifndef __iEngine_h__
#define __iEngine_h__

#include "MultiSys.h"

#include <string>
#include <map>

namespace XEngine {
    namespace Api {
        class iModule;

        class iNetApi;
        class iHttpApi;
        class iTimerApi;
        class iZipApi;

        typedef UInt64 ProcessHandle;

        class iEngine {
        public:
            virtual ~iEngine() {}

            virtual const char* GetName() const = 0;
            virtual const char* GetLaunchParameter(const std::string& name) const = 0;

            virtual iNetApi* GetNetApi() const = 0;
            virtual iHttpApi* GetHttpApi() const = 0;
            virtual iTimerApi* GetTimerApi() const = 0;
            virtual iZipApi* GetZipApi() const = 0;

            virtual void LogAsync(const char* header, const char* content, const char* file, const int line) = 0;
            virtual void LogSync(const char* header, const char* content, const char* file, const int line) = 0;

            virtual void Shutdown() = 0;
            virtual ProcessHandle LaunchXEngineProcess(const std::map<std::string, std::string>& launchParameters = std::map<std::string, std::string>()) const = 0; //ProcessHandle == 0, launch failed

            template<typename T>
            T* GetModule(const std::string& name) {
                return dynamic_cast<T*>(FindModule(name));
            }
        protected:
            virtual iModule* FindModule(const std::string& name) = 0;
        };
    }
}



#ifdef _DEBUG
#define DEBUG(engine, format, ...) {\
    char log[1024] = {0};\
    sprintf_s(log, sizeof(log), format, ##__VA_ARGS__);\
    engine->LogAsync("Debug", log, __FILE__, __LINE__);\
}
#else
#define DEBUG(engine, format, ...)
#endif //_DEBUG

#define TRACE(engine, format, ...) {\
    char log[8192] = {0};\
    sprintf_s(log, sizeof(log), format, ##__VA_ARGS__);\
    engine->LogAsync("Trace", log, __FILE__, __LINE__);\
}

#define ERROR(engine, format, ...) {\
    char log[1024] = {0}; \
    sprintf_s(log, sizeof(log), format, ##__VA_ARGS__); \
    engine->LogSync("Error", log, __FILE__, __LINE__); \
}

#define SYSERR(engine, format, ...) {\
    char log[1024] = {0}; \
    sprintf_s(log, sizeof(log), format, ##__VA_ARGS__);\
    engine->LogSync("System-Error", log, __FILE__, __LINE__);\
}

#define IMPORTANT(engine, format, ...) {\
    char log[1024] = {0}; \
    sprintf_s(log, sizeof(log), format, ##__VA_ARGS__);\
    engine->LogSync("Important", log, __FILE__, __LINE__);\
}

#endif //__iEngine_h__
