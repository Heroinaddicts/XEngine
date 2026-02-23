#ifndef __iEngine_h__
#define __iEngine_h__

#include "SafeString.h"

#include "iNetApi.h"
#include "iHttpApi.h"
#include "iTimerApi.h"
#include "iZipApi.h"
#include "iTaskApi.h"
#include "iMysqlApi.h"
#include "iRedisApi.h"
#include "iWebSocketsApi.h"

namespace XEngine {
    namespace Api {
        class iModule;
        typedef UInt64 ProcessHandle;
        class iEngine {
        public:
            virtual ~iEngine() {}

            virtual const char* GetName() const = 0;

            UInt16 GetLaunchParameterUInt16(const std::string& name) const {
                return SafeString::StringToUInt16(GetLaunchParameter(name));
            }

            Int16 GetLaunchParameterInt16(const std::string& name) const {
                return SafeString::StringToInt16(GetLaunchParameter(name));
            }

            UInt32 GetLaunchParameterUInt32(const std::string& name) const {
                return SafeString::StringToUInt32(GetLaunchParameter(name));
            }

            Int32 GetLaunchParameterInt32(const std::string& name) const {
                return SafeString::StringToInt32(GetLaunchParameter(name));
            }

            UInt64 GetLaunchParameterUInt64(const std::string& name) const {
                return SafeString::StringToUInt64(GetLaunchParameter(name));
            }

            Int64 GetLaunchParameterInt64(const std::string& name) const {
                return SafeString::StringToInt64(GetLaunchParameter(name));
            }

            virtual const char* GetLaunchParameter(const std::string& name) const = 0;

            virtual iNetApi* GetNetApi() const = 0;
            virtual iHttpApi* GetHttpApi() const = 0;
            virtual iTimerApi* GetTimerApi() const = 0;
            virtual iZipApi* GetZipApi() const = 0;
            virtual iTaskApi* GetTaskApi() const = 0;
            virtual iMysqlApi* GetMysqlApi() const = 0;
            virtual iRedisApi* GetRedisApi() const = 0;
            virtual iWebSocketsApi* GetWebSocketsApi() const = 0;

            virtual void LogAsync(const char* header, const char* content, const bool console, const char* file, const int line) = 0;
            virtual void LogSync(const char* header, const char* content, const bool console, const char* file, const int line) = 0;

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

    __forceinline void __Log__(Api::iEngine* engine, const char* tag, const bool sync, const char* file, int line, const char* format, ...) {
        va_list args;
        va_start(args, format);
        va_list args_copy;
        va_copy(args_copy, args);

        int len = vsnprintf(nullptr, 0, format, args_copy);
        va_end(args_copy);

        if (len <= 0) {
            va_end(args);
            return;
        }

        char* log = (char*)txmalloc(len + 1);
        if (!log) {
            va_end(args);
            return;
        }

        vsnprintf(log, len + 1, format, args);
        va_end(args);

        sync ? engine->LogSync(tag, log, true, file, line) : engine->LogAsync(tag, log, true, file, line);
        txfree(log);
    }
}


#ifdef _DEBUG
#define DEBUG(engine, format, ...) printf("Debug %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(engine, format, ...) printf("Debug %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif //_DEBUG

#define TraceLog(engine, format, ...) __Log__(engine, "Trace", false, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define OtherLog(engine, format, ...) __Log__(engine, "Other", false, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define ErrorLog(engine, format, ...) {__Log__(engine, "Error", true, __FILE__, __LINE__, format, ##__VA_ARGS__); XASSERT(false, format, ##__VA_ARGS__);}

#define SYSERR(engine, format, ...) __Log__(engine, "SysErr", true, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define IMPORTANT(engine, format, ...) __Log__(engine, "Important", true, __FILE__, __LINE__, format, ##__VA_ARGS__)

#endif //__iEngine_h__
