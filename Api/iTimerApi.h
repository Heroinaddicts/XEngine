#ifndef __iTimerApi_h__
#define __iTimerApi_h__

#include "MultiSys.h"

namespace XEngine {
    class TimeBase;
    namespace Api {
        class iTimer {
        public:
            virtual ~iTimer() {}

        protected:
            friend class XEngine::TimeBase;

            virtual void OnStart(const int id, const UInt64 context, const Int64 timestamp) {}
            virtual void OnTimer(const int id, const UInt64 context, const Int64 timestamp) {}
            virtual void OnEnd(const int id, const UInt64 context, bool nonviolent, const Int64 timestamp) {}

            virtual void OnPause(const int id, const UInt64 context, const Int64 timestamp) {}
            virtual void OnResume(const int id, const UInt64 context, const Int64 timestamp) {}
        };

        class iTimerApi {
        public:
            virtual ~iTimerApi() {}

#define     START_TIMER(engine, timer, id, delay, count, interval, context) engine->GetTimerApi()->StartTimer(timer, id, delay, count, interval, (UInt64)context, __FILE__, __LINE__)
#define     STOP_TIMER(engine, timer, id, context) engine->GetTimerApi()->KillTimer(timer, id, context);
            virtual void StartTimer(iTimer* timer, const int id, const int delay, const int count, const int interval, const UInt64 context, const char* const file, const int line) = 0;
            virtual void KillTimer(iTimer* timer, const int id, const UInt64 context = 0) = 0;
            virtual void PauseTimer(iTimer* timer, const int id, const UInt64 context = 0) = 0;
            virtual void ResumeTimer(iTimer* timer, const int id, const UInt64 context = 0) = 0;

        };

        const int Unlimited = -1;
    }
}

#endif //__iTimerApi_h__
