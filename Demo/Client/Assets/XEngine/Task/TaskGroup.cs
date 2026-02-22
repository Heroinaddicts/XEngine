using System;
using System.Threading;
using static XEngine.XApi.ITask;

namespace XEngine
{
    class TaskGroup : XApi.ITask.ITaskGroup
    {
        int _TaskCount = 0;
        int _DoneCount = 0;

        readonly Task _Task = null;
        readonly Action _TaskAllCompletedCallback = null;

        public TaskGroup(Task task, Action completedCallback)
        {
            _Task = task;
            _TaskAllCompletedCallback = completedCallback;
        }

        public void AddTask(XApi.ITask.eTaskType taskType, Action<ITaskContext> task, Action<ITaskContext> callback)
        {
            switch (taskType)
            {
                case XApi.ITask.eTaskType.OrderedQueue:
                    {
                        _Task.TaskQueue.Push(new Task.TaskInfo(task, callback, this));
                        break;
                    }
                case XApi.ITask.eTaskType.SeparateThread:
                    {
                        new Thread(() =>
                        {
                            new Thread(() =>
                            {
                                Task.TaskContext context = new Task.TaskContext();
                                try
                                {
                                    task(context);
                                }
                                catch (Exception e)
                                {
                                    XApi.Error(e.Message);
                                }

                                _Task.TaskReturnQueue.Push(
                                    new Task.TaskReturn(
                                        new Task.TaskInfo(task, callback, this),
                                        context
                                    )
                                );
                            }).Start();
                        }).Start();
                        break;
                    }
            }

            _TaskCount++;
        }

        public void TaskDone()
        {
            _DoneCount++;

            if (_DoneCount == _TaskCount)
            {
                if (null != _TaskAllCompletedCallback)
                {
                    _TaskAllCompletedCallback();
                }
            }
        }
    }
}
