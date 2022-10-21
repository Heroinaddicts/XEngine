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
#include "Net/windows/Net.h"
#endif //WIN32

#ifdef Linux
#include "Net/Linux/Net.h"
#endif //Linux


static std::map<std::string, std::string> static_parameter_map;

XEngine::iLogic* g_logic = nullptr;
XEngine::iNet* g_net = nullptr;
XEngine::iTimeWheel* g_timewheel = nullptr;
XEngine::iNavigation* g_navigation = nullptr;
XEngine::iPhysics* g_physics = nullptr;

static int static_fixed_time_step = 33333;

namespace XEngine {
    Engine* Engine::GetInstance() {
        static Engine engine;
        return &engine;
    }
    const char* Engine::GetLaunchParameter(const std::string& name) {
        auto itor = static_parameter_map.find(name);
        if (itor != static_parameter_map.end()) {
            return itor->second.c_str();
        }

        return nullptr;
    }

    Api::iNetApi* Engine::GetNetApi() {
        return g_net;
    }

    Api::iTimerApi* Engine::GetTimerApi() {
        return g_timewheel;
    }

    Api::iNavigationApi* Engine::GetNavigationApi() {
        return g_navigation;
    }

    Api::iPhysicsApi* Engine::GetPhysicsApi() {
        return g_physics;
    }

    float Engine::GetFixedTimeStep() {
        return static_fixed_time_step / 1000.0f;
    }

    void Engine::LogAsync(const std::string& log) {
#ifdef _DEBUG
        printf("%s\n", log.c_str());
#endif //_DEBUG
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
        return g_logic->FindComponent(name);
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
                static_parameter_map[name] = val;
            }
            else if (strlen(args[i]) > 2) {
                static_parameter_map[args[i] + 2] = "";
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
        static_fixed_time_step = XEngine::SafeString::StringToFloat(fixedTimeStepStr) * 1000;
    }

    {
        g_logic = XEngine::Logic::GetInstance();
        g_net = XEngine::Net::GetInstance();
        g_timewheel = XEngine::TimeWheel::GetInstance();
        g_navigation = XEngine::Navigation::GetInstance();
        g_physics = XEngine::Physics::GetInstance();
    }


    { // Initialize
        g_physics->Initialize(engine);
        g_navigation->Initialize(engine);
        g_timewheel->Initialize(engine);
        g_net->Initialize(engine);
        g_logic->Initialize(engine);
    }

    { // Launche
        g_physics->Launch(engine);
        g_navigation->Launch(engine);
        g_timewheel->Launch(engine);
        g_net->Launch(engine);
        g_logic->Launch(engine);
    }

    unsigned_int64 tick = XEngine::SafeSystem::Time::GetMicroSecond();
    while (!engine->isShutdown()) {
        g_navigation->EarlyUpdate(engine);
        g_timewheel->EarlyUpdate(engine);
        g_net->EarlyUpdate(engine);
        g_logic->EarlyUpdate(engine);
        g_physics->EarlyUpdate(engine);

        g_navigation->Update(engine);
        g_timewheel->Update(engine);
        g_net->Update(engine);
        g_logic->Update(engine);
        g_physics->Update(engine);

        g_navigation->LaterUpdate(engine);
        g_timewheel->LaterUpdate(engine);
        g_logic->LaterUpdate(engine);
        g_physics->LaterUpdate(engine);
        g_net->LaterUpdate(engine);

		unsigned_int64 tick2 = XEngine::SafeSystem::Time::GetMicroSecond();
		if (tick2 - tick >= static_fixed_time_step) {
			g_navigation->FixedUpdate(engine);
			g_timewheel->FixedUpdate(engine);
			g_net->FixedUpdate(engine);
			g_logic->FixedUpdate(engine);
			g_physics->FixedUpdate(engine);
			tick += static_fixed_time_step;
        }
        else {
            XEngine::SafeSystem::Time::MillisecondSleep(0);
        }
    }

    { // Release
        g_logic->Release(engine);
        g_net->Release(engine);
    }

    return 0;
}
