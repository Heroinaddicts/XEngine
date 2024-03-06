#include "SafeThread.h"
#ifdef WIN32
#include <process.h>
#endif //WIN32

#ifdef Linux
#include <pthread.h>
#endif //WIN32

namespace XEngine {

    struct ThreadArgs {
        SafeThread* _St;
        void* _Context;
    };

#ifdef WIN32
    unsigned int __stdcall SafeThread::ThreadProc(void* args) {
        ThreadArgs* p = (ThreadArgs*)args;
        XASSERT(p, "a null thread owner point");
        p->_St->Run(p->_Context);
        _endthreadex(0);
        xdel p;
        return 0;
    }
#endif //WIN32

#ifdef Linux
    void* SafeThread::ThreadProc(void* args) {
        ThreadArgs* p = (ThreadArgs*)args;
        XASSERT(p, "a null thread owner point");
        p->_St->Run(p->_Context);
        xdel p;
        return nullptr;
    }
#endif //Linux

    bool SafeThread::Start(const int thread_count, void* context) {
        _IsClose = false;
        ThreadArgs* p = xnew ThreadArgs;
        p->_St = this;
        p->_Context = context;
        for (int i = 0; i < thread_count; i++) {
#ifdef WIN32
            unsigned long long ret = _beginthreadex(nullptr, 0, ThreadProc, (void*)p, 0, nullptr);
            if (ret == 0) {
                return false;
            }
#else
            pthread_t ptid = 0;
            int ret = pthread_create(&ptid, nullptr, ThreadProc, (void*)p);
            if (ret != 0) {
                return false;
            }
#endif
        }
        return true;
    }
}

