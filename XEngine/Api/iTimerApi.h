#ifndef __iTimerApi_h__
#define __iTimerApi_h__

#include "MultiSys.h"

namespace XEngine {
    namespace Api {
        class iTimer {
        public:
            virtual ~iTimer() {}

            virtual void OnStart(const int id, void* const context, const int64 timestamp) = 0;
            virtual void OnTimer(const int id, void* const context, const int64 timestamp) = 0;
            virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) = 0;

            virtual void OnPause(const int id, void* const context, const int64 timestamp) = 0;
            virtual void OnResume(const int id, void* const context, const int64 timestamp) = 0;
        };

        class iTimerApi {
        public:
            virtual ~iTimerApi() {}

#define     START_TIMER(engine, timer, id, delay, count, interval, context) engine->GetTimerApi()->StartTimer(timer, id, delay, count, interval, context, __FILE__, __LINE__)
            virtual void StartTimer(iTimer* timer, const int id, const int delay, const int count, const int interval, void* const context, const char* const file, const int line) = 0;
            virtual void KillTimer(iTimer* timer, const int id, void* const context = nullptr) = 0;
            virtual void PauseTimer(iTimer* timer, const int id, void* const context = nullptr) = 0;
            virtual void ResumeTimer(iTimer* timer, const int id, void* const context = nullptr) = 0;

        };

        const int Unlimited = -1;
    }
}

#endif //__iTimerApi_h__
