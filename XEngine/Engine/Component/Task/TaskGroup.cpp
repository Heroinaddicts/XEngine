#include "TaskGroup.h"
#include "TaskManager.h"
#include "Engine.h"

namespace XEngine {
    static XPool<TaskGroup> s_TaskGroupPool;

    TaskGroup* TaskGroup::Create() {
        return XPOOL_CREATE(s_TaskGroupPool);
    }

    void TaskGroup::Release(TaskGroup* const group) {
        DEBUG(g_Engine, "Release TaskGroup %llx", group);
        XPOOL_RELEASE(s_TaskGroupPool, group);
    }

    void TaskGroup::AddTask(Api::fTask task, Api::fTaskCompleted callback) {
        if (_AlreadyStart) {
            ErrorLog(g_Engine, "Group already start");
            return;
        }

        _TaskList.push_back(XPOOL_CREATE(g_TaskInfoPool,
            task,
            [=](const Int64 code, const Api::iTaskContext& context) {
                callback(code, context);
                _TaskDealCount--;
                if (_TaskDealCount == 0) {
                    if (_TasksAllCompletedCallback) {
                        _TasksAllCompletedCallback();
                    }
                    Release(this);
                }
            }, this,
            __FILE__,
                __LINE__));
    }

    void TaskGroup::SetTasksAllCompletedCallback(Api::fTasksAllCompleted callback) {
        if (_AlreadyStart) {
            ErrorLog(g_Engine, "Group already start");
            return;
        }
        _TasksAllCompletedCallback = callback;
    }

    void TaskGroup::Start(const Api::eTaskProcessorType type) {
        if (_AlreadyStart) {
            ErrorLog(g_Engine, "try to start a group which is already started");
            return;
        }

        _AlreadyStart = true;

        _TaskDealCount = _TaskList.size();
        if (_TaskDealCount == 0) {
            if (_TasksAllCompletedCallback) {
                _TasksAllCompletedCallback();
            }

            Release(this);
            return;
        }

        switch (type) {
        case Api::eTaskProcessorType::OrderedQueue: {
            for (auto itor = _TaskList.begin(); itor != _TaskList.end(); itor++) {
                g_TaskReqQueue.Push(*itor);
            }
            break;
        }
        case Api::eTaskProcessorType::SeparateThread: {
            for (auto itor = _TaskList.begin(); itor != _TaskList.end(); itor++) {
                SafeThread::StartThread(
                    [&](void* context) {
                        TaskInfo* info = (TaskInfo*)context;
                        if (info) {
                            info->_Code = info->_Task(info->_Context);
                            g_TaskAswQueue.Push(info);
                        }
                    },
                    *itor
                );
            }
            break;
        }
        }
    }
}
