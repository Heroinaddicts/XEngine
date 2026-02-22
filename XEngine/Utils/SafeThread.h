#ifndef __SafeThread_h__
#define __SafeThread_h__

#include "MultiSys.h"
#include <functional>
#include "XLock.h"

namespace XEngine {
    typedef std::function<void(void* context)> fThread;
    class SafeThread {
    public:
        SafeThread() : _IsClose(true), _ThreadCount(0) {}
        virtual ~SafeThread() {}

        virtual bool Start(const int thread_count = 1, void* context = nullptr);

        static bool StartThread(fThread fun, void* context);

    protected:
        virtual void Run(void* context) = 0;

        __forceinline bool IsClose() {
            return _IsClose;
        }

        __forceinline void CloseThread() {
            _IsClose = true;
        }

        __forceinline int GetThreadCount() const {
            return _ThreadCount;
        }

    protected:
        bool _IsClose;

        SpinLock _Lock;
        int _ThreadCount;
    private:
#ifdef WIN32
        static unsigned int __stdcall ThreadProc(void* lpParam);
#endif //WIN32

#ifdef Linux
        static void* ThreadProc(void* pParam);
#endif //Linux
    };
}

#endif //__SafeThread_h__
