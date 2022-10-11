/*
* File:   Navigation.h
* Author : max
*
* Created on 2022-09-05 11:58:14
*/

#ifndef __Navigation_h__
#define __Navigation_h__

#include "Header.h"
#include "SafeQueue.h"
#include "SafeThread.h"
#include "XLock.h"

namespace XEngine {
#define MESHLOAD_QUEUE_SIZE 1024

    class Navmesh;
    class Navigation : public iNavigation, public SafeThread {
    public:
        virtual ~Navigation() {}

        static iNavigation* GetInstance();

        // 通过 iEngineComponent 继承
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;

        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void FixedUpdate(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        // 通过 iNavigationApi 继承
        virtual Api::iNavmesh* LoadNavmeshSync(const std::string& file) override;
        virtual void LoadNavmeshAsync(const std::string& file, void(* const func)(Api::iNavmesh* mesh)) override;
        virtual void ReleaseNavmesh(Api::iNavmesh* mesh) override;

    protected:
        virtual void Run(void* constext);

    private:
        Navigation() : _load_queue(MESHLOAD_QUEUE_SIZE), _loaded_queue(MESHLOAD_QUEUE_SIZE) {}

        std::map<std::string, Navmesh*> _mesh_map;

        struct AsyncMeshLoader {
            Navmesh* mesh;
            void(*call)(Api::iNavmesh* mesh);
            AsyncMeshLoader() : mesh(nullptr), call(nullptr) {}
            AsyncMeshLoader(Navmesh* mesh_, void(* const call_)(Api::iNavmesh* mesh)) : mesh(mesh_), call(call_) {}
        };
        SafeQueue::SpscQueue<AsyncMeshLoader> _load_queue;
        SafeQueue::SpscQueue<AsyncMeshLoader> _loaded_queue;

        SpinLock _pull_lock;
        SpinLock _push_lock;
    };
}

#endif //__Navigation_h__