#ifndef __iEngine_h__
#define __iEngine_h__

#include <string>

namespace XEngine {
    namespace Api {
        class iNetApi;
        class iTimerApi;

        class iEngine {
        public:
            virtual ~iEngine() {}

            virtual const char* GetLaunchParameter(const std::string& name) = 0;

            virtual iNetApi* GetNetApi() = 0;
            virtual iTimerApi* GetTimerApi() = 0;

            virtual void LogAsync(const std::string& log) = 0;
            virtual void LogSync(const std::string& log) = 0;

            virtual void Shutdown() = 0;
        };
    }
}

#endif //__iEngine_h__
