using System;
using System.Threading;
using XUtils;

namespace XEngine
{
    class Task : XApi.ITask
    {
        public class TaskContext : XApi.ITask.ITaskContext
        {
            Int64 _Code = -1;
            object _Context = null;
            public long GetCode()
            {
                return _Code;
            }

            public T GetContext<T>()
            {
                return (T)_Context;
            }

            public void SetCode(long code)
            {
                _Code = code;
            }

            public void SetContext<T>(T context)
            {
                _Context = context;
            }
        }

        public class TaskInfo
        {
            readonly public Action<XApi.ITask.ITaskContext> _Task;
            readonly public Action<XApi.ITask.ITaskContext> _Callback;
            readonly public TaskGroup _Group;

            public TaskInfo(Action<XApi.ITask.ITaskContext> task, Action<XApi.ITask.ITaskContext> callback, TaskGroup group)
            {
                _Task = task;
                _Callback = callback;
                _Group = group;
            }
        }

        public class TaskReturn
        {
            readonly public TaskInfo _Info;
            readonly public TaskContext _Context;

            public TaskReturn(TaskInfo info, TaskContext context)
            {
                _Info = info;
                _Context = context;
            }
        }

        readonly int _CoreCount = 0;

        SPSCQueue<TaskInfo>[] _TaskMaskQueue = null;
        SPSCQueue<TaskInfo> _TaskQueue = new SPSCQueue<TaskInfo>();
        SPSCQueue<TaskReturn> _TaskReturnQueue = new SPSCQueue<TaskReturn>();

        public SPSCQueue<TaskInfo> TaskQueue => _TaskQueue;
        public SPSCQueue<TaskReturn> TaskReturnQueue => _TaskReturnQueue;

        Thread[] _Runers;

        public Task()
        {
            _CoreCount = Environment.ProcessorCount;
            _TaskMaskQueue = new SPSCQueue<TaskInfo>[_CoreCount];
            _Runers = new Thread[_CoreCount];
            for (int i = 0; i < _CoreCount; i++)
            {
                _TaskMaskQueue[i] = new SPSCQueue<TaskInfo>();
                _Runers[i] = new Thread(new ParameterizedThreadStart(this.Run));
                _Runers[i].Start(i);
            }
        }

        ~Task()
        {
            for (int i = 0; i < Environment.ProcessorCount; i++)
            {
                _Runers[i].Abort();
                _Runers[i] = null;
            }

            _Runers = null;
        }

        public XApi.ITask.ITaskGroup CreateTaskGroup(Action completedCallback)
        {
            return new TaskGroup(this, completedCallback);
        }

        public void StartTask(XApi.ITask.eTaskType taskType, Action<XApi.ITask.ITaskContext> task, Action<XApi.ITask.ITaskContext> callback, int mask)
        {
            switch (taskType)
            {
                case XApi.ITask.eTaskType.OrderedQueue:
                    {
                        if (mask != XApi.ITask.InvaildMask)
                        {
                            _TaskMaskQueue[mask % _CoreCount].Push(new TaskInfo(task, callback, null));
                        }
                        else
                        {
                            _TaskQueue.Push(new TaskInfo(task, callback, null));
                        }
                        break;
                    }
                case XApi.ITask.eTaskType.SeparateThread:
                    {
                        new Thread(() =>
                        {
                            TaskContext context = new TaskContext();
                            try
                            {
                                task(context);
                            }
                            catch (Exception e)
                            {
                                XApi.Error(e.Message);
                            }

                            _TaskReturnQueue.Push(
                                new TaskReturn(
                                    new TaskInfo(task, callback, null),
                                    context
                                )
                            );
                        }).Start();
                        break;
                    }
            }
        }

        void Run(object c)
        {
            int index = (int)c;

            SPSCQueue<TaskInfo> maskQueue = _TaskMaskQueue[index];

            TaskInfo info = null;
            while (true)
            {
                while (maskQueue.Pull(out info))
                {
                    TaskContext context = new TaskContext();
                    try
                    {
                        info._Task(context);
                    }
                    catch (Exception e)
                    {
                        XApi.Error(e.Message);
                    }
                    _TaskReturnQueue.Push(
                        new TaskReturn(
                            info, context
                        )
                    );
                }

                if (_TaskQueue.Pull(out info))
                {
                    TaskContext context = new TaskContext();
                    try
                    {
                        info._Task(context);
                    }
                    catch (Exception e)
                    {
                        XApi.Error(e.Message);
                    }
                    _TaskReturnQueue.Push(
                        new TaskReturn(
                            info, context
                        )
                    );
                }
                else
                {
                    Thread.Sleep(10);
                }
            }
        }

        public void Update()
        {
            TaskReturn ret = null;
            while (_TaskReturnQueue.Pull(out ret))
            {
                ret._Info._Callback(ret._Context);
                if (ret._Info._Group != null)
                {
                    try
                    {
                        ret._Info._Group.TaskDone();
                    }
                    catch (Exception e)
                    {
                        XApi.Error(e.Message);
                    }
                }
            }
        }
    }
}
