#ifndef __XLock_h__
#define __XLock_h__

#include "MultiSys.h"
#include "SafeSystem.h"
#include <atomic>
#include <string>

namespace XEngine {
    class iLock {
    public:
        virtual ~iLock() {}

        virtual bool TryLock() = 0;

        virtual void Lock(const char* file, const int line) = 0;
        virtual void Unlock() = 0;
        virtual void ForceUnlock() = 0;
    };

#define AUTO_LOCK(lock) LockAndAutoFree __auto_##lock__(&lock, __FILE__, __LINE__)
    class LockAndAutoFree {
    public:
        virtual ~LockAndAutoFree() {
            _Lock->Unlock();
        }

        LockAndAutoFree(iLock* lock, const char* file, const int line) : _Lock(lock) {
            XASSERT(nullptr != lock, "lock nullptr");
            _Lock->Lock(file, line);
        }

        void Free() {
            _Lock->Unlock();
        }

    private:
        iLock* const _Lock;
    };

    class SpinLock : public iLock {
    public:
        virtual ~SpinLock() {}
        SpinLock() : _LockThreadID(SafeSystem::Process::INVAILD_ID), _Ref(0) {}


        virtual bool TryLock() {
            return _Flag.test_and_set(std::memory_order_acquire);
        }

        virtual void Lock(const char* file, const int line) {
            if (_LockThreadID == SafeSystem::Process::INVAILD_ID || _LockThreadID != SafeSystem::Process::GetCurrentThreadID()) {
                while (_Flag.test_and_set(std::memory_order_acquire)) {}
                _LockThreadID = SafeSystem::Process::GetCurrentThreadID();
            }
            _Ref++;
#ifdef _DEBUG
            _File = file;
            _Line = line;
#endif
            //printf("Lock on %s:%d\n", _File.c_str(), _Line);
        }

        virtual void Unlock() {
            XASSERT(_LockThreadID == SafeSystem::Process::GetCurrentThreadID(), "wtf");
            _Ref--;
            if (0 == _Ref) {
                _LockThreadID = SafeSystem::Process::INVAILD_ID;
                _Flag.clear(std::memory_order_release);
            }
        }

        virtual void ForceUnlock() {
            _Ref = 0;
            _LockThreadID = SafeSystem::Process::INVAILD_ID;
            _Flag.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag _Flag = ATOMIC_FLAG_INIT;
        UInt64 _LockThreadID;
        int _Ref;

#ifdef _DEBUG
        std::string _File;
        int _Line;
#endif //_DEBUG
    };
}

#endif //__XLock_h__
