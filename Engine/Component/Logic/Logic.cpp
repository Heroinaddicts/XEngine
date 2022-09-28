#include "Logic.h"
#include "SafeFile.h"
#include "SafeString.h"
#include "MultiSys.h"
#include <vector>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif //linux

namespace XEngine {
    typedef Api::iComponent* (*__GetComponents)(void);


    bool Logic::Initialize(Api::iEngine* const engine) {
        const char* component_path = engine->GetLaunchParameter("component_path");
        component_path ? _ComponentPath = component_path : _ComponentPath = SafeFile::GetApplicationPath();

        const char* components_arg = engine->GetLaunchParameter("components");
        if (components_arg) {
            std::vector<std::string> names;
            int count = SafeString::Split(std::string(components_arg), ",", names);

            for (int i = 0; i < count; i++) {
#ifdef WIN32
                std::string path = _ComponentPath + "/" + names[i] + ".dll";
                HINSTANCE hinst = ::LoadLibrary(path.c_str());
                if (!hinst) {
                    XERROR(engine, "load %s error %s\n", path.c_str(), strerror(::GetLastError()));
                    return false;
                }

                __GetComponents fun = (__GetComponents)::GetProcAddress(hinst, "GetComponents");
#else
#endif //WIN32
                XASSERT(fun, "Can not export dll function GetComponents, dll %s", path.c_str());
                Api::iComponent* component = fun();
                if (false == component->Initialize(engine)) {
                    XERROR(engine, "Component %s Initialize failed", component->_name.c_str());
                    return false;
                }

                XLOG(engine, "Component %s Initialized", component->_name.c_str());
                _ComponentMap.insert(std::make_pair(component->_name, component));
            }
        }

        return false;
    }

    bool Logic::Launch(Api::iEngine* const engine) {
        for (auto itor = _ComponentMap.begin(); itor != _ComponentMap.end(); itor++) {
            if (false == itor->second->Launch(engine)) {
                XERROR(engine, "Component %s Launch failed", itor->second->_name.c_str());
                return false;
            }
            XLOG(engine, "Component %s Launched", itor->second->_name.c_str());
        }

        return true;
    }

    void Logic::Release(Api::iEngine* const engine) {
        for (auto itor = _ComponentMap.begin(); itor != _ComponentMap.end(); itor++) {
            if (false == itor->second->Destroy(engine)) {
                XERROR(engine, "component %s Destroy failed", itor->second->_name.c_str());
            }
        }
    }

    void Logic::EarlyUpdate(Api::iEngine* const engine) {

    }

    void Logic::Update(Api::iEngine* const engine) {

    }

    void Logic::LaterUpdate(Api::iEngine* const engine) {

    }
}
