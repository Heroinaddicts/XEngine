#ifndef __TaskManager_h__
#define __TaskManager_h__

#include "Header.h"

namespace XEngine {
    class TaskManager : public iTaskManager, public SafeThread {
    public:
        virtual ~TaskManager() {}

        static TaskManager* GetInstance();

        // Inherited via iTaskManager
        bool Initialize(Api::iEngine* const engine) override;
        bool Launch(Api::iEngine* const engine) override;
        void Release(Api::iEngine* const engine) override;
        void EarlyUpdate(Api::iEngine* const engine) override;
        void Update(Api::iEngine* const engine) override;
        void LaterUpdate(Api::iEngine* const engine) override;


        void PushTask(const Api::eTaskProcessorType processorType, Api::fTask task, Api::fTaskCompleted callback, const Int64 mask) override;
        Api::iTaskGroup* CreateTaskGroup() override;

        // Inherited via SafeThread
        void Run(void* context) override;

    private:
        int _NumberOfCpuCore;
        SafeQueue::SpscQueue<TaskInfo*> * _TaskReqMaskQueue;
    };
}

#endif //__TaskManager_h__
