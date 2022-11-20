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
        Navigation() {}

        std::map<std::string, Navmesh*> _MeshMap;

        struct AsyncMeshLoader {
            Navmesh* _Mesh;
            void(*_Call)(Api::iNavmesh* mesh);
            AsyncMeshLoader() : _Mesh(nullptr), _Call(nullptr) {}
            AsyncMeshLoader(Navmesh* mesh_, void(* const call_)(Api::iNavmesh* mesh)) : _Mesh(mesh_), _Call(call_) {}
        };
        SafeQueue::SpscQueue<AsyncMeshLoader, MESHLOAD_QUEUE_SIZE> _LoadQueue;
        SafeQueue::SpscQueue<AsyncMeshLoader, MESHLOAD_QUEUE_SIZE> _LoadedQueue;

        SpinLock _PullLock;
        SpinLock _PushLock;
    };
}

#endif //__Navigation_h__