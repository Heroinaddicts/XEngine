#ifndef __XBuffer_h__
#define __XBuffer_h__

#include "SafeMemory.h"

namespace XEngine {
    class XBuffer {
    public:
        XBuffer(const unsigned int size) : _Size(size), _In(0), _Out(0) {
            XASSERT(size > 0, "wtf");
            _Data = xnew char[size];
        }

        ~XBuffer() {
            xdel[] _Data;
        }

        inline bool In(const void* data, const unsigned int size) {
            if (size == 0) {
                return true;
            }

            XASSERT(_In >= _Out && _In <= _Size, "wtf");
            if (_Size - Length() < size) {
                char* old_data = _Data;
                const int old_size = _Size;

                while (_Size - Length() < size) {
                    _Size *= 2;
                }

                _Data = xnew char[_Size];

                if (_In - _Out > 0) {
                    SafeMemory::Memcpy(_Data, _Size, old_data + _Out, _In - _Out);
                }

                _In -= _Out;
                _Out = 0;
                xdel old_data;

            }

            if (_Size - _In < size) {
                if (_In == _Out) {
                    _In = 0;
                    _Out = 0;
                }
                else {
                    SafeMemory::Memmove(_Data, _Size, _Data + _Out, _In - _Out);
                    _In -= _Out;
                    _Out = 0;
                }
            }

            XASSERT(_In < _Size&& _Out < _Size, "wtf");
            SafeMemory::Memcpy(_Data + _In, _Size - Length(), data, size);
            _In += size;
            XASSERT(_In <= _Size, "wtf");

            return true;
        }

        inline bool Out(const unsigned int size) {
            XASSERT(_Out + size <= _In && _In <= _Size, "wtf");
            if (_Out + size > _In) {
                return false;
            }

            _Out += size;
            return true;
        }

        inline const void* GetData() const {
            return _Data + _Out;
        }

        inline const unsigned int Length() const {
            XASSERT(_In >= _Out && _In <= _Size, "wtf");
            return _In - _Out;
        }

        inline void Clear() {
            _In = 0;
            _Out = 0;
        }

    private:
        unsigned int _Size;
        unsigned int _In;
        unsigned int _Out;
        char* _Data;
    };
}

#endif //__XBuffer_h__

