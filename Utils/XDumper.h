#ifndef __XDumper_h__
#define __XDumper_h__

#ifdef WIN32

#ifndef _WINDOWS_  
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif  
#include <ImageHlp.h>

namespace XEngine {
    class XDumper {
    public:
        static XDumper& GetInstance();
        ~XDumper();
        void SetDumpFilename(const char* dumpName);

    protected:
        static void CreateMinidump(EXCEPTION_POINTERS* pep);
        static long __stdcall SnUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
        static bool __stdcall MiniDumpCallback(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput);

    private:
        XDumper(void);
        XDumper(const XDumper& rhs) {}
    private:
        LPTOP_LEVEL_EXCEPTION_FILTER m_previousFilter;
    };
}

#endif //WIN32

#endif //__XDumper_h__

