#ifndef __TaskGroup_h__
#define __TaskGroup_h__

#include "Header.h"

namespace XEngine {
    class TaskGroup : public Api::iTaskGroup {
    public:
        virtual ~TaskGroup() {}

        static TaskGroup* Create();
        static void Release(TaskGroup* const group);

        // Inherited via iTaskGroup
        void AddTask(Api::fTask task, Api::fTaskCompleted callback) override;
        void SetTasksAllCompletedCallback(Api::fTasksAllCompleted callback) override;
        void Start(const Api::eTaskProcessorType type) override;

    private:
        friend XPool<TaskGroup>;
        TaskGroup() : _TasksAllCompletedCallback(nullptr), _AlreadyStart(false), _TaskDealCount(0) {}

    private:
        std::list<TaskInfo*> _TaskList;
        Api::fTasksAllCompleted _TasksAllCompletedCallback;
        bool _AlreadyStart;
        int _TaskDealCount;

        SpinLock _Lock;
    };
}

#endif //__TaskGroup_h__
