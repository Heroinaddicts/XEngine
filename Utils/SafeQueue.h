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
            eDataState _Sign;
            T _Data;
            QueueSpace() : _Sign(NoData) {}
        };

        template<typename T, int Size = 512, int Capacity = 512>
        class SpscQueue : public iQueue<T> {
        public:
            virtual ~SpscQueue() {
                if (_QueuePoint != _Queue) {
                    xfree(_QueuePoint);
                    _QueuePoint = nullptr;
                }
            }
            SpscQueue()
                : _Size(Size),
                _ReadIndex(0),
                _WriteIndex(0),
                _ReadCount(0),
                _WriteCount(0) {
                _QueuePoint = _Queue;
            }

            virtual void Push(const T& o) {
                if (_QueuePoint[_WriteIndex]._Sign != NoData) {
                    AUTO_LOCK(_Lock);
                    do {
                        if (_QueuePoint[_WriteIndex]._Sign == NoData) {
                            break;
                        }

                        const int newSize = _Size + Capacity;
                        QueueSpace<T>* oldQueuePoint = _QueuePoint;
                        _QueuePoint = (QueueSpace<T> *)xmalloc(newSize * sizeof(QueueSpace<T>));
                        SafeMemory::Memset(_QueuePoint, newSize * sizeof(QueueSpace<T>), 0, newSize * sizeof(QueueSpace<T>));
                        SafeMemory::Memcpy(_QueuePoint, newSize * sizeof(QueueSpace<T>), &oldQueuePoint[_WriteIndex], (_Size - _WriteIndex) * sizeof(QueueSpace<T>));
                        SafeMemory::Memcpy(&_QueuePoint[_Size - _WriteIndex], (newSize - (_Size - _WriteIndex)) * sizeof(QueueSpace<T>), oldQueuePoint, (_WriteIndex) * sizeof(QueueSpace<T>));
                        _ReadIndex = 0;
                        _WriteIndex = _Size;
                        _Size = newSize;
                        if (oldQueuePoint != _Queue) {
                            xfree(oldQueuePoint);
                        }
                    } while (false);
                }

                _QueuePoint[_WriteIndex]._Data = o;
                _QueuePoint[_WriteIndex++]._Sign = HasData;
                _WriteCount++;
                if (_WriteIndex >= _Size) {
                    _WriteIndex = 0;
                }
            }

            virtual bool Pull(OUT T& o) {
                AUTO_LOCK(_Lock);
                if (_QueuePoint[_ReadIndex]._Sign != HasData) {
                    return false;
                }

                o = _QueuePoint[_ReadIndex]._Data;
                _QueuePoint[_ReadIndex++]._Sign = NoData;
                _ReadCount++;
                if (_ReadIndex >= _Size) {
                    _ReadIndex = 0;
                }

                return true;
            }

            virtual int Count() {
                XASSERT(_WriteCount >= _ReadCount, "SpscQueue opt overstep");
                return _WriteCount - _ReadCount;
            }

        private:
            SpinLock _Lock;

            QueueSpace<T> _Queue[Size];
            QueueSpace<T>* _QueuePoint;
            int _Size;

            int _ReadIndex;
            int _WriteIndex;
            int _ReadCount;
            int _WriteCount;
        };
    }
}

#endif //__SafeQueue_h__
