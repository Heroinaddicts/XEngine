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
            //idΪ�����߼���indexΪ��Ϸ��ʼ�������ܹ��߹�������֡���� offsetΪ��ǰ��ʱ���Ѿ��߹���֡����context����������
            void OnStart(in int id, in int index, in int offset, in object context);
            void OnTime(in int id, in int index, in int offset, in object context);
            //natural��trueΪ����Ȼ��������ʱ��repeated�����ˣ���falseΪ��kill������ɱ�����µ�OnEnd
            void OnEnd(in int id, in int index, in int offset, in object context, in bool natural);
        }
    }
}
