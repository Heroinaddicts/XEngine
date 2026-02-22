#include "Config.h"
#include "AutoConfigData.h"

Api::iEngine* g_Engine = nullptr;
static Config* s_Self = nullptr;

bool Config::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool Config::Launch(Api::iEngine* const engine) {
    return true;
}

bool Config::Destroy(Api::iEngine* const engine) {
    return true;
}

const AutoConfig::IAutoConfig* Config::GetAutoConfig() const {
    return AutoConfig::GetAutoConfig();
}
