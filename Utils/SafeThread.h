#ifndef __SafeThread_h__
#define __SafeThread_h__

namespace XEngine {
	class SafeThread {
	public:
		virtual ~SafeThread() {}

		virtual bool Start(void* context);

	protected:
		virtual void Run(void* constext) = 0;

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
