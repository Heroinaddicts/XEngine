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
    typedef Api::iModule* (*__GetModules)(void);


    iLogic* Logic::GetInstance() {
        static Logic s_Logic;
        return &s_Logic;
    }

    Api::iModule* Logic::FindModule(const std::string& name) {
        auto itor = _ModuleMap.find(name);
        if (_ModuleMap.end() != itor) {
            return itor->second;
        }

        return nullptr;
    }

    bool Logic::Initialize(Api::iEngine* const engine) {
        const char* module_path = engine->GetLaunchParameter("module_path");
        module_path ? _ModulePath = module_path : _ModulePath = SafeSystem::File::GetsPathOfCurrentExecutable();

        const char* modules = engine->GetLaunchParameter("modules");
        if (modules) {
            std::vector<std::string> names;
            int count = SafeString::Split(modules, ";", names);

            for (int i = 0; i < count; i++) {
#ifdef WIN32
                std::string path = _ModulePath + "/" + names[i] + ".dll";
                HINSTANCE hinst = ::LoadLibrary(path.c_str());
                if (!hinst) {
                    XERROR(engine, "load %s error %s\n", path.c_str(), strerror(::GetLastError()));
                    return false;
                }

                __GetModules fun = (__GetModules)::GetProcAddress(hinst, GET_MODULES_FUNC_NAME);
#else
                std::string path = _ModulePath + "/lib" + names[i] + ".so";
                void* handle = dlopen(path.c_str(), RTLD_LAZY);
                if (!handle) {
                    XERROR(engine, "load %s error %s\n", path.c_str(), errno);
                    return false;
                }

                __GetModules fun = (__GetModules)dlsym(handle, GET_MODULES_FUNC_NAME);
#endif //WIN32
                XASSERT(fun, "Can not export dll function GetModules, dll %s", path.c_str());
                Api::iModule* module = fun();
                if (false == module->Initialize(engine)) {
                    XERROR(engine, "Module %s Initialize failed", module->_Name);
                    return false;
                }

                XLOG(engine, "Module %s Initialized", module->_Name);
                _ModuleMap.insert(std::make_pair(module->_Name, module));
            }
        }

        return true;
    }

    bool Logic::Launch(Api::iEngine* const engine) {
        for (auto itor = _ModuleMap.begin(); itor != _ModuleMap.end(); itor++) {
            if (false == itor->second->Launch(engine)) {
                XERROR(engine, "Module %s Launch failed", itor->second->_Name);
                return false;
            }
            XLOG(engine, "Module %s Launched", itor->second->_Name);
        }

        return true;
    }

    void Logic::Release(Api::iEngine* const engine) {
        for (auto itor = _ModuleMap.begin(); itor != _ModuleMap.end(); itor++) {
            if (false == itor->second->Destroy(engine)) {
                XERROR(engine, "Module %s Destroy failed", itor->second->_Name);
            }
        }
    }

    void Logic::EarlyUpdate(Api::iEngine* const engine) {

    }

    int64 tick = SafeSystem::Time::GetMilliSecond();
    void Logic::Update(Api::iEngine* const engine) {
        int64 temp = SafeSystem::Time::GetMilliSecond() - tick;
        if (temp > 5) {
            //XLOG(engine, "%lld", temp);
        }
        tick = SafeSystem::Time::GetMilliSecond();
    }

    void Logic::FixedUpdate(Api::iEngine* const engine) {

    }

    void Logic::LaterUpdate(Api::iEngine* const engine) {

    }
}
