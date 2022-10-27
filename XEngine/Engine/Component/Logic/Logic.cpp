#include "Logic.h"
#include "SafeSystem.h"
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


    iLogic* Logic::GetInstance() {
        static Logic s_Logic;
        return &s_Logic;
    }

    Api::iComponent* Logic::FindComponent(const std::string& name) {
        auto itor = _ComponentMap.find(name);
        if (_ComponentMap.end() != itor) {
            return itor->second;
        }

        return nullptr;
    }

    bool Logic::Initialize(Api::iEngine* const engine) {
        const char* component_path = engine->GetLaunchParameter("component_path");
        component_path ? _ComponentPath = component_path : _ComponentPath = SafeSystem::File::GetApplicationPath();

        const char* components = engine->GetLaunchParameter("components");
        if (components) {
            std::vector<std::string> names;
            int count = SafeString::Split(components, ";", names);

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
                std::string path = _ComponentPath + "/" + names[i] + ".so";
                void* handle = dlopen(path.c_str(), RTLD_LAZY);
                if (!handle) {
                    XERROR(engine, "load %s error %s\n", path.c_str(), errno);
                    return false;
                }

                __GetComponents fun = (__GetComponents)dlsym(handle, "GetComponents");
#endif //WIN32
                XASSERT(fun, "Can not export dll function GetComponents, dll %s", path.c_str());
                Api::iComponent* component = fun();
                if (false == component->Initialize(engine)) {
                    XERROR(engine, "Component %s Initialize failed", component->_Name);
                    return false;
                }

                XLOG(engine, "Component %s Initialized", component->_Name);
                _ComponentMap.insert(std::make_pair(component->_Name, component));
            }
        }

        return true;
    }

    bool Logic::Launch(Api::iEngine* const engine) {
        for (auto itor = _ComponentMap.begin(); itor != _ComponentMap.end(); itor++) {
            if (false == itor->second->Launch(engine)) {
                XERROR(engine, "Component %s Launch failed", itor->second->_Name);
                return false;
            }
            XLOG(engine, "Component %s Launched", itor->second->_Name);
        }

        return true;
    }

    void Logic::Release(Api::iEngine* const engine) {
        for (auto itor = _ComponentMap.begin(); itor != _ComponentMap.end(); itor++) {
            if (false == itor->second->Destroy(engine)) {
                XERROR(engine, "component %s Destroy failed", itor->second->_Name);
            }
        }
    }

    void Logic::EarlyUpdate(Api::iEngine* const engine) {

    }

    int64 tick = SafeSystem::Time::GetMilliSecond();
    void Logic::Update(Api::iEngine* const engine) {
        int64 temp = SafeSystem::Time::GetMilliSecond() - tick;
        if (temp > 5) {
            XLOG(engine, "%lld", temp);
        }
        tick = SafeSystem::Time::GetMilliSecond();
    }

    void Logic::FixedUpdate(Api::iEngine* const engine) {

    }

    void Logic::LaterUpdate(Api::iEngine* const engine) {

    }
}
