#ifndef __SafeQueue_h__
#define __SafeQueue_h__

#include "XLock.h"
#include "SafeSystem.h"

namespace XEngine {
    namespace SafeQueue {
        template<typename T>
        class iQueue {
        public:
            virtual ~iQueue() {}

            virtual void Push(const T&) = 0;
            virtual bool Pull(OUT T&) = 0;
            virtual int Count() = 0;

            virtual bool Empty() {
                return Count() == 0;
            }
        };

        enum eDataState {
            NoData = 0,
            HasData = 1
        };

        template<typename T>
        struct QueueSpace {
            std::atomic<uint8_t> _Sign; // 0: NoData, 1: HasData
            T _Data;
            QueueSpace() : _Sign(0) {}
        };

        template<typename T, int Size = 512, int Capacity = 512>
        class SpscQueue : public iQueue<T> {
        public:
            SpscQueue()
                : _Size(Size), _ReadIndex(0), _WriteIndex(0),
                _ReadCount(0), _WriteCount(0) {
                _QueuePoint = _Queue;
            }
            ~SpscQueue() {
                if (_QueuePoint != _Queue) {
                    txfree(_QueuePoint);
                    _QueuePoint = nullptr;
                }
            }

            void Push(const T& o) override {
                AUTO_LOCK(_PushLock);

                // 若当前位置非空，扩容（与 Pull 互斥）
                if (_QueuePoint[_WriteIndex]._Sign.load(std::memory_order_acquire) != 0) {
                    AUTO_LOCK(_CapacityLock);

                    if (_QueuePoint[_WriteIndex]._Sign.load(std::memory_order_acquire) != 0) {
                        const int newSize = _Size + Capacity;
                        auto* old = _QueuePoint;
                        auto* neo = (QueueSpace<T>*)txmalloc(newSize * sizeof(QueueSpace<T>));
                        // 显式初始化新空间的标志为 NoData
                        for (int i = 0; i < newSize; ++i) neo[i]._Sign.store(0, std::memory_order_relaxed);

                        // 以“读取顺序”重排旧环到新数组
                        const int oldSize = _Size;
                        const int r = _ReadIndex;           // 与 Pull 在 _CapacityLock 下互斥，可直接读
                        for (int i = 0; i < oldSize; ++i) {
                            int idx = r + i;
                            if (idx >= oldSize) idx -= oldSize;

                            if (old[idx]._Sign.load(std::memory_order_acquire) == 1) {
                                neo[i]._Data = old[idx]._Data; // 这里只做拷贝/赋值
                                neo[i]._Sign.store(1, std::memory_order_release);
                            }
                        }
                        _QueuePoint = neo;
                        _ReadIndex = 0;
                        _WriteIndex = oldSize;  // 满队列：新写入点放在已占用区尾
                        _Size = newSize;

                        if (old != _Queue) txfree(old);
                    }
                }

                // 正常写入：先写数据，再发布 HasData
                _QueuePoint[_WriteIndex]._Data = o;
                _QueuePoint[_WriteIndex]._Sign.store(1, std::memory_order_release);

                if (++_WriteIndex >= _Size) _WriteIndex = 0;
                _WriteCount.fetch_add(1, std::memory_order_relaxed);
            }

            bool Pull(OUT T& o) override {
                AUTO_LOCK(_PullLock);
                AUTO_LOCK(_CapacityLock); // 与扩容互斥

                if (_QueuePoint[_ReadIndex]._Sign.load(std::memory_order_acquire) != 1)
                    return false;

                o = _QueuePoint[_ReadIndex]._Data;
                _QueuePoint[_ReadIndex]._Sign.store(0, std::memory_order_release);

                if (++_ReadIndex >= _Size) _ReadIndex = 0;
                _ReadCount.fetch_add(1, std::memory_order_relaxed);
                return true;
            }

            int Count() override {
                // 仅用于统计；跨线程可见用原子读
                const uint64_t w = _WriteCount.load(std::memory_order_acquire);
                const uint64_t r = _ReadCount.load(std::memory_order_acquire);
                return (int)((w >= r) ? (w - r) : 0);
            }

        private:
            // 可选：限制 T，避免 UB（如果你确实需要支持非平凡 T，就必须写显式 move/构造/析构）
            // static_assert(std::is_trivially_copyable<T>::value, "T 必须是平凡可拷贝类型");

            SpinLock _CapacityLock; // 仅用于扩容与 Pop 的互斥
            SpinLock _PushLock;
            SpinLock _PullLock;

            QueueSpace<T> _Queue[Size];
            QueueSpace<T>* _QueuePoint;
            int _Size;

            int _ReadIndex;
            int _WriteIndex;

            std::atomic<uint64_t> _ReadCount;
            std::atomic<uint64_t> _WriteCount;
        };
    }
}

#endif //__SafeQueue_h__
