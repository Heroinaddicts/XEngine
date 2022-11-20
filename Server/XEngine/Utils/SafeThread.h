#ifndef __SafeThread_h__
#define __SafeThread_h__

namespace XEngine {
    class SafeThread {
    public:
        SafeThread() : _IsClose(true) {}
        virtual ~SafeThread() {}

        virtual bool Start(const int thread_count = 1, void* context = nullptr);

    protected:
        virtual void Run(void* constext) = 0;

        __forceinline bool IsClose() {
            return _IsClose;
        }

        __forceinline void Close() {
            _IsClose = true;
        }

    private:
        bool _IsClose;

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
