namespace TCore
{
    namespace Api
    {
        public abstract class iModule
        {
            public readonly string _name;
            protected iModule(in string name)
            {
                _name = name;
            }

            public abstract bool Initialize();
            public abstract bool Launch();
            public abstract void Release();
        }

        public interface iPipe
        {

        }

        public interface iTcpPipe : iPipe
        {
            public void Send(in byte[] data, in int offset, in int length, in bool immediately = true);
            public void Close();
        }

        public abstract class iTcpSession
        {
            public iTcpPipe _pipe;

            public abstract void OnConnect(in bool r);
            public abstract int OnRecv(in byte[] data, in int offset, in int length);
            public abstract void OnDisconnected();

            public void Send(in byte[] data, in int offset, in int length, in bool immediately = true)
            {
                if (null != _pipe)
                {
                    _pipe.Send(data, offset, length, immediately);
                }
            }

            public void Close()
            {
                if (null != _pipe)
                {
                    _pipe.Close();
                }
            }
        }


        public interface iTimer
        {
            //id为区分逻辑，index为游戏开始到现在总共走过的数据帧数， offset为当前定时器已经走过的帧数，context关联上下文
            void OnStart(in int id, in int index, in int offset, in object context);
            void OnTime(in int id, in int index, in int offset, in object context);
            //natural：true为否自然消亡（定时器repeated跑完了），false为被kill，主动杀掉导致的OnEnd
            void OnEnd(in int id, in int index, in int offset, in object context, in bool natural);
        }
    }
}
