#ifndef __XBuffer_h__
#define __XBuffer_h__

#include "SafeMemory.h"
#include "XLock.h"
#include <functional>

namespace XEngine {

    typedef std::function<UInt32(const void* const data, const UInt32 size)> fXBuffReadCallback;

    class IBuffer {
    public:
        virtual ~IBuffer() {}

        virtual bool Read(const fXBuffReadCallback& callback) = 0;
        virtual void Write(const void* const data, const UInt32 len) = 0;
        virtual void Clear() = 0;
    };

    template<UInt32 Size = 1024, UInt32 Capacity = 1024>
    class XBuffer : public IBuffer {
    public:
        virtual ~XBuffer() {
            if (_RealBuff != _OriginalBuff) {
                xdel(_RealBuff);
            }
        }

        XBuffer() : _Size(Size), _ReadIndex(0), _WriteIndex(0) {
            SafeMemory::Memset(_OriginalBuff, sizeof(_OriginalBuff), 0, sizeof(_OriginalBuff));
            _RealBuff = _OriginalBuff;
        }

        __forceinline bool Read(const fXBuffReadCallback& callback) {
            AUTO_LOCK(_Lock);
            XASSERT(_WriteIndex >= _ReadIndex, "Wtf");
            UInt32 useSize = callback(_RealBuff + _ReadIndex, _WriteIndex - _ReadIndex);
            if (useSize == 0) {
                return false;
            }

            _ReadIndex += useSize;
            return true;
        }

        __forceinline void Write(const void* const data, const UInt32 len) {
            XASSERT(_WriteIndex >= _ReadIndex, "Wtf");

            char* oldBuff = nullptr;
            if (GetTailSize() < len) {
                AUTO_LOCK(_Lock);
                if (GetTailSize() < len) {
                    if (GetFreeBuffSize() < len) {
                        oldBuff = _RealBuff;
#ifdef _DEBUG
                        UInt32 oldSize = _Size;
#endif //_DEBUG
                        do {
                            _Size += Capacity;
                        } while (GetFreeBuffSize() < len);
                        _RealBuff = (char*)xmalloc(_Size);
                        SafeMemory::Memcpy(_RealBuff, _Size, oldBuff + _ReadIndex, _WriteIndex - _ReadIndex);
#ifdef _DEBUG
                        printf("XBuffer Write, Increase Space old Size %lld, new Size %lld\n", oldSize, _Size);
#endif //_DEBUG
                    }
                    else {
#ifdef _DEBUG
                        printf("XBuffer Write, Move Copy, ReadIndex %lld, WirteIndex %lld, Copy Size %lld\n", _ReadIndex, _WriteIndex, _WriteIndex - _ReadIndex);
#endif //_DEBUG
                        if (_WriteIndex - _ReadIndex > 0) {
                            SafeMemory::Memcpy(_RealBuff, _Size, _RealBuff + _ReadIndex, _WriteIndex - _ReadIndex);
                        }
                    }
                    _WriteIndex -= _ReadIndex;
                    _ReadIndex = 0;
                }
            }

            SafeMemory::Memcpy(_RealBuff + _WriteIndex, _Size - _WriteIndex, data, len);
            _WriteIndex += len;

            if (oldBuff != nullptr && oldBuff != _OriginalBuff) {
                xfree(oldBuff);
            }
        }

        __forceinline void ReadOut(const UInt32 len) {
            AUTO_LOCK(_Lock);
            _ReadIndex += len;
        }

        __forceinline void Clear() {
            AUTO_LOCK(_Lock);
            _ReadIndex = 0;
            _WriteIndex = 0;
        }

    private:
        __forceinline UInt32 GetDataSize() const {
            return _WriteIndex - _ReadIndex;
        }

        __forceinline UInt32 GetFreeBuffSize() {
            return _Size - this->GetDataSize();
        }

        __forceinline UInt32 GetTailSize() {
            return _Size - _WriteIndex;
        }

    private:
        char _OriginalBuff[Size];
        char* _RealBuff;
        UInt32 _Size;
        SpinLock _Lock;

        UInt32 _ReadIndex;
        UInt32 _WriteIndex;
    };
}

#endif //__XBuffer_h__

