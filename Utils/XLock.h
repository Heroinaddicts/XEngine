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
            _lock->Unlock();
        }

        LockAndAutoFree(iLock* lock, const char* file, const int line) : _lock(lock) {
            XASSERT(nullptr != lock, "lock nullptr");
            _lock->Lock(file, line);
        }

        void Free() {
            _lock->Unlock();
        }

    private:
        iLock* const _lock;
    };

    class SpinLock : public iLock {
    public:
        virtual ~SpinLock() {}
        SpinLock() : _lock_thread_id(SafeSystem::Process::INVAILD_ID), _ref(0) {}


        virtual bool TryLock() {
            return _flag.test_and_set(std::memory_order_acquire);
        }

        virtual void Lock(const char* file, const int line) {
            if (_lock_thread_id == SafeSystem::Process::INVAILD_ID || _lock_thread_id != SafeSystem::Process::GetCurrentThreadID()) {
                while (_flag.test_and_set(std::memory_order_acquire)) {}
                _lock_thread_id = SafeSystem::Process::GetCurrentThreadID();
            }
            _ref++;
#ifdef _DEBUG
            _file = file;
            _line = line;
#endif
        }

        virtual void Unlock() {
            XASSERT(_lock_thread_id == SafeSystem::Process::GetCurrentThreadID(), "wtf");
            _ref--;
            if (0 == _ref) {
                _lock_thread_id = SafeSystem::Process::INVAILD_ID;
                _flag.clear(std::memory_order_release);
            }
        }

        virtual void ForceUnlock() {
            _ref = 0;
            _lock_thread_id = SafeSystem::Process::INVAILD_ID;
            _flag.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag _flag = ATOMIC_FLAG_INIT;
        unsigned_int64 _lock_thread_id;
        int _ref;

#ifdef _DEBUG
        std::string _file;
        int _line;
#endif //_DEBUG
    };
}

#endif //__XLock_h__
