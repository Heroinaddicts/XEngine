#include "Engine.h"
#include "MultiSys.h"
#include "Logic/Logic.h"
#include "TimeWheel/TimeWheel.h"
#include "Navigation/Navigation.h"
#include "Physics/Physics.h"
#include "GameObjectManager/GameObjectManager.h"

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
XEngine::iGameObjectManager* g_GameObjectManager = nullptr;

static int s_FixedTimeStep = 33333;



namespace XEngine {
    Engine* Engine::GetInstance() {
        static Engine s_Instance;
        return &s_Instance;
    }

    Engine* const g_Engine = Engine::GetInstance();

    const char* Engine::GetLaunchParameter(const std::string& name) {
        auto itor = s_ParameterMap.find(name);
        if (itor != s_ParameterMap.end()) {
            return itor->second.c_str();
        }

        return nullptr;
    }

    Api::iNetApi* Engine::GetNetApi() const {
        return g_Net;
    }

    Api::iTimerApi* Engine::GetTimerApi() const {
        return g_Timewheel;
    }

    Api::iNavigationApi* Engine::GetNavigationApi() const {
        return g_Navigation;
    }

    Api::iPhysicsApi* Engine::GetPhysicsApi() const {
        return g_Physics;
    }

    Api::iGameObjectApi* Engine::GetGameObjectApi() const {
        return g_GameObjectManager;
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

    Api::iModule* Engine::FindModule(const std::string& name) {
        return g_Logic->FindModule(name);
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

class A {
public:
    virtual ~A() {}
    A() {

    }
};

class B : public A {
public:
    virtual ~B() {}
};

void Test() {
    int i = 100;
    const type_info& t1 = typeid(i);

    //type_info::type_info info;
    printf(t1.name());
    printf("=====");
    printf(t1.raw_name());

    A* b = new B();

    const type_info& tb = typeid(*b);
    printf("test %s\n", tb.name());
}

int main(int argc, const char** args, const char** env) {
    Test();

    if (!AnalysisLaunchParameters(argc, args, env)) {
        XASSERT(false, "AnalysisLaunchParameters error");
        return 0;
    }

    const char* fixedTimeStepStr = XEngine::g_Engine->GetLaunchParameter("fixedTimeStep");
    if (fixedTimeStepStr) {
        s_FixedTimeStep = XEngine::SafeString::StringToFloat(fixedTimeStepStr) * 1000;
    }

    {
        g_Logic = XEngine::Logic::GetInstance();
        g_Net = XEngine::Net::GetInstance();
        g_Timewheel = XEngine::TimeWheel::GetInstance();
        g_Navigation = XEngine::Navigation::GetInstance();
        g_Physics = XEngine::Physics::GetInstance();
        g_GameObjectManager = XEngine::GameObjectManager::GetInstance();
    }


    { // Initialize
        g_Physics->Initialize(XEngine::g_Engine);
        g_Navigation->Initialize(XEngine::g_Engine);
        g_Timewheel->Initialize(XEngine::g_Engine);
        g_Net->Initialize(XEngine::g_Engine);
        g_Logic->Initialize(XEngine::g_Engine);
        g_GameObjectManager->Initialize(XEngine::g_Engine);
    }

    { // Launche
        g_Physics->Launch(XEngine::g_Engine);
        g_Navigation->Launch(XEngine::g_Engine);
        g_Timewheel->Launch(XEngine::g_Engine);
        g_Net->Launch(XEngine::g_Engine);
        g_Logic->Launch(XEngine::g_Engine);
        g_GameObjectManager->Launch(XEngine::g_Engine);
    }

    unsigned_int64 tick = XEngine::SafeSystem::Time::GetMicroSecond();
    while (!XEngine::g_Engine->isShutdown()) {
        g_Navigation->EarlyUpdate(XEngine::g_Engine);
        g_Timewheel->EarlyUpdate(XEngine::g_Engine);
        g_Net->EarlyUpdate(XEngine::g_Engine);
        g_Logic->EarlyUpdate(XEngine::g_Engine);
        g_GameObjectManager->EarlyUpdate(XEngine::g_Engine);
        g_Physics->EarlyUpdate(XEngine::g_Engine);

        g_Navigation->Update(XEngine::g_Engine);
        g_Timewheel->Update(XEngine::g_Engine);
        g_Net->Update(XEngine::g_Engine);
        g_Logic->Update(XEngine::g_Engine);
        g_GameObjectManager->Update(XEngine::g_Engine);
        g_Physics->Update(XEngine::g_Engine);

        unsigned_int64 tick2 = XEngine::SafeSystem::Time::GetMicroSecond();
        if (tick2 - tick >= s_FixedTimeStep) {
            g_Navigation->FixedUpdate(XEngine::g_Engine);
            g_Timewheel->FixedUpdate(XEngine::g_Engine);
            g_Net->FixedUpdate(XEngine::g_Engine);
            g_Logic->FixedUpdate(XEngine::g_Engine);
            g_GameObjectManager->FixedUpdate(XEngine::g_Engine);
            g_Physics->FixedUpdate(XEngine::g_Engine);
            tick += s_FixedTimeStep;
        }

        g_Navigation->LaterUpdate(XEngine::g_Engine);
        g_Timewheel->LaterUpdate(XEngine::g_Engine);
        g_Logic->LaterUpdate(XEngine::g_Engine);
        g_GameObjectManager->LaterUpdate(XEngine::g_Engine);
        g_Physics->LaterUpdate(XEngine::g_Engine);
        g_Net->LaterUpdate(XEngine::g_Engine);
    }

    { // Release
        g_Navigation->Release(XEngine::g_Engine);
        g_Timewheel->Release(XEngine::g_Engine);
        g_Logic->Release(XEngine::g_Engine);
        g_GameObjectManager->Release(XEngine::g_Engine);
        g_Physics->Release(XEngine::g_Engine);
        g_Net->Release(XEngine::g_Engine);
    }


    return 0;
}
