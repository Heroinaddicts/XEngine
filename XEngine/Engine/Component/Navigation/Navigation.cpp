#include "Navigation.h"
#include "Navmesh.h"
#include "SafeString.h"

namespace XEngine {
    Api::iEngine* g_engine = nullptr;
    iNavigation* Navigation::GetInstance() {
        static Navigation static_navigtaion;
        return &static_navigtaion;
    }
    bool Navigation::Initialize(Api::iEngine* const engine) {
        g_engine = engine;
        return true;
    }

    bool Navigation::Launch(Api::iEngine* const engine) {
        int thread_count = 4;
        const char* navmesh_load_thread_count = engine->GetLaunchParameter("navmesh_load_thread_count");
        if (navmesh_load_thread_count) {
            thread_count = SafeString::StringToInt(navmesh_load_thread_count);
            if (0 == thread_count) {
                thread_count = 4;
            }
        }

        Start(thread_count);
        return true;
    }

    void Navigation::Release(Api::iEngine* const engine) {
    }

    void Navigation::EarlyUpdate(Api::iEngine* const engine) {
    }

    void Navigation::Update(Api::iEngine* const engine) {
    }

    void Navigation::FixedUpdate(Api::iEngine* const engine) {

    }

    void Navigation::LaterUpdate(Api::iEngine* const engine) {
        AsyncMeshLoader loader;
        while (_loaded_queue.Pull(loader)) {
            if (loader.mesh) {
                _mesh_map.insert(std::make_pair(loader.mesh->_file, loader.mesh));
            }

            loader.call(loader.mesh);
        }
    }

    Api::iNavmesh* Navigation::LoadNavmeshSync(const std::string& file) {
        Navmesh* mesh = xnew Navmesh(file);
        if (!mesh->Load()) {
            xdel mesh;
            return nullptr;
        }

        _mesh_map.insert(std::make_pair(file, mesh));
        return mesh;
    }

    void Navigation::LoadNavmeshAsync(const std::string& file, void(* const func)(Api::iNavmesh* mesh)) {
        auto itor = _mesh_map.find(file);
        if (itor != _mesh_map.end()) {
            func(itor->second);
            return;
        }

        Navmesh* mesh = xnew Navmesh(file);
        if (false == _load_queue.Push(AsyncMeshLoader(mesh, func))) {
            XERROR(g_engine, "async load mesh queue is full, load %s error", file.c_str());
            xdel mesh;
            mesh = nullptr;
            func(nullptr);
        }
    }

    void Navigation::ReleaseNavmesh(Api::iNavmesh* mesh) {

    }

    void Navigation::Run(void* constext) {
        AsyncMeshLoader loader;
        while (true) {
            bool ret = false;
            {
                AUTO_LOCK(_pull_lock);
                ret = _load_queue.Pull(loader);
            }

            if (ret) {
                if (!loader.mesh->Load()) {
                    xdel loader.mesh;
                    loader.mesh = nullptr;
                }

                AUTO_LOCK(_push_lock);
                _loaded_queue.Push(loader);
            }
            else {
                SafeSystem::Time::MillisecondSleep(1);
            }
        }
    }
}


