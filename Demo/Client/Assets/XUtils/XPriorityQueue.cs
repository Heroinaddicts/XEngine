using System;
using System.Collections.Generic;

namespace XUtils
{
    public class XPriorityQueue<T>
    {
        private readonly List<T> _Heap;
        private readonly IComparer<T> _Comparer;

        public int Count => _Heap.Count;

        public XPriorityQueue() : this(Comparer<T>.Default) { }

        public XPriorityQueue(IComparer<T> comparer)
        {
            _Heap = new List<T>();
            _Comparer = comparer;
        }

        public void Clear()
        {
            _Heap.Clear();
        }

        public void Push(T item)
        {
            _Heap.Add(item);
            SiftUp(_Heap.Count - 1);
        }

        public T Pop()
        {
            if (_Heap.Count == 0)
                throw new InvalidOperationException("Heap is empty");

            T root = _Heap[0];
            int lastIndex = _Heap.Count - 1;
            _Heap[0] = _Heap[lastIndex];
            _Heap.RemoveAt(lastIndex);

            if (_Heap.Count > 0)
                SiftDown(0);

            return root;
        }

        public T Peek()
        {
            if (_Heap.Count == 0)
                throw new InvalidOperationException("Heap is empty");
            return _Heap[0];
        }

        private void SiftUp(int index)
        {
            while (index > 0)
            {
                int parent = (index - 1) / 2;
                if (_Comparer.Compare(_Heap[index], _Heap[parent]) >= 0)
                    break;

                Swap(index, parent);
                index = parent;
            }
        }

        private void SiftDown(int index)
        {
            int last = _Heap.Count - 1;
            while (true)
            {
                int left = index * 2 + 1;
                int right = index * 2 + 2;
                int smallest = index;

                if (left <= last && _Comparer.Compare(_Heap[left], _Heap[smallest]) < 0)
                    smallest = left;
                if (right <= last && _Comparer.Compare(_Heap[right], _Heap[smallest]) < 0)
                    smallest = right;

                if (smallest == index)
                    break;

                Swap(index, smallest);
                index = smallest;
            }
        }

        private void Swap(int a, int b)
        {
            T temp = _Heap[a];
            _Heap[a] = _Heap[b];
            _Heap[b] = temp;
        }
    }
}