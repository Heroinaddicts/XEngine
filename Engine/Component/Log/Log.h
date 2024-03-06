#ifndef __Log_h__
#define __Log_h__

#include "Header.h"

namespace XEngine {
    class Log : public iLog, public SafeThread {
    public:
        virtual ~Log() {}

        static Log* GetInstance();

        // 通过 iLog 继承
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;
        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        virtual void LogAsync(const char* header, const char* content, const char* file, const int line) override;
        virtual void LogSync(const char* header, const char* content, const char* file, const int line) override;

        // 通过 SafeThread 继承
        virtual void Run(void* context) override;

    private:
        XPool<LogContent> _LogContentPool;
        SafeQueue::SpscQueue<LogContent*, 81920> _LogContentQueue;
        SafeQueue::SpscQueue<LogContent*, 81920> _LogContentReleaseQueue;

        std::string _SyncLogFilePath;
        XFile _SyncLogFile;
        SpinLock _SycnLogLock;

        std::string _AsyncLogFilePath;
        XFile _AsyncLogFile;
        SpinLock _AsyncLogLock;
    };
}

#endif //__Log_h__
