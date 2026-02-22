#include "Engine.h"
#include "MultiSys.h"
#include "Logic/Logic.h"
#include "Log/Log.h"
#include "TimeWheel/TimeWheel.h"
#include "Http/Http.h"
#include "Mysql/Mysql.h"
#include "Redis/RedisClient.h"
#include "Launch.h"
//#include "WebSockets/WebSockets.h"
#ifdef WIN32
#include "Net/Windows/Net.h"
#endif //WIN32

#ifdef Linux
#include "Net/Linux/Net.h"
#endif //Linux

#include "Task/TaskManager.h"

#include "SafeString.h"
#include "SafeSystem.h"
#include <map>


namespace XEngine {
    Engine* Engine::GetInstance() {
        static Engine s_Instance;
        return &s_Instance;
    }

    Engine* const g_Engine = Engine::GetInstance();

    const char* Engine::GetName() const {
        return Launch::GetInstance()->GetName();
    }

    const char* Engine::GetLaunchParameter(const std::string& name) const {
        return Launch::GetInstance()->GetLaunchParameter(name);
    }

    Api::iNetApi* Engine::GetNetApi() const {
        return Net::GetInstance();
    }

    Api::iHttpApi* Engine::GetHttpApi() const {
        return Http::GetInstance();
    }

    Api::iTimerApi* Engine::GetTimerApi() const {
        return TimeWheel::GetInstance();
    }

    Api::iZipApi* Engine::GetZipApi() const {
        return nullptr;
    }

    Api::iTaskApi* Engine::GetTaskApi() const {
        return TaskManager::GetInstance();
    }

    Api::iMysqlApi* Engine::GetMysqlApi() const {
        return Mysql::GetInstance();
    }

    Api::iRedisApi* Engine::GetRedisApi() const {
        return RedisClient::GetInstace();
    }

    Api::iWebSocketsApi* Engine::GetWebSocketsApi() const {
        return nullptr; //WebSockets::GetInstance();
    }

    void Engine::LogAsync(const char* header, const char* content, const bool console, const char* file, const int line) {
        Log::GetInstance()->LogAsync(header, content, console, file, line);
    }

    void Engine::LogSync(const char* header, const char* content, const bool console, const char* file, const int line) {
        Log::GetInstance()->LogSync(header, content, console, file, line);
    }

    Api::ProcessHandle Engine::LaunchXEngineProcess(const std::map<std::string, std::string>& launchParameters) const {
        std::string cmd;
        for (auto itor = launchParameters.begin(); itor != launchParameters.end(); itor++) {
            cmd += " --" + itor->first + "=" + itor->second;
        }

        return SafeSystem::Process::LaunchProcess(SafeSystem::File::GetCurrentExecutablePath(), cmd);
    }

    Api::iModule* Engine::FindModule(const std::string& name) {
        return Logic::GetInstance()->FindModule(name);
    }
}

