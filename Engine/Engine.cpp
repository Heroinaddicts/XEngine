#include "Engine.h"
#include "MultiSys.h"
#include "Logic/Logic.h"
#include <map>

static std::map<std::string, std::string> static_parameter_map;

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


XEngine::Logic* g_logic = nullptr;

int main(int argc, const char** args, const char** env) {
    if (!AnalysisLaunchParameters(argc, args, env)) {
        XASSERT(false, "AnalysisLaunchParameters error");
        return 0;
    }

    XEngine::Engine* engine = XEngine::Engine::GetInstance();

    {
        g_logic = x_new XEngine::Logic();
    }


    { // Initialize
        g_logic->Initialize(engine);
    }

    { // Launche
        g_logic->Launch(engine);
    }

    while (!engine->isShutdown()) {
        g_logic->EarlyUpdate(engine);

        g_logic->Update(engine);

        g_logic->LaterUpdate(engine);
    }

    { // Release

        g_logic->Release(engine);
    }

    return 0;
}
