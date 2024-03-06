#include "Log.h"
#include "Launch.h"

namespace XEngine {
    Log* Log::GetInstance() {
        static Log s_Instance;
        return &s_Instance;
    }
    bool Log::Initialize(Api::iEngine* const engine) {
        std::string path = SafeSystem::File::GetDirectoryOfCurrentExe();
        const char* logpath = engine->GetLaunchParameter("log");
        if (logpath) {
            path += "/" + std::string(logpath);
        }

        _SyncLogFilePath = path + "/" + Launch::GetInstance()->GetName() + "-sync-" + SafeString::Int64ToString(SafeSystem::Process::GetCurrentProcessID()) + ".log";
        _AsyncLogFilePath = path + "/" + Launch::GetInstance()->GetName() + "-async-" + SafeString::Int64ToString(SafeSystem::Process::GetCurrentProcessID()) + ".log";

        if (!_SyncLogFile.Open(_SyncLogFilePath)) {
            return false;
        }

        if (!_AsyncLogFile.Open(_AsyncLogFilePath)) {
            return false;
        }

        Start();
        return true;
    }

    bool Log::Launch(Api::iEngine* const engine) {
        return true;
    }

    void Log::Release(Api::iEngine* const engine) {
    }

    void Log::EarlyUpdate(Api::iEngine* const engine) {
    }

    void Log::Update(Api::iEngine* const engine) {
    }

    void Log::LaterUpdate(Api::iEngine* const engine) {
        LogContent* lc = nullptr;
        AUTO_LOCK(_AsyncLogLock);
        while (_LogContentReleaseQueue.Pull(lc)) {
            XPOOL_RELEASE(_LogContentPool, lc);
        }
    }

    void Log::LogAsync(const char* header, const char* content, const char* file, const int line) {
        AUTO_LOCK(_AsyncLogLock);
        LogContent* lc = XPOOL_CREATE(_LogContentPool, header, content, file, line, SafeSystem::Time::GetMilliSecond());
        _LogContentQueue.Push(lc);
    }

    void Log::LogSync(const char* header, const char* content, const char* file, const int line) {
        UInt64 tick = SafeSystem::Time::GetMilliSecond();
        AUTO_LOCK(_SycnLogLock);
        std::string log = SafeSystem::Time::GetTimeStringByMillisecond(tick)
            + " " + header
            + " " + file
            + ":" + SafeString::Int64ToString(line)
            + " >> " + content + "\n";

        _SyncLogFile << log;
        _SyncLogFile.Save();
#ifdef _DEBUG
        printf(log.c_str());
#endif //_DEBUG
    }

    void Log::Run(void* context) {
        int count = 0;
        UInt64 tick = SafeSystem::Time::GetMilliSecond();
        while (!IsClose()) {
            LogContent* lc = nullptr;
            if (_LogContentQueue.Pull(lc)) {
                std::string content = SafeSystem::Time::GetTimeStringByMillisecond(lc->_Tick)
                    + " " + lc->_Head
                    + " " + lc->_File
                    + ":" + SafeString::Int64ToString(lc->_Line)
                    + " >> " + lc->_Content + "\n";
                _LogContentReleaseQueue.Push(lc);

                _AsyncLogFile << content;
                count++;
#ifdef _DEBUG
                printf(content.c_str());
#endif //_DEBUG
            }
            else {
                count = 0;
                SafeSystem::Time::MillisecondSleep(2);
            }

            UInt64 tick2 = SafeSystem::Time::GetMilliSecond();
            if (count > 0) {
                if (count >= 50 || tick2 - tick >= 50) {
                    _AsyncLogFile.Save();
                    tick = tick2;
                    count = 0;
                }
            }
        }
    }
}
