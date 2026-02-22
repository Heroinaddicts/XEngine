#ifndef __TaskHeader_h__
#define __TaskHeader_h__

#include "iTaskManager.h"
#include "SafeSystem.h"
#include "SafeThread.h"
#include "SafeQueue.h"

#include "XPool.h"

#include <list>

namespace XEngine {
    class TaskGroup;

    class TaskContext : public Api::iTaskContext {
    public:
        virtual ~TaskContext() {}

        TaskContext() : _Context(nullptr) {}

        // Í¨¹ý iTaskContext ¼Ì³Ð
        void* GetContext() const override {
            return _Context;
        }

        void SetContext(void* const context) override {
            _Context = context;
        }
    private:
        void* _Context;
    };

    struct TaskInfo {
        const char* _File;
        const int _Line;

        const Api::fTask _Task;
        const Api::fTaskCompleted _TaskCompleted;
        TaskGroup* const _Group;
        Int64 _Code;
        TaskContext _Context;

        TaskInfo(const Api::fTask& task, const Api::fTaskCompleted& callback, TaskGroup* const group, const char* file, const int line)
            : _Task(task), _TaskCompleted(callback), _Group(group), _Code(-1), _File(file), _Line(line) {
        }
    };

    extern XPool<TaskInfo, 1, 10240> g_TaskInfoPool;

    extern SafeQueue::SpscQueue<TaskInfo*, 102400> g_TaskReqQueue;
    extern SafeQueue::SpscQueue<TaskInfo*, 102400> g_TaskAswQueue;

    class TaskManager;
    extern TaskManager* g_TaskManager;
}

#endif //__TaskHeader_h__
