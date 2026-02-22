using System;
using System.Threading;

namespace XUtils
{
    public interface IXBuffer
    {
        void Write(in byte[] data, in int offset, in int len);
        int GetLength();
        bool Read(Func<byte[], int, int, int> func);
        bool Out(int length);
        void Clear();
        void SetLogFun(Action<string> logFun);
    }

    public class SPSCBuffer : IXBuffer
    {
        byte[] _Buffer;
        readonly int _Capacity; // 扩容步长
        int _Size;
        int _In;
        int _Out;

        Action<string> _Logger;

        object _Lock = new object();

        public void SetLogFun(Action<string> logFun)
        {
            _Logger = logFun;
        }

        void Log(in string log)
        {
            if (_Logger != null)
            {
                _Logger(log);
            }
        }

        public SPSCBuffer(in int size = 1024, in int capacity = 512)
        {
            _Buffer = new byte[size];
            _Capacity = capacity;
            _Size = size;
            _In = 0;
            _Out = 0;
            _Logger = null;
        }

        // ---------- 修复：Clear 用 Volatile.Write（轻量） ----------
        public void Clear()
        {
            Volatile.Write(ref _In, 0);
            Volatile.Write(ref _Out, 0);
        }

        // ---------- 修复：用 Volatile.Read 保证可见性 ----------
        public int GetLength()
        {
            return Volatile.Read(ref _In) - Volatile.Read(ref _Out);
        }

        // ---------- 修复：Out 不需要锁（SPSC），使用本地读取再写入 ----------
        public bool Out(int length)
        {
            int available = GetLength();
            if (length > available)
            {
                Log($"Out length {length} > GetLength {available}");
                return false;
            }

            int outPos = Volatile.Read(ref _Out);
            Volatile.Write(ref _Out, outPos + length);
            return true;
        }

        // ---------- Read: 保留 lock，但使用原子读并在锁内调用（与你原实现一致）
        // 注意：func 在 lock 内执行会阻塞写入的扩容路线；若 func 可能耗时，见下面给出的非阻塞替代选项 ----------
        public bool Read(Func<byte[], int, int, int> func)
        {
            int ret = 0;

            lock (_Lock)
            {
                int outPos = Volatile.Read(ref _Out);
                int inPos = Volatile.Read(ref _In);
                int available = inPos - outPos;
                if (available <= 0) return false;

                ret = func(_Buffer, outPos, available);
                if (ret > 0)
                {
                    Volatile.Write(ref _Out, outPos + ret);
                }
            }

            return ret > 0;
        }

        // ---------- 用 Volatile.Read 保证这些计算用的是最新值 ----------
        int GetDataSize => Volatile.Read(ref _In) - Volatile.Read(ref _Out);
        int GetTailSize => _Size - Volatile.Read(ref _In);
        int GetFreeSize => _Size - GetDataSize;

        public void Write(in byte[] data, in int offset, in int len)
        {
            if (len <= 0) return;

            // 先判断尾部是否足够
            if (GetTailSize < len)
            {
                lock (_Lock)
                {
                    // 进入 lock 之后重新采样所有相关量
                    int inPos = Volatile.Read(ref _In);
                    int outPos = Volatile.Read(ref _Out);
                    int dataSize = inPos - outPos; // 本地保存，避免重复读取导致不一致
                    int tailSize = _Size - inPos;
                    int freeSize = _Size - dataSize;

                    if (tailSize < len)
                    {
                        if (freeSize < len)
                        {
                            // ---- 扩容：先计算新的大小，然后拷贝到新数组 ----
                            byte[] old = _Buffer;
                            int newSize = _Size;
                            do
                            {
                                newSize += _Capacity;
                                // 可选：你可以在这里检查 newSize 是否溢出或超过某个上限
                            } while (newSize - dataSize < len);

                            byte[] newBuf = new byte[newSize];
                            // 使用本地 outPos/dataSize 确保一致性
                            Array.Copy(old, outPos, newBuf, 0, dataSize);

                            _Buffer = newBuf;
                            _Size = newSize;
                        }
                        else
                        {
                            // ---- 搬移（in-place）: 把数据搬到头部，保证 [_Out.._In) 连续 ----
                            if (inPos > outPos)
                            {
                                Array.Copy(_Buffer, outPos, _Buffer, 0, dataSize);
                            }
                        }

                        // 重置指针：_In = dataSize, _Out = 0
                        Volatile.Write(ref _In, dataSize);
                        Volatile.Write(ref _Out, 0);
                    }
                }
            }

            // 写数据：先读取当前 _In 到本地（生产者独占 _In，所以不会和其它写冲突）
            int inStart = Volatile.Read(ref _In);
            Array.Copy(data, offset, _Buffer, inStart, len);
            Volatile.Write(ref _In, inStart + len);
        }
    }
}
