#include "TestXEngine.h"

iEngine* g_Engine = nullptr;
TestXEngine* g_TestXEngine = nullptr;

bool TestXEngine::Initialize(iEngine* const engine) {
    g_TestXEngine = this;
    g_Engine = engine;
    return true;
}

bool TestXEngine::Launch(iEngine* const engine) {
    //--modules=Connecter --name=Gate --node_id=10001 --remote_nodes=127.0.0.1:4499

    std::map<std::string, std::string> args;
    args.insert(std::make_pair("modules", "Connecter"));
    args.insert(std::make_pair("name", "Gate"));
    args.insert(std::make_pair("node_id", "10001"));
    args.insert(std::make_pair("remote_nodes", "127.0.0.1:4499"));

    g_Engine->LaunchXEngineProcess(args);
    return true;
}

bool TestXEngine::Destroy(iEngine* const engine) {
    return true;
}
