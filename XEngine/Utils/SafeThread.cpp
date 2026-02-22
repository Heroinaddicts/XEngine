#include "SafeThread.h"
#ifdef WIN32
#include <Windows.h>
#include <process.h>
#endif //WIN32

#ifdef Linux
#include <pthread.h>
#endif //Linux

namespace XEngine {

    struct ThreadArgs {
        SafeThread* _St;
        fThread _Fun;
        void* _Context;
    };

#ifdef WIN32
    unsigned int __stdcall SafeThread::ThreadProc(void* args) {
        ThreadArgs* p = (ThreadArgs*)args;
        XASSERT(p, "a null thread owner point");
        p->_St->Run(p->_Context);
        LockAndAutoFree autolock(&p->_St->_Lock, __FILE__, __LINE__);
        p->_St->_ThreadCount--;
        _endthreadex(0);
        txdel p;
        return 0;
    }
#endif //WIN32

#ifdef Linux
    void* SafeThread::ThreadProc(void* args) {
        ThreadArgs* p = (ThreadArgs*)args;
        XASSERT(p, "a null thread owner point");
        p->_St->Run(p->_Context);
        LockAndAutoFree autolock(&p->_St->_Lock, __FILE__, __LINE__);
        p->_St->_ThreadCount--;
        txdel p;
        return nullptr;
    }
#endif //Linux

    bool SafeThread::Start(const int thread_count, void* context) {
        AUTO_LOCK(_Lock);
        _IsClose = false;
        for (int i = 0; i < thread_count; i++) {
            ThreadArgs* p = txnew ThreadArgs;
            p->_St = this;
            p->_Context = context;
#ifdef WIN32
            HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, ThreadProc, (void*)p, 0, nullptr);
            if (handle == 0) {
                txdel p;
                return false;
            }

            CloseHandle(handle);
#else
            pthread_t ptid = 0;
            int ret = pthread_create(&ptid, nullptr, ThreadProc, (void*)p);
            if (ret != 0) {
                txdel p;
                return false;
            }
#endif
            _ThreadCount++;
        }
        return true;
    }

#ifdef WIN32
    unsigned int __stdcall ThreadRunFun(void* args) {
        ThreadArgs* p = (ThreadArgs*)args;
        p->_Fun(p->_Context);
        _endthreadex(0);
        txdel p;
        return 0;
    }
#endif //WIN32

#ifdef Linux
    void* ThreadRunFun(void* args) {
        ThreadArgs* p = (ThreadArgs*)args;
        XASSERT(p, "a null thread owner point");
        p->_Fun(p->_Context);
        txdel p;
        return nullptr;
    }
#endif //Linux

    bool SafeThread::StartThread(fThread fun, void* context) {
        ThreadArgs* p = txnew ThreadArgs;
        p->_Fun = fun;
        p->_Context = context;
#ifdef WIN32
        HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, ThreadRunFun, (void*)p, 0, nullptr);
        if (handle == 0) {
            txdel p;
            return false;
        }
        CloseHandle(handle);
#else
        pthread_t ptid = 0;
        int ret = pthread_create(&ptid, nullptr, ThreadRunFun, (void*)p);
        if (ret != 0) {
            txdel p;
            return false;
        }
#endif
        return true;
    }
}

