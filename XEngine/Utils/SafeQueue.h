#ifndef __SafeQueue_h__
#define __SafeQueue_h__

namespace XEngine {
    namespace SafeQueue {
        template<typename T>
        class iQueue {
        public:
            virtual ~iQueue() {}

            virtual bool Push(const T&) = 0;
            virtual bool Pull(OUT T&) = 0;
            virtual bool Count() = 0;

            virtual bool Empty() {
                return Count() == 0;
            }
        };

        enum {
            NoData = 0,
            HasData = 1
        };

        template<typename T>
        struct QueueSpace {
            char sign;
            T data;
            QueueSpace() : sign(NoData) {}
        };

        template<typename T>
        class SpscQueue : public iQueue<T> {
        public:
            virtual ~SpscQueue() {}
            SpscQueue(const int size)
                : _ReadIndex(0),
                _WriteIndex(0),
                _ReadCount(0),
                _WriteCount(0),
                _Size(size),
                _Queue(xnew QueueSpace<T>[size]) {
                XASSERT(_Queue, "Failed to allocate memory");
            }

            virtual bool Push(const T& o) {
                if (_Queue[_WriteIndex].sign != NoData) {
                    XASSERT(false, "SpscQueue is full");
                    return false;
                }

                _Queue[_WriteIndex].data = o;
                _Queue[_WriteIndex++].sign = HasData;
                _WriteCount++;
                if (_WriteIndex >= _Size) {
                    _WriteIndex = 0;
                }

                return true;
            }

            virtual bool Pull(OUT T& o) {
                if (_Queue[_ReadIndex].sign != HasData) {
                    return false;
                }

                o = _Queue[_ReadIndex].data;
                _Queue[_ReadIndex++].sign = NoData;
                _ReadCount++;

                if (_ReadIndex >= _Size) {
                    _ReadIndex = 0;
                }

                return true;
            }

            virtual bool Count() {
                XASSERT(_WriteCount >= _ReadCount, "SpscQueue opt overstep");
                return _WriteCount - _ReadCount;
            }

        private:
            const int _Size;
            QueueSpace<T>* const _Queue;

            int _ReadIndex;
            int _WriteIndex;
            int _ReadCount;
            int _WriteCount;
        };
    }
}

#endif //__SafeQueue_h__
