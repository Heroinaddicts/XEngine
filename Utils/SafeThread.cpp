#include "SafeThread.h"
#include "MultiSys.h"
#ifdef WIN32
#include <process.h>
#else
#include <pthread.h>
#endif //WIN32

namespace XEngine {

	struct ThreadArgs {
		SafeThread* st;
		void* context;
	};

#ifdef WIN32
	unsigned int __stdcall SafeThread::ThreadProc(void* args) {
		ThreadArgs* p = (ThreadArgs*)args;
		XASSERT(p, "a null thread owner point");
		p->st->Run(p->context);
		_endthreadex(0);
		return 0;
	}
#endif //WIN32

#ifdef Linux
	void* SafeThread::ThreadProc(void* args) {
		ThreadArgs* p = (ThreadArgs*)args;
		XASSERT(p, "a null thread owner point");
		p->st->Run(p->context);
		return p;
	}
#endif //Linux

	bool SafeThread::Start(void* context) {
		ThreadArgs args;
		args.st = this;
		args.context = context;
#ifdef WIN32
		unsigned long long ret = _beginthreadex(nullptr, 0, ThreadProc, (void*)&args, 0, nullptr);
		if (ret == 0) {
			return false;
		}
#else
		pthread_t ptid = 0;
		int ret = pthread_create(&ptid, nullptr, threadProc, (void*)&args);
		if (ret != 0) {
			return false;
		}
#endif
	}
}

