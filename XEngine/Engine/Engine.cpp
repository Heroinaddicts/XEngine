#include "Engine.h"
#include "MultiSys.h"
#include "Logic/Logic.h"
#include "TimeWheel/TimeWheel.h"
#include "Navigation/Navigation.h"
#include "Physics/Physics.h"

#include "SafeString.h"
#include "SafeSystem.h"
#include <map>

#ifdef WIN32
#include "Net/Windows/Net.h"
#endif //WIN32

#ifdef Linux
#include "Net/Linux/Net.h"
#endif //Linux

static std::map<std::string, std::string> s_ParameterMap;

XEngine::iLogic* g_Logic = nullptr;
XEngine::iNet* g_Net = nullptr;
XEngine::iTimeWheel* g_Timewheel = nullptr;
XEngine::iNavigation* g_Navigation = nullptr;
XEngine::iPhysics* g_Physics = nullptr;

static int s_FixedTimeStep = 33333;

namespace XEngine {
    Engine* Engine::GetInstance() {
        static Engine s_Engine;
        return &s_Engine;
    }
    const char* Engine::GetLaunchParameter(const std::string& name) {
        auto itor = s_ParameterMap.find(name);
        if (itor != s_ParameterMap.end()) {
            return itor->second.c_str();
        }

        return nullptr;
    }

    Api::iNetApi* Engine::GetNetApi() {
        return g_Net;
    }

    Api::iTimerApi* Engine::GetTimerApi() {
        return g_Timewheel;
    }

    Api::iNavigationApi* Engine::GetNavigationApi() {
        return g_Navigation;
    }

    Api::iPhysicsApi* Engine::GetPhysicsApi() {
        return g_Physics;
    }

    float Engine::GetFixedTimeStep() {
        return s_FixedTimeStep / 1000.0f;
    }

    void Engine::LogAsync(const std::string& log) {
        printf("%s\n", log.c_str());
    }

    void Engine::LogSync(const std::string& log) {
#ifdef _DEBUG
        printf("%s\n", log.c_str());
#endif //_DEBUG
    }

    void Engine::Shutdown() {

    }

    bool Engine::isShutdown() {
        return false;
    }

    Api::iComponent* Engine::FindComponent(const std::string& name) {
        return g_Logic->FindComponent(name);
    }
}

bool AnalysisLaunchParameters(const int argc, const char** args, const char** env) {
    for (int i = 1; i < argc; ++i) {
        if (strncmp(args[i], "--", 2) == 0) {
            const char* start = args[i] + 2;
            const char* equal = strstr(start, "=");
            if (equal != nullptr) {
                std::string name(start, equal);
                std::string val(equal + 1);
                s_ParameterMap[name] = val;
            }
            else if (strlen(args[i]) > 2) {
                s_ParameterMap[args[i] + 2] = "";
            }
        }
        else {
            return false;
        }
    }

    return true;
}

int main(int argc, const char** args, const char** env) {
    if (!AnalysisLaunchParameters(argc, args, env)) {
        XASSERT(false, "AnalysisLaunchParameters error");
        return 0;
    }

    XEngine::Engine* engine = XEngine::Engine::GetInstance();
    const char* fixedTimeStepStr = engine->GetLaunchParameter("fixedTimeStep");
    if (fixedTimeStepStr) {
        s_FixedTimeStep = XEngine::SafeString::StringToFloat(fixedTimeStepStr) * 1000;
    }

    {
        g_Logic = XEngine::Logic::GetInstance();
        g_Net = XEngine::Net::GetInstance();
        g_Timewheel = XEngine::TimeWheel::GetInstance();
        g_Navigation = XEngine::Navigation::GetInstance();
        g_Physics = XEngine::Physics::GetInstance();
    }


    { // Initialize
        g_Physics->Initialize(engine);
        g_Navigation->Initialize(engine);
        g_Timewheel->Initialize(engine);
        g_Net->Initialize(engine);
        g_Logic->Initialize(engine);
    }

    { // Launche
        g_Physics->Launch(engine);
        g_Navigation->Launch(engine);
        g_Timewheel->Launch(engine);
        g_Net->Launch(engine);
        g_Logic->Launch(engine);
    }

    unsigned_int64 tick = XEngine::SafeSystem::Time::GetMicroSecond();
    while (!engine->isShutdown()) {
        g_Navigation->EarlyUpdate(engine);
        g_Timewheel->EarlyUpdate(engine);
        g_Net->EarlyUpdate(engine);
        g_Logic->EarlyUpdate(engine);
        g_Physics->EarlyUpdate(engine);

        g_Navigation->Update(engine);
        g_Timewheel->Update(engine);
        g_Net->Update(engine);
        g_Logic->Update(engine);
        g_Physics->Update(engine);

        g_Navigation->LaterUpdate(engine);
        g_Timewheel->LaterUpdate(engine);
        g_Logic->LaterUpdate(engine);
        g_Physics->LaterUpdate(engine);
        g_Net->LaterUpdate(engine);

        unsigned_int64 tick2 = XEngine::SafeSystem::Time::GetMicroSecond();
        if (tick2 - tick >= s_FixedTimeStep) {
            g_Navigation->FixedUpdate(engine);
            g_Timewheel->FixedUpdate(engine);
            g_Net->FixedUpdate(engine);
            g_Logic->FixedUpdate(engine);
            g_Physics->FixedUpdate(engine);
            tick += s_FixedTimeStep;
        }
        else {
            XEngine::SafeSystem::Time::MillisecondSleep(0);
        }
    }

    { // Release
        g_Logic->Release(engine);
        g_Net->Release(engine);
    }

    return 0;
}
