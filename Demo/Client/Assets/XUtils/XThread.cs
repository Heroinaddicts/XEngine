using System.Threading;

namespace XUtils
{
    public abstract class XThread
    {
        bool _Stop = false;

        public void Start(object c = null)
        {
            new Thread(new ParameterizedThreadStart(__ThreadRun__)).Start(c);
        }

        public void Stop()
        {
            _Stop = true;
        }

        private void __ThreadRun__(object c)
        {
            while (!_Stop)
            {
                Run(c);
            }
        }

        protected abstract void Run(object c);
    }
}
