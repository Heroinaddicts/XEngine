#include "Launcher.h"
#include <map>
#include <vector>
#include "SafeString.h"

UsingXEngine;

static std::map<std::string, std::string> s_LauncherArgs;

Launcher* Launcher::GetInstance() {
    static Launcher s_Launcher;
    return &s_Launcher;
}

void Launcher::ParseLauncherArgs(int argc, const char** args) {
    for (int i = 1; i < argc; ++i) {
        std::vector<std::string> parms;

        int count = XEngine::SafeString::Split(args[i], " ", parms);
        for (int index = 0; index < parms.size(); index++) {
            if (strncmp(parms[index].c_str(), "--", 2) == 0) {
                const char* start = parms[index].c_str() + 2;
                const char* equal = strstr(start, "=");
                if (equal != nullptr) {
                    std::string name(start, equal);
                    std::string val(equal + 1);
                    s_LauncherArgs[name] = val;
                    printf("Launch args %s=%s\n", name.c_str(), val.c_str());
                }
                else if (strlen(parms[index].c_str()) > 2) {
                    s_LauncherArgs[parms[index].c_str() + 2] = "";
                    printf("Launch args %s\n", parms[index].c_str() + 2);
                }
            }
        }
    }
}

const char* Launcher::GetLauncherArg(const std::string& name) const {
    auto itor = s_LauncherArgs.find(name);
    if (itor == s_LauncherArgs.end()) {
        return nullptr;
    }

    return itor->second.c_str();
}
