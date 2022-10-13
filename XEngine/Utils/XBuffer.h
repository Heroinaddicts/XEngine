#ifndef __XBuffer_h__
#define __XBuffer_h__

#include "SafeMemory.h"

namespace XEngine {
    class XBuffer {
    public:
        XBuffer(const unsigned int size) : _size(size), _in(0), _out(0) {
            XASSERT(size > 0, "wtf");
            _data = xnew char[size];
        }

        ~XBuffer() {
            xdel[] _data;
        }

        inline bool In(const void* data, const unsigned int size) {
            if (size == 0) {
                return true;
            }

            XASSERT(_in >= _out && _in <= _size, "wtf");
            if (_size - Length() < size) {
                char* old_data = _data;
                const int old_size = _size;

                while (_size - Length() < size) {
                    _size *= 2;
                }

                _data = xnew char[_size];

                if (_in - _out > 0) {
                    SafeMemory::Memcpy(_data, _size, old_data + _out, _in - _out);
                }

                _in -= _out;
                _out = 0;
                xdel old_data;

            }

            if (_size - _in < size) {
                if (_in == _out) {
                    _in = 0;
                    _out = 0;
                }
                else {
                    SafeMemory::Memmove(_data, _size, _data + _out, _in - _out);
                    _in -= _out;
                    _out = 0;
                }
            }

            XASSERT(_in < _size&& _out < _size, "wtf");
            SafeMemory::Memcpy(_data + _in, _size - Length(), data, size);
            _in += size;
            XASSERT(_in <= _size, "wtf");

            return true;
        }

        inline bool Out(const unsigned int size) {
            XASSERT(_out + size <= _in && _in <= _size, "wtf");
            if (_out + size > _in) {
                return false;
            }

            _out += size;
            return true;
        }

        inline const void* GetData() const {
            return _data + _out;
        }

        inline const unsigned int Length() const {
            XASSERT(_in >= _out && _in <= _size, "wtf");
            return _in - _out;
        }

    private:
        unsigned int _size;
        unsigned int _in;
        unsigned int _out;
        char* _data;
    };
}

#endif //__XBuffer_h__

