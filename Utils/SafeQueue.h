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
            no_data = 0,
            has_data = 1
        };

        template<typename T>
        struct QueueSpace {
            char sign;
            T data;
            QueueSpace() : sign(no_data) {}
        };

        template<typename T>
        class SpscQueue : public iQueue<T> {
        public:
            virtual ~SpscQueue() {}
            SpscQueue(const int size)
                : _read_index(0),
                _write_index(0),
                _read_count(0),
                _write_count(0),
                _size(size),
                _queue(xnew QueueSpace<T>[size]) {
                XASSERT(_queue, "Failed to allocate memory");
            }

            virtual bool Push(const T& o) {
                if (_queue[_write_index].sign != no_data) {
                    XASSERT(false, "SpscQueue is full");
                    return false;
                }

                _queue[_write_index].data = o;
                _queue[_write_index++].sign = has_data;
                _write_count++;
                if (_write_index >= _size) {
                    _write_index = 0;
                }

                return true;
            }

            virtual bool Pull(OUT T& o) {
                if (_queue[_read_index].sign != has_data) {
                    return false;
                }

                o = _queue[_read_index].data;
                _queue[_read_index++].sign = no_data;
                _read_count++;

                if (_read_index >= _size) {
                    _read_index = 0;
                }

                return true;
            }

            virtual bool Count() {
                XASSERT(_write_count >= _read_count, "SpscQueue opt overstep");
                return _write_count - _read_count;
            }

        private:
            const int _size;
            QueueSpace<T>* const _queue;

            int _read_index;
            int _write_index;
            int _read_count;
            int _write_count;
        };
    }
}

#endif //__SafeQueue_h__
