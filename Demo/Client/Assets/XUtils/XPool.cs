using System;
using System.Collections.Concurrent;
using System.Threading;

namespace XUtils
{
    public class XPool<T>
    {
        public delegate void ResetRefDelegate<T>(ref T item);

        private readonly ConcurrentBag<T> _Objects;
        private readonly Func<T> _ObjectGenerator;
        private readonly Action<T> _ObjectReset;
        private readonly int _MaxSize;
        private int _Count;


        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="objectGenerator">用于生成新实例的委托，不能为空</param>
        /// <param name="objectReset">回收时执行的重置逻辑，可为 null</param>
        /// <param name="maxSize">池中最大容量，默认为 int.MaxValue</param>
        public XPool(Func<T> objectGenerator, Action<T> objectReset = null, int maxSize = int.MaxValue)
        {
            _ObjectGenerator = objectGenerator ?? throw new ArgumentNullException(nameof(objectGenerator));
            _ObjectReset = objectReset;
            _MaxSize = maxSize > 0 ? maxSize : throw new ArgumentOutOfRangeException(nameof(maxSize));
            _Objects = new ConcurrentBag<T>();
            _Count = 0;
        }

        /// <summary>
        /// 获取一个对象：有可用则复用并重置，否则新建
        /// </summary>
        public T Get()
        {
            if (_Objects.TryTake(out var item))
            {
                // 减少计数，调用重置
                Interlocked.Decrement(ref _Count);
                _ObjectReset?.Invoke(item);
                return item;
            }
            // 池为空时生成新实例
            return _ObjectGenerator();
        }

        /// <summary>
        /// 回收一个对象：如果未超出最大容量则加入池中，否则丢弃
        /// </summary>
        public void Return(T item)
        {
            if (item == null) throw new ArgumentNullException(nameof(item));

            // 增加计数并判断是否超出上限
            if (Interlocked.Increment(ref _Count) <= _MaxSize)
            {
                _Objects.Add(item);
            }
            else
            {
                // 超出容量，恢复计数，丢弃对象
                Interlocked.Decrement(ref _Count);
            }
        }

        /// <summary>
        /// 当前池中可用对象数量（近似值）
        /// </summary>
        public int Count => Volatile.Read(ref _Count);
    }
}
