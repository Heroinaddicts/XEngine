#include "TaskManager.h"
#include "TaskGroup.h"
#include "Engine.h"
#include "XLock.h"
#include "SafeString.h"

namespace XEngine {

    XPool<TaskInfo, 1, 10240> g_TaskInfoPool;

    SafeQueue::SpscQueue<TaskInfo*, 102400> g_TaskReqQueue;
    SafeQueue::SpscQueue<TaskInfo*, 102400> g_TaskAswQueue;

    TaskManager* g_TaskManager = nullptr;

    TaskManager* TaskManager::GetInstance() {
        static TaskManager s_Instance;
        return &s_Instance;
    }

    bool TaskManager::Initialize(Api::iEngine* const engine) {
        g_TaskManager = this;
        _NumberOfCpuCore = SafeSystem::HardWare::NumberOfCpuCores();

        if (engine->GetLaunchParameter("TaskThreadCount")) {
            int count = engine->GetLaunchParameterInt32("TaskThreadCount");
            if (count > 0) {
                _NumberOfCpuCore = count;
            }
        }

        _TaskReqMaskQueue = txnew SafeQueue::SpscQueue<TaskInfo*>[_NumberOfCpuCore];
        for (int i = 0; i < _NumberOfCpuCore; i++) {
            Start(1, (void*)i);
        }

        return true;
    }

    bool TaskManager::Launch(Api::iEngine* const engine) {
        return false;
    }

    void TaskManager::Release(Api::iEngine* const engine) {
    }

    void TaskManager::EarlyUpdate(Api::iEngine* const engine) {
    }

    void TaskManager::Update(Api::iEngine* const engine) {
        TaskInfo* info = nullptr;
        UInt64 tick = SafeSystem::Time::GetMicroSecond();
        while (g_TaskAswQueue.Pull(info)) {
            info->_TaskCompleted(info->_Code, info->_Context);
            XPOOL_RELEASE(g_TaskInfoPool, info);
            info = nullptr;
            if (SafeSystem::Time::GetMicroSecond() - tick > 1000) {
                break;
            }
        }
    }

    void TaskManager::LaterUpdate(Api::iEngine* const engine) {
    }

    void TaskManager::PushTask(const Api::eTaskProcessorType processorType, Api::fTask task, Api::fTaskCompleted callback, const Int64 mask) {
        switch (processorType) {
        case Api::eTaskProcessorType::OrderedQueue: {
            if (NOT_USE_MASK == mask) {
                g_TaskReqQueue.Push(XPOOL_CREATE(g_TaskInfoPool, task, callback, nullptr,
                    __FILE__,
                    __LINE__));
            }
            else {
                _TaskReqMaskQueue[mask % _NumberOfCpuCore].Push(XPOOL_CREATE(g_TaskInfoPool, task, callback, nullptr,
                    __FILE__,
                    __LINE__));
            }

            break;
        }
        case Api::eTaskProcessorType::SeparateThread: {
            TaskInfo* info = XPOOL_CREATE(g_TaskInfoPool, task, callback, nullptr,
                __FILE__,
                __LINE__);
            SafeThread::StartThread(
                [=](void* context) {
                    info->_Code = info->_Task(info->_Context);
                    g_TaskAswQueue.Push(info);
                },
                nullptr);
            break;
        }
        }
    }

    Api::iTaskGroup* TaskManager::CreateTaskGroup() {
        return TaskGroup::Create();
    }

    void TaskManager::Run(void* context) {
        int index = (int)((Int64)context);

        while (!g_Engine->isShutdown() || !g_TaskReqQueue.Empty()) {
            TaskInfo* task = nullptr;
            while (_TaskReqMaskQueue[index].Pull(task)) {
                task->_Code = task->_Task(task->_Context);
                g_TaskAswQueue.Push(task);
            }

            while (g_TaskReqQueue.Pull(task)) {
                task->_Code = task->_Task(task->_Context);
                g_TaskAswQueue.Push(task);
            }

            SafeSystem::Time::MillisecondSleep(5);
        }
    }
}
