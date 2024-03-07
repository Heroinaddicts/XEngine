#include "Launch.h"
#include "SafeString.h"
#include "SafeSystem.h"

#include "Engine.h"
#include "iEngineComponent.h"

#include "TinyXML/tinyxml2.h"


namespace XEngine {
    Launch* Launch::GetInstance() {
        static Launch s_Instance;
        return &s_Instance;
    }

    bool Launch::AnalysisLaunchParameters(const int argc, const char** args, const char** env) {
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
                        _ParameterMap[name] = val;
                        TRACE(g_Engine, "Launch args %s=%s", name.c_str(), val.c_str());
                    }
                    else if (strlen(parms[index].c_str()) > 2) {
                        _ParameterMap[parms[index].c_str() + 2] = "";
                        TRACE(g_Engine, "Launch args %s", parms[index].c_str() + 2);
                    }
                }
            }
        }

        const char* coreXml = GetLaunchParameter("core_xml");
        if (coreXml) {
            std::string xmlPath = SafeSystem::File::GetDirectoryOfCurrentExe() + "/" + coreXml;
            tinyxml2::XMLDocument doc;
            if (tinyxml2::XMLError::XML_SUCCESS != doc.LoadFile(xmlPath.c_str())) {
                XASSERT(false, "load core xml %s error", xmlPath.c_str());
                XERROR(Engine::GetInstance(), "load core xml %s error", xmlPath.c_str());
                return false;
            }

            tinyxml2::XMLElement* root = doc.RootElement();
            XASSERT(root, "wtf");
            if (root) {
                _Name = root->Attribute("name");

                tinyxml2::XMLElement* modules = root->FirstChildElement("modules");
                if (modules) {
                    tinyxml2::XMLElement* module = modules->FirstChildElement("module");
                    while (module) {
                        _ModuleNames.push_back(module->Attribute("name"));
                        module = module->NextSiblingElement("module");
                    }
                }
            }
            else {
                XERROR(Engine::GetInstance(), "core xml %s format error", xmlPath.c_str());
                return false;
            }
        }
        else {
            const char* modules = GetLaunchParameter("modules");
            if (modules) {
                SafeString::Split(modules, ";", _ModuleNames);
            }
            const char* name = GetLaunchParameter("name");
            _Name = name ? name : "XEngine";
        }

        return true;
    }

    const char* Launch::GetLaunchParameter(const std::string& name) const {
        auto itor = _ParameterMap.find(name);
        if (itor != _ParameterMap.end()) {
            return itor->second.c_str();
        }

        return nullptr;
    }

    const char* Launch::GetName() const {
        return _Name.c_str();
    }

    const std::vector<std::string>& Launch::GetModuleNames() const {
        return _ModuleNames;
    }
}
