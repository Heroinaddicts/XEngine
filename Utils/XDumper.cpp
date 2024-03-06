#ifdef WIN32
#include "XDumper.h"
#include "MultiSys.h"
#include <string>
#include <assert.h>
#include <tchar.h>
#pragma comment(lib, "dbghelp.lib")

static char s_DumperName[MAX_PATH * 2] = { 0 };
#define SnArraySize(x) sizeof(x) / sizeof(x[0])

namespace XEngine {
    char* SnFindLastOfChar(const char* pSrc, const char* keyChar) {
        assert(nullptr != pSrc && nullptr != keyChar);
        size_t    nKeyLen = strlen(keyChar);
        const char* pFindPos = nullptr;
        const char* pLabor = pSrc;
        while ((pLabor = strstr(pLabor, keyChar)) != nullptr) {
            pLabor += nKeyLen;
            pFindPos = pLabor;
        }

        return    const_cast<char*>(pFindPos);
    }

    static std::string wstringToString(const wchar_t* pwText, UINT code) {
        //assert(pwText);
        int iNeedSize = ::WideCharToMultiByte(code, 0, pwText, -1, nullptr, 0, nullptr, nullptr);

        if (iNeedSize >= 4096) {
            char* pchBuffer = xnew char[iNeedSize + 1];
            memset(pchBuffer, 0, (iNeedSize + 1) * sizeof(char));
            WideCharToMultiByte(code, 0, pwText, -1, pchBuffer, iNeedSize + 1, nullptr, nullptr);

            std::string strResult(pchBuffer);
            delete[]pchBuffer;
            return strResult;
        }
        else {
            //assert(pwText);
            char pchBuffer[4096] = { '\0' };

            ::WideCharToMultiByte(code, 0, pwText, -1, pchBuffer, sizeof(pchBuffer), nullptr, nullptr);
            return std::string(pchBuffer);
        }
    }

    bool IsDataSectionNeeded(const wchar_t* pModuleName) {
        if (nullptr == pModuleName) {
            return false;
        }

        std::string strTemp = wstringToString(pModuleName, CP_ACP);
        char szFileName[MAX_PATH] = { "\0" };
        _splitpath_s(strTemp.c_str(), nullptr, 0, nullptr, 0, szFileName, SnArraySize(szFileName), nullptr, 0);
        return (0 == _stricmp(szFileName, "ntdll") ? true : false);
    }

    XDumper& XDumper::GetInstance() {
        static XDumper s_Dumper;
        return s_Dumper;
    }

    XDumper::~XDumper(void) {
        if (nullptr != m_previousFilter) {
            SetUnhandledExceptionFilter(m_previousFilter);
            m_previousFilter = nullptr;
        }
    }

    LONG __stdcall XDumper::SnUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo) {
        XDumper::CreateMinidump(pExceptionInfo);

        exit(pExceptionInfo->ExceptionRecord->ExceptionCode);

        return EXCEPTION_EXECUTE_HANDLER;
    }

    bool CALLBACK XDumper::MiniDumpCallback(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput) {
        if (pInput == 0 || pOutput == 0) return FALSE;
        switch (pInput->CallbackType) {
        case ModuleCallback:
            if (pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
                if (!IsDataSectionNeeded(pInput->Module.FullPath))
                    pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
            // fall through
        case IncludeModuleCallback:
        case IncludeThreadCallback:
        case ThreadCallback:
        case ThreadExCallback:
            return TRUE;
        default:;
        }
        return FALSE;
    }

    void XDumper::CreateMinidump(EXCEPTION_POINTERS* pep) {
        HANDLE hFile = CreateFileA(s_DumperName,
            GENERIC_READ | GENERIC_WRITE,
            0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

        if ((hFile != nullptr) && (hFile != INVALID_HANDLE_VALUE)) {
            MINIDUMP_EXCEPTION_INFORMATION mdei;
            mdei.ThreadId = GetCurrentThreadId();
            mdei.ExceptionPointers = pep;
            mdei.ClientPointers = FALSE;
            MINIDUMP_CALLBACK_INFORMATION mci;
            mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)(XDumper::MiniDumpCallback);
            mci.CallbackParam = 0;

            MINIDUMP_TYPE mdt = MiniDumpNormal;

            BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
                hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci);

            CloseHandle(hFile);
        }
    }

    XDumper::XDumper(void) {
        ::GetModuleFileNameA(0, s_DumperName, SnArraySize(s_DumperName));

        bool    bModified = false;
        char* pszDot = SnFindLastOfChar(s_DumperName, ".");
        if (pszDot) {
            if (strlen(pszDot) >= 3) {
                bModified = true;
                strcpy_s(pszDot, sizeof("dmp") / sizeof(char) + 1, "dmp");
            }
        }

        if (!bModified) {
            strcat_s(s_DumperName, ".dmp");
        }

        m_previousFilter = ::SetUnhandledExceptionFilter(XDumper::SnUnhandledExceptionFilter);
    }

    void XDumper::SetDumpFilename(const char* dumpName) {
        if (nullptr != dumpName) {
            strcpy_s(s_DumperName, SnArraySize(s_DumperName), dumpName);
        }
    }
}

#endif //WIN32